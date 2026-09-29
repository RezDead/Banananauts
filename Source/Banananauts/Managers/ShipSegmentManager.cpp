// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentManager.h"

#include "Banananauts/Items/ItemBase.h"
#include "Banananauts/Utilities/ShipSegmentUtility.h"

AShipSegmentManager::AShipSegmentManager()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<UShipSegmentAttributes>(TEXT("Segment Attributes"));
	MaxHeat = 0.0f;
	MaxHeatAblation = 0.0f;
	MaxBananas = 0;
}

void AShipSegmentManager::AttachItem_Implementation(AActor* Item)
{
	//Error + Establish vars
	if (!Item->IsA<AItemBase>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipSegmentManager::AttachItem_Implementation - Tried to attach actor that is not an item"));
		return;
	}
	
	const UAbilitySystemComponent* ItemASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Item);
	
	if (!ItemASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipSegmentManager::AttachItem_Implementation - Tried to attach item without ASC"));
		return;
	}
	
	const UAbilitySystemComponent* ShipASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ShipRef);
	
	if (!ShipASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ShipSegmentManager::AttachItem_Implementation - ShipRef has no ASC"));
		return;
	}
	
	//Main Body
	
	AttachedItems.Add(Item);
	
	if (ItemASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Mass"))))
	{
		UShipSegmentUtility::AddItemMassToShip(ItemASC, ShipASC);
	}
	
	IAttachableSegment::AttachItem_Implementation(Item);
}

void AShipSegmentManager::BeginPlay()
{
	Super::BeginPlay();
	
	ShipRef = GetOwner();
	
	if (!ShipRef)
		UE_LOG(LogTemp, Error, TEXT("ShipSegmentManager::BeginPlay - ShipRef is null, how did we get here?"));
	
	
	if (AbilitySystemComponent)
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitAttributes();
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
