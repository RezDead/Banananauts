// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipManager.h"

#include "Banananauts/GAS/Effects/EModifyFuel.h"
#include "Banananauts/GAS/Effects/EModifyHeat.h"
#include "Banananauts/Utilities/ShipStatsUtility.h"
#include "Kismet/GameplayStatics.h"

AShipManager::AShipManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	MinFlightTime = 7.5f;
	FlightLinearGrowthRate = 0.05556f;
	
	SystemTickRate = 5.0f;
	BaseHeatGainPerTick = 1.0f;
	FuelBurnRate = 10.0f;
	
	MaxMass = 0.0f;
	MaxFuel = 0.0f;
		
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<UShipAttributes>(TEXT("ShipAttributes"));

	NoseSegmentComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("NoseSegment"));
	BodySegmentComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("BodySegment"));
	TailSegmentComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TailSegment"));

	NoseSegmentComponent->SetChildActorClass(AShipSegmentManager::StaticClass());
	BodySegmentComponent->SetChildActorClass(AShipSegmentManager::StaticClass());
	TailSegmentComponent->SetChildActorClass(AShipSegmentManager::StaticClass());
}

void AShipManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsFlying)
	{
		//Update ship progress
		ShipProgress += UShipStatsUtility::CalculateShipProgressAdditive(Attributes->GetMass(), FuelComposition.Thrust,
																	 MinFlightTime, FlightLinearGrowthRate, DeltaTime);
		
		if (ShipProgress >= 1)
		{
			const FTransform NewTransform = ShipRouter->GetTransformAtPercent(1.0f);
			SetActorLocationAndRotation(NewTransform.GetLocation(), NewTransform.GetRotation());
			
			Success();
		}
		
		//Update position
		const FTransform NewTransform = ShipRouter->GetTransformAtPercent(ShipProgress);
		SetActorLocationAndRotation(NewTransform.GetLocation(), NewTransform.GetRotation());
		
		ConsumeFuel(DeltaTime);
		
		if (Attributes->GetFuel() <= 0)
			Failure();
		
	}
}

void AShipManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitSegments();
	
	if (AbilitySystemComponent)
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitAttributes();
	
	InitiateFlight();
}

/**
 * Initializes ship segments. Throws fatal error if any of the segments are not initialized.
 */
void AShipManager::InitSegments()
{
	Nose = NoseSegmentComponent ? Cast<AShipSegmentManager>(NoseSegmentComponent->GetChildActor()) : nullptr;
	Body = BodySegmentComponent ? Cast<AShipSegmentManager>(BodySegmentComponent->GetChildActor()) : nullptr;
	Tail = TailSegmentComponent ? Cast<AShipSegmentManager>(TailSegmentComponent->GetChildActor()) : nullptr;
	
	if (!Nose || !Body || !Tail)
	{
		UE_LOG(LogTemp, Fatal, TEXT("ShipManager: Failed to initialize ship segments. Make sure these are spawned in and attached to the ship via editor."));
	}
	
	Segments.Add(Nose); Segments.Add(Body); Segments.Add(Tail);
}

/**
 * Initializes attributes to default values.
 */
void AShipManager::InitAttributes() const
{
	Attributes->InitMass(0.0f);
	Attributes->InitMaxMass(MaxMass);
	Attributes->InitFuel(0.0f);
	Attributes->InitMaxFuel(MaxFuel);
}

/**
 * Initiates the flight of the ship and all in-flight systems.
 */
void AShipManager::InitiateFlight()
{
	bIsFlying = true;
	
	ShipRouter = Cast<AShipRouter>(UGameplayStatics::GetActorOfClass(GetWorld(), AShipRouter::StaticClass()));
	if (!ShipRouter.IsValid())
	{
		UE_LOG(LogTemp, Fatal, TEXT("ShipManager: Failed to find ship router. Make sure it is spawned in world."));
	}
	
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &AShipManager::TickSystems, SystemTickRate, true);
}

/**
 * Calculates the number of bananas on the ship.
 * 
 * @return The number of bananas on the ship.
 */
int AShipManager::GetBananaCount()
{
	return UShipStatsUtility::GetBananaCount(Segments);
}

/**
 * Calculates the maximum number of bananas the ship can hold.
 * 
 * @return The maximum number of bananas the ship can hold.
 */
int AShipManager::GetBananaCapacity()
{
	return UShipStatsUtility::GetBananaCapacity(Segments);
}

/**
 * Uses the specified number of bananas from the ship.
 * 
 * @param Amount The number of bananas to use. Must be greater than 0.
 * @return True if the operation was successful, false otherwise.
 */
bool AShipManager::UseBananas(int Amount)
{
	return UShipStatsUtility::UseBananas(Amount, Segments);
}

/**
 * Adds the specified number of bananas to the ship.
 * 
 * @param Amount The number of bananas to add. Must be greater than 0.
 * @return True if any bananas were added, false if already at capacity or invalid input.
 */
bool AShipManager::AddBananas(int Amount)
{
	return UShipStatsUtility::AddBananas(Amount, Segments);
}

/**
 * Ticks all systems on the ship.
 */
void AShipManager::TickSystems()
{
	UpdateHeat();
}

/**
 * Updates the heat of all segments on the ship.
 */
void AShipManager::UpdateHeat() const
{
	for (const auto Segment : Segments)
	{
		UAbilitySystemComponent* ASC = Segment->GetAbilitySystemComponent();
		if (!ASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("ShipManager::UpdateHeat - Segment has no ASC"));
			continue;
		}
		const float HeatMult = UShipStatsUtility::CalculateHeatMult(Segment);
		
		const FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
			UEModifyHeat::StaticClass(),
			1.0f,
			ContextHandle
		);
		
		const FGameplayTag MagTag = FGameplayTag::RequestGameplayTag(FName("Data.Magnitude"));
		if (!MagTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("ShipManager::UpdateHeat - Invalid magnitude tag"));
			continue;
		}
		
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(MagTag, BaseHeatGainPerTick * HeatMult);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

/**
 * Consumes fuel at the rate of FuelBurnRate per minute, to be used on tick during flight.
 * 
 * @param DeltaTime Time since last tick.
 */
void AShipManager::ConsumeFuel(const float& DeltaTime) const
{
	if (!AbilitySystemComponent)
		return;
	
	//60 Converts minutes to seconds
	const float ConsumedFuel = DeltaTime / (FuelBurnRate * 60);
	
	const FGameplayTag MagTag = FGameplayTag::RequestGameplayTag(FName("Data.Magnitude"));
	if (!MagTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipManager::ConsumeFuel - Invalid magnitude tag"));
		return;;
	}
	
	const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		UEModifyFuel::StaticClass(),
		1.0f,
		ContextHandle
	);
	
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(MagTag, -ConsumedFuel);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

/**
 * What happens when the ship succeeds. Currently not implemented.
 */
void AShipManager::Success()
{
	UE_LOG(LogTemp, Warning, TEXT("ShipManager::Success - Game Won, need to add implementation"));
}

/**
 * What happens when the ship fails. Currently not implemented.
 */
void AShipManager::Failure()
{
	UE_LOG(LogTemp, Warning, TEXT("ShipManager::Failure - Game Lost, need to add implementation"));
}
