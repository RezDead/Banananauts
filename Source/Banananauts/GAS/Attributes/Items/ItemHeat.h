// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ItemHeat.generated.h"

/**
 * Heat attribute and handling for items.
 * 
 * Last Edited: 9/28/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UItemHeat : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData Heat;
	ATTRIBUTE_ACCESSORS_BASIC(UItemHeat, Heat)
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData MinHeat;
	ATTRIBUTE_ACCESSORS_BASIC(UItemHeat, MinHeat)
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData MaxHeat;
	ATTRIBUTE_ACCESSORS_BASIC(UItemHeat, MaxHeat)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
