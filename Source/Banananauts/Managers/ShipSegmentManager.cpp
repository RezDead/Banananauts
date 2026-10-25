// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentManager.h"
#include "Banananauts/Structs/ItemSaveInfo.h"
#include "Banananauts/Subsystems/SaveSubsystem.h"

AShipSegmentManager::AShipSegmentManager()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<UShipSegmentAttributes>(TEXT("Segment Attributes"));
	MaxHeat = 0.0f;
	MaxHeatAblation = 0.0f;
	MaxBananas = 0;
}

TArray<AActor*> AShipSegmentManager::GetAttachedItems() const
{
	TArray<AActor*> Ret;
	AttachedEffects.GetKeys(Ret);
	return Ret;
}

void AShipSegmentManager::OnSaveGame_Implementation(USaveData* SaveGameObject)
{
	ISaveable::OnSaveGame_Implementation(SaveGameObject);

	TArray<AActor*> AttachedItems = GetAttachedItems();
	TArray<FItemSaveInfo> ItemSaveInfo;

	for (const auto Item : AttachedItems)
	{
		FItemSaveInfo Info;
		Info.ItemType = Item->GetClass();
		Info.AttachedTo = SegmentType;

		const FTransform ItemTransform = Item->GetActorTransform();
		const FTransform SelfTransform = GetActorTransform();

		Info.RelativeTransform = ItemTransform.GetRelativeTransform(SelfTransform);

		ItemSaveInfo.Add(Info);
	}

	FSectionSaveWrapper Wrapper;
	Wrapper.SectionItems = ItemSaveInfo;

	SaveGameObject->ShipItems.Add(SegmentType, Wrapper);
}

void AShipSegmentManager::OnLoadGame_Implementation(USaveData* SaveGameObject)
{
	ISaveable::OnLoadGame_Implementation(SaveGameObject);

	if (!SaveGameObject->ShipItems.Contains(SegmentType)) return;

	TArray<FItemSaveInfo> ItemSaveInfo = SaveGameObject->ShipItems[SegmentType].SectionItems;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (auto Item : ItemSaveInfo)
	{
		const FTransform SpawnTransform = Item.RelativeTransform * GetActorTransform();

		AActor* SpawnedItem = GetWorld()->SpawnActor(Item.ItemType.Get(), &SpawnTransform, SpawnParams);
		Execute_AttachItem(this, SpawnedItem);
	}
}

void AShipSegmentManager::BeginPlay()
{
	Super::BeginPlay();
	
	ShipRef = GetParentActor();
	
	if (!ShipRef)
		UE_LOG(LogTemp, Error, TEXT("ShipSegmentManager::BeginPlay - ShipRef is null, how did we get here?"));
	
	
	if (AbilitySystemComponent)
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitAttributes();
	
	SetupAttributeListeners();
	
	Execute_InitSaveSubsystem(this);
}

void AShipSegmentManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (USaveSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		Subsystem->UnregisterSaveable(this);
	}
}

void AShipSegmentManager::InitSaveSubsystem_Implementation()
{
	ISaveable::InitSaveSubsystem_Implementation();
	
	if (USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		SaveSubsystem->RegisterSaveable(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ShipSegmentManager::InitSaveSubsystem - SaveSubsystem is null, how did we get here?"));
	}
}

/**
 * Initializes attributes to default values.
 */
void AShipSegmentManager::InitAttributes() const
{
	Attributes->InitHeat(0.0f);
	Attributes->InitMaxHeat(MaxHeat);
	Attributes->InitHeatAblation(0.0f);
	Attributes->InitMaxHeatAblation(MaxHeatAblation);
	Attributes->InitBananas(0.0f);
	Attributes->InitMaxBananas(MaxBananas);
}

/**
 * Sets up attribute listeners.
 */
void AShipSegmentManager::SetupAttributeListeners()
{
	StabilityListener = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetStabilityAttribute()).AddUObject(this, &AShipSegmentManager::OnStabilityChanged);
}

void AShipSegmentManager::OnStabilityChanged(const FOnAttributeChangeData& Data) const
{
	if (Attributes->GetStability() > Attributes->GetMaxStability())
	{
		AddTagToAllItems(FGameplayTag::RequestGameplayTag(FName("Status.Adhered")));
	}
}

/**
 * Adds a tag to all attached items. However, it does not add duplicate tags.
 */
void AShipSegmentManager::AddTagToAllItems(const FGameplayTag Tag) const
{
	for (AActor* Item : GetAttachedItems())
	{
		UAbilitySystemComponent* ItemASC = nullptr;
		if (const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Item))
		{
			ItemASC = ASCInterface->GetAbilitySystemComponent();
		}
		
		if (!ItemASC) {continue;}
		
		if (ItemASC->HasMatchingGameplayTag(Tag)) {continue;}
		ItemASC->AddLooseGameplayTag(Tag);
	}
}
