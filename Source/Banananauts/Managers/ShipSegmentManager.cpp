// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentManager.h"

#include "Banananauts/Items/ItemBase.h"

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
	if (!Item->IsA<AItemBase>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipSegmentManager::AttachItem_Implementation - Tried to attach actor that is not an item"));
		return;
	}
	
	const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Item);
	
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShipSegmentManager::AttachItem_Implementation - Tried to attach item without ASC"));
		return;
	}
	
	AttachedItems.Add(Item);
	
	if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Mass"))))
	{
		
	}
	
	IAttachableSegment::AttachItem_Implementation(Item);
}

void AShipSegmentManager::BeginPlay()
{
	Super::BeginPlay();
	
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
