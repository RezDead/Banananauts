// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "AbilitySystemComponent.h"
#include "Banananauts/Data/TRB_ItemInformation.h"
#include "Banananauts/GAS/Attributes/Items/ItemAblation.h"
#include "Banananauts/GAS/Attributes/Items/ItemHeat.h"
#include "Banananauts/GAS/Attributes/Items/ItemMass.h"
#include "Banananauts/GAS/Attributes/Items/ItemStability.h"


AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
}

void AItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (!ItemInfoDT)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoDT is not set in %s"), *GetName());
		return;
	}

	const FString ContextString((TEXT("Looking up item info in class %s"), *GetName()));

	if (const FTRB_ItemInformation* ItemRow = ItemInfoDT->FindRow<FTRB_ItemInformation>(*GetName(), ContextString))
	{
		DisplayName = ItemRow->DisplayName;
		ItemMesh->SetStaticMesh(ItemRow->Mesh);
		GameplayTags = ItemRow->Tags;
		AddItemTypeTag(ItemRow->Type);
		InitializeItemAttributes(ItemRow);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoDT does not contain a row for %s"), *GetName());
		return;
	}
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Initializes the item's attributes based on the provided item information row.
 * 
 * @param ItemRow The item information row containing attribute data.
 */
void AItemBase::InitializeItemAttributes(const FTRB_ItemInformation* ItemRow)
{
	if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Mass"))))
	{
		UItemMass* MassSet = NewObject<UItemMass>(this);
		MassSet->InitMass(ItemRow->Mass);
		AbilitySystemComponent->AddAttributeSetSubobject(MassSet);
	}
	
	if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Ablation"))))
	{
		UItemAblation* AblationSet = NewObject<UItemAblation>(this);
		AblationSet->InitHeatAblation(ItemRow->HeatAblation);
		AbilitySystemComponent->AddAttributeSetSubobject(AblationSet);
	}
	
	if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Heat"))))
	{
		UItemHeat* HeatSet = NewObject<UItemHeat>(this);
		HeatSet->InitHeat(ItemRow->MinHeat);
		HeatSet->InitMinHeat(ItemRow->MinHeat);
		HeatSet->InitMaxHeat(ItemRow->MaxHeat);
		AbilitySystemComponent->AddAttributeSetSubobject(HeatSet);
	}
	
	if (HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Stability"))))
	{
		UItemStability* StabilitySet = NewObject<UItemStability>(this);
		StabilitySet->InitStability(ItemRow->Stability);
		AbilitySystemComponent->AddAttributeSetSubobject(StabilitySet);
	}
	
	//User error logging
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Mass"))) && ItemRow->Mass != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s does not have a mass attribute tag but has a mass value."), *ItemRow->DisplayName.ToString());
	}
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Heat"))) && (ItemRow->MaxHeat != 0 || ItemRow->MinHeat != 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s does not have a heat attribute tag but has a heat value."), *ItemRow->DisplayName.ToString());
	}
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Ablation"))) && ItemRow->HeatAblation != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s does not have a ablation attribute tag but has a ablation value."), *ItemRow->DisplayName.ToString());
	}
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Attribute.Item.Stability"))) && ItemRow->Stability != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s does not have a stability attribute tag but has a stability value."), *ItemRow->DisplayName.ToString());
	}
}

/**
 * Adds a gameplay tag based on the item type.
 * 
 * @param ItemType The type of the item.
 */
void AItemBase::AddItemTypeTag(const EItemTypes& ItemType)
{
	switch (ItemType)
	{
		case EItemTypes::BoltOn:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.BoltOn")));
			break;
		
		case EItemTypes::Wall:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.Wall")));
			break;
		
		case EItemTypes::Activated:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.Activated")));
			break;
		
		case EItemTypes::Fuel:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.Fuel")));
			break;
		
		case EItemTypes::Catalyst:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.Catalyst")));
			break;
		
		case EItemTypes::Engine:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.Engine")));
			break;
		
		case EItemTypes::Thruster:
			GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Type.Item.Thruster")));
			break;
	}
}
