// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Banananauts/Enums/BurnStatus.h"
#include "Banananauts/GAS/Attributes/ShipSegmentAttributes.h"
#include "Banananauts/Interfaces/AttachableSegment.h"
#include "UObject/Object.h"
#include "ShipSegmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeatStatusChanged, EBurnStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBananasEmpty, bool, Empty);

/**
 * Holds the attributes and ASC that handles the state of the ship segments.
 * All data handling is handled by the attribute class.
 * 
 * Last Edited: 9/28/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS(PrioritizeCategories="Default")
class BANANANAUTS_API AShipSegmentManager : public AActor, public IAbilitySystemInterface, public IAttachableSegment,
                                            public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	AShipSegmentManager();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UShipSegmentAttributes* Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	virtual void AttachItem_Implementation(AActor* Item) override;
	
protected:
	virtual void BeginPlay() override;

private:
	void InitAttributes() const;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float MaxHeat = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float MaxHeatAblation = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	int MaxBananas = 0;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AttachedItems;
};
