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

void AShipSegmentManager::OnSaveGame_Implementation(USaveData* SaveGameObject, const ESaveType SaveEvent)
{
	ISaveable::OnSaveGame_Implementation(SaveGameObject, SaveEvent);

	switch (SaveEvent) {
	case ESaveType::Flight:
		OnFlightSaveEvent(SaveGameObject);
		break;
	case ESaveType::Fail:
		OnFailSaveEvent(SaveGameObject);
		break;
	default:;
	}

}

void AShipSegmentManager::OnLoadGame_Implementation(USaveData* SaveGameObject, ESaveType LoadEvent)
{
	ISaveable::OnLoadGame_Implementation(SaveGameObject, LoadEvent);
		
	if (LoadEvent != ESaveType::Flight) return;
	if (!SaveGameObject->ShipItems.Contains(SegmentType)) return;

	TArray<FItemSaveInfo> ItemSaveInfo = SaveGameObject->ShipItems[SegmentType].SectionItems;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (auto Item : ItemSaveInfo)
	{
		UClass* ItemClass = Item.ItemType.LoadSynchronous();
		if (!ItemClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load item class for spawning."));
			continue;
		}
		
		const FTransform SpawnTransform = Item.RelativeTransform * GetActorTransform();

		AActor* SpawnedItem = GetWorld()->SpawnActor(ItemClass, &SpawnTransform, SpawnParams);
		if (SpawnedItem)
		{
			Execute_AttachItem(this, SpawnedItem);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn item type: %s, on flight begin"), *ItemClass->GetName());
		}
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
	
	Execute_RegisterToSaveSubsystem(this);
}

void AShipSegmentManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
		
	Execute_UnregisterFromSaveSubsystem(this);
}

/**
 * What happens on a flight save event.
 */
void AShipSegmentManager::OnFlightSaveEvent(USaveData* SaveGameObject) const
{
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

/**
 * What happens on a fail save event.
 */
void AShipSegmentManager::OnFailSaveEvent(USaveData* SaveGameObject) const
{
	TArray<AActor*> AttachedItems = GetAttachedItems();
	auto& FailItems = SaveGameObject->FailItems;
	
	for (const auto Item : AttachedItems)
	{
		int& ItemNum = FailItems.FindOrAdd(Item->GetClass());
		ItemNum++;
	}
}

void AShipSegmentManager::RegisterToSaveSubsystem_Implementation()
{
	ISaveable::RegisterToSaveSubsystem_Implementation();
	
	if (USaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		SaveSubsystem->RegisterSaveable(this, ESaveType::Flight);
		SaveSubsystem->RegisterSaveable(this, ESaveType::Fail);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ShipSegmentManager::InitSaveSubsystem - SaveSubsystem is null, how did we get here?"));
	}
}

void AShipSegmentManager::UnregisterFromSaveSubsystem_Implementation()
{
	ISaveable::UnregisterFromSaveSubsystem_Implementation();
	
	if (USaveSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		Subsystem->UnregisterSaveable(this, ESaveType::Flight);
		Subsystem->UnregisterSaveable(this, ESaveType::Fail);
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
