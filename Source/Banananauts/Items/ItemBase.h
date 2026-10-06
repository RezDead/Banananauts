// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Banananauts/Data/TRB_ItemInformation.h"
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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGameplayTagContainer GameplayTags;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	UDataTable* ItemInfoDT;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Data")
	FName DisplayName;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
private:
	void InitializeItemAttributes(const FTRB_ItemInformation* ItemRow);
	void InitializeTypeTags(const EItemTypes& ItemType);
	void LogItemAttributeErrors(const FTRB_ItemInformation* ItemRow) const;
};
