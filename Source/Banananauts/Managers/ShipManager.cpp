// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipManager.h"

#include "Banananauts/GAS/Attributes/Items/ItemStability.h"
#include "Banananauts/GAS/Effects/EModifyFuel.h"
#include "Banananauts/GAS/Effects/EModifyHeat.h"
#include "Banananauts/Utilities/ShipStatsUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Banananauts/Ship/RandomEventComponent.h"

AShipManager::AShipManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	LaunchDuration = 10.0f;
	LaunchDelay = 2.5f;
	
	MinFlightTime = 7.5f;
	FlightLinearGrowthRate = 0.05556f;
	
	SystemTickRate = 5.0f;
	BaseHeatGainPerTick = 1.0f;
	FuelBurnRate = 10.0f;
	
	MaxMass = 0.0f;
	MaxFuel = 0.0f;
	
	Status = EShipStatus::Clear;
	
	FuelThreshold = 1.0f/3.0f;
	StabilityThreshold = 0.0f;
	HeatThreshold = 1.0f/3.0f;
	
	
	DefineStateTransitions();
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
		
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<UShipAttributes>(TEXT("ShipAttributes"));
	
	RandomEventComponent = CreateDefaultSubobject<URandomEventComponent>(TEXT("RandomEventComponent"));

	NoseSegmentComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("NoseSegment"));
	BodySegmentComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("BodySegment"));
	TailSegmentComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TailSegment"));
	
	NoseSegmentComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BodySegmentComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TailSegmentComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

/**
 * Begins launch sequence for the ship
 */
void AShipManager::BeginLaunchSequence_Implementation()
{
	IShipCommunication::BeginLaunchSequence_Implementation();
	
	/*if (Attributes->GetMass() >= Attributes->GetMaxMass() || Attributes->GetFuel() <= 0)
	{
		LaunchFailed();
		return;
	}*/
	
	InitShipRouter();
	
	FTimerHandle LaunchDelayHandle;
	GetWorld()->GetTimerManager().SetTimer(LaunchDelayHandle, this, &AShipManager::Launch, LaunchDelay, false);
}

void AShipManager::InitShipRouter()
{
	ShipRouter = Cast<AShipRouter>(UGameplayStatics::GetActorOfClass(GetWorld(), AShipRouter::StaticClass()));
	if (!ShipRouter.IsValid())
	{
		UE_LOG(LogTemp, Fatal, TEXT("ShipManager: Failed to find ship router. Make sure it is spawned in world."));
	}
	
	ShipRouter->SetShip(this);
	ShipRouter->SuccessDelegate.AddDynamic(this, &AShipManager::Success);
	ShipRouter->FailureDelegate.AddDynamic(this, &AShipManager::Failure);
}

void AShipManager::Launch_Implementation()
{
	bIsLaunching = true;
}

/**
 * Initiates the flight of the ship and all in-flight systems.
 */
void AShipManager::BeginFlightSequence_Implementation()
{
	IShipCommunication::BeginFlightSequence_Implementation();
	
	bIsFlying = true;
	
	InitShipRouter();
	
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &AShipManager::TickSystems, SystemTickRate, true);
	RandomEventComponent->BeginRandomEvents();
}

/**
 * Calculates the number of bananas on the ship.
 * 
 * @return The number of bananas on the ship.
 */
int AShipManager::GetBananaCount()
{
	return UShipStatsUtility::GetAttributeTotal(UShipSegmentAttributes::GetBananasAttribute(), Segments);
}

/**
 * Calculates the maximum number of bananas the ship can hold.
 * 
 * @return The maximum number of bananas the ship can hold.
 */
int AShipManager::GetBananaCapacity() const
{
	return UShipStatsUtility::GetAttributeTotal(UShipSegmentAttributes::GetBananasAttribute(), Segments);
}

/**
 * Uses the specified number of bananas from the ship.
 * 
 * @param Amount The number of bananas to use. Must be greater than 0.
 * @return True if the operation was successful, false otherwise.
 */
bool AShipManager::UseBananas(int Amount) const
{
	return UShipStatsUtility::UseBananas(Amount, Segments);
}

/**
 * Adds the specified number of bananas to the ship.
 * 
 * @param Amount The number of bananas to add. Must be greater than 0.
 * @return True if any bananas were added, false if already at capacity or invalid input.
 */
bool AShipManager::AddBananas(int Amount) const
{
	return UShipStatsUtility::AddBananas(Amount, Segments);
}

/**
 * Gets total heat across all segments
 * @return Total heat of segments
 */
float AShipManager::GetHeat() const
{
	return UShipStatsUtility::GetAttributeTotal(UShipSegmentAttributes::GetHeatAttribute(), Segments);
}

/**
 * Gets total max heat across all segments
 * @return Total max heat of segments
 */
float AShipManager::GetMaxHeat() const
{
	return UShipStatsUtility::GetAttributeTotal(UShipSegmentAttributes::GetMaxHeatAttribute(), Segments);
}

/**
* Gets total Stability across all segments
 * @return Total Stability of segments
 */
float AShipManager::GetStability() const
{
	return UShipStatsUtility::GetAttributeTotal(UShipSegmentAttributes::GetStabilityAttribute(), Segments);
}

/**
* Gets total max Stability across all segments
 * @return Total max Stability of segments
 */
float AShipManager::GetMaxStability() const
{
	return UShipStatsUtility::GetAttributeTotal(UShipSegmentAttributes::GetMaxStabilityAttribute(), Segments);
}

void AShipManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	HandleLaunch(DeltaTime);
	
	HandleFlight(DeltaTime);
}

void AShipManager::HandleFlight(const float DeltaTime) const
{
	if (bIsFlying)
	{
		//Update ship progress
		const float DeltaProgress = UShipStatsUtility::CalculateShipProgressAdditive(Attributes->GetMass(), FuelComposition.Thrust,
			MinFlightTime, FlightLinearGrowthRate, DeltaTime);
		
		ShipRouter->AddShipProgress(DeltaProgress);
		
		ConsumeFuel(DeltaTime);
		
		if (Attributes->GetFuel() <= 0)
			ShipRouter->FailureDelegate.Broadcast();
	}
}

void AShipManager::HandleLaunch(const float DeltaTime) const
{
	if (bIsLaunching)
	{
		const float DeltaProgress = DeltaTime * 1/LaunchDuration;
		
		ShipRouter->AddShipProgress(DeltaProgress);
	}
}

void AShipManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitSegments();
	
	if (AbilitySystemComponent)
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitAttributes();
	
	SetupAttributeListeners();

	UpdateStatus();
	
	RandomEventComponent->SetShipManager(this);
}

/**
 * What happens when the ship fails to launch.
 */
void AShipManager::LaunchFailed_Implementation()
{
	
}

/**
 * Sets up attribute listeners for both the ship and ship segments.
 */
void AShipManager::SetupAttributeListeners()
{
	if (!AbilitySystemComponent || !Nose || !Body || !Tail)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipManager::SetupAttributeListeners - ASC, Nose, Body, or Tail is null"));
		return;
	}
	
	UAbilitySystemComponent* NoseASC = Nose->GetAbilitySystemComponent();
	UAbilitySystemComponent* BodyASC = Body->GetAbilitySystemComponent();
	UAbilitySystemComponent* TailASC = Tail->GetAbilitySystemComponent();
	
	if (!NoseASC || !BodyASC || !TailASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipManager::SetupAttributeListeners - Nose, Body, or Tail ASC is null"));
		return;
	}
	
	FuelListener = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UShipAttributes::GetFuelAttribute()).AddUObject(this, &AShipManager::OnFuelChanged);
	
	HeatListeners.Add(EShipSection::Nose, NoseASC->GetGameplayAttributeValueChangeDelegate(UShipSegmentAttributes::GetHeatAttribute()).AddUObject(this, &AShipManager::OnHeatChanged));
	HeatListeners.Add(EShipSection::Body, BodyASC->GetGameplayAttributeValueChangeDelegate(UShipSegmentAttributes::GetHeatAttribute()).AddUObject(this, &AShipManager::OnHeatChanged));
	HeatListeners.Add(EShipSection::Tail, TailASC->GetGameplayAttributeValueChangeDelegate(UShipSegmentAttributes::GetHeatAttribute()).AddUObject(this, &AShipManager::OnHeatChanged));
	
	StabilityListeners.Add(EShipSection::Nose, NoseASC->GetGameplayAttributeValueChangeDelegate(UShipSegmentAttributes::GetStabilityAttribute()).AddUObject(this, &AShipManager::OnStabilityChanged));
	StabilityListeners.Add(EShipSection::Body, BodyASC->GetGameplayAttributeValueChangeDelegate(UShipSegmentAttributes::GetStabilityAttribute()).AddUObject(this, &AShipManager::OnStabilityChanged));
	StabilityListeners.Add(EShipSection::Tail, TailASC->GetGameplayAttributeValueChangeDelegate(UShipSegmentAttributes::GetStabilityAttribute()).AddUObject(this, &AShipManager::OnStabilityChanged));
}

void AShipManager::OnFuelChanged(const FOnAttributeChangeData& Data)
{
	UpdateStatus();
}

void AShipManager::OnHeatChanged(const FOnAttributeChangeData& Data)
{
	UpdateStatus();
}

void AShipManager::OnStabilityChanged(const FOnAttributeChangeData& Data)
{
	UpdateStatus();
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
	
	Segments.Add(EShipSection::Nose, Nose);
	Segments.Add(EShipSection::Body, Body);
	Segments.Add(EShipSection::Tail, Tail);
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
	for (const TPair<EShipSection, AShipSegmentManager*>& SegmentPair : Segments)
	{
		AShipSegmentManager* Segment = SegmentPair.Value;
		if (!Segment)
		{
			UE_LOG(LogTemp, Warning, TEXT("ShipManager::UpdateHeat - Segment is null"));
			continue;
		}
		
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
	if (bIsLaunching)
	{
		//Launch Complete, load flight level
		if (FlightLevel.GetAssetName().IsEmpty())
		{
			UE_LOG(LogTemp, Fatal, TEXT("ShipManager::Success - Invalid level to load, make sure Flight Level is valid"));
			return;
		}
	
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), FlightLevel);
	} 
	else if (bIsFlying)
	{
		//Game Win
	}
}

/**
 * What happens when the ship fails. Currently not implemented.
 */
void AShipManager::Failure()
{
	if (bIsChangingLevel)
		return;
	
	UE_LOG(LogTemp, Display, TEXT("ShipManager::Failure - Game Lost"));
	
	bIsChangingLevel = true;
	
	const TSoftObjectPtr<UWorld> LevelToLoad = UShipStatsUtility::GetRandomLevel(LevelsDT);
	
	if (LevelToLoad.GetAssetName().IsEmpty())
	{
		UE_LOG(LogTemp, Fatal, TEXT("ShipManager::Failure - Invalid level to load, make sure Levels DT is valid"));
		return;
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelToLoad);
}

/**
 * Defines the state transitions for the ship status. Registers transition functions for each valid state-to-state transition
 * in the TransitionRegistry. Each transition function is associated with a specific pair of starting and destination statuses.
 */
void AShipManager::DefineStateTransitions()
{
	TransitionRegistry.Add(GetTransitionKey(EShipStatus::Clear, EShipStatus::Warning), [this]() {
		OnClearToWarning();
	});
	
	TransitionRegistry.Add(GetTransitionKey(EShipStatus::Clear, EShipStatus::Fail), [this]()
	{
		OnClearToFail();
	});
	
	TransitionRegistry.Add(GetTransitionKey(EShipStatus::Warning, EShipStatus::Fail), [this]()
	{
		OnWarningToFail();
	});
	
	TransitionRegistry.Add(GetTransitionKey(EShipStatus::Warning, EShipStatus::Clear), [this]()
	{
		OnWarningToClear();
	});
	TransitionRegistry.Add(GetTransitionKey(EShipStatus::Fail, EShipStatus::Clear), [this]()
	{
		OnFailToClear();
	});
	
	TransitionRegistry.Add(GetTransitionKey(EShipStatus::Fail, EShipStatus::Warning), [this]()
	{
		OnFailToWarning();
	});
}

void AShipManager::OnClearToWarning()
{

}

void AShipManager::OnClearToFail()
{

}

void AShipManager::OnWarningToClear()
{

}

void AShipManager::OnWarningToFail()
{

}

void AShipManager::OnFailToClear()
{

}

void AShipManager::OnFailToWarning()
{

}

/**
 * Attempts to swap from one status to another, activating any logic that should occur.
 * @param NewStatus Status to swap to.
 */
void AShipManager::SetStatus(const EShipStatus NewStatus)
{
	if (Status == NewStatus) return;
	
	const EShipStatus OldStatus = Status;
	const FTransitionKey Key = GetTransitionKey(Status, NewStatus);

	// Check if illegal state swap
	if (!TransitionRegistry.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid status swap attempted from %d to %d"), (int32)Status, (int32)NewStatus);
		return;
	}
	
	// Commit status swap
	Status = NewStatus;
	
	// Execute the code mapped to this path
	TransitionRegistry[Key]();
	
	OnShipStatusChanged.Broadcast(OldStatus, NewStatus);
}

/**
 * Updates the ship status based on all conditions. This is where status conditions are implemented.
 */
void AShipManager::UpdateStatus()
{
	bool bFail = false;
	bool bWarning = false;
	
	//Status checks below here
	
	if (!CheckAttributeStatus())
	{
		bWarning = true;
	}
	
	//Update status hierarchically
	if (bFail)
	{
		SetStatus(EShipStatus::Fail);
	}
	else if (bWarning)
	{
		SetStatus(EShipStatus::Warning);
	}
	else
	{
		SetStatus(EShipStatus::Clear);
	}
}

/**
 * Checks if attributes clear the status threshold
 * @return True if clear, false if not
 */
bool AShipManager::CheckAttributeStatus() const
{
	float lMaxHeat = GetMaxHeat();
	float HeatPercent = lMaxHeat > 0 ? GetHeat() / lMaxHeat : 0.0f;
	
	float lMaxStability = GetMaxStability();
	float StabilityPercent = lMaxStability > 0 ? GetStability() / lMaxStability : 0.0f;
	
	float lMaxFuel = Attributes->GetMaxFuel();
	float FuelPercent = lMaxFuel > 0 ? Attributes->GetFuel() / lMaxFuel : 0.0f;
	
	if (HeatPercent <= HeatThreshold && StabilityPercent > StabilityThreshold && FuelPercent <= FuelThreshold)
	{
		return true;
	}
	return false;
}

void AShipManager::StartRandomEvent() const
{
	RandomEventComponent->BeginRandomEvents();
}

/**
 * Triggers a specified number of events on a specific ship section
 * @param Category Section to active event
 * @param Amount Number of events
 */
void AShipManager::TriggerEvent(const EShipSection Category, const int Amount) const
{
	UShipStatsUtility::PopOffEvent(Category, Segments, Amount);
}
