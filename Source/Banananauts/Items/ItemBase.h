// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Banananauts/GAS/Attributes/ItemAttributes.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

/**
 * Functionality/data that all items will share.
 * 
 * Last Edited: 9/27/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API AItemBase : public AActor, public IGameplayTagAssetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AItemBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UItemAttributes* Attributes;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data")
	UDataTable* ItemInfoDT;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
	
};
