// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ItemAttributes.generated.h"

/**
 * Attributes that items will have.
 * 
 * Last Edited: 9/27/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UItemAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Mass")
	FGameplayAttributeData Mass;
	ATTRIBUTE_ACCESSORS_BASIC(UItemAttributes, Mass)
	
	UPROPERTY(BlueprintReadOnly, Category = "Stability")
	FGameplayAttributeData Stability;
	ATTRIBUTE_ACCESSORS_BASIC(UItemAttributes, Stability)
	
	UPROPERTY(BlueprintReadOnly, Category = "Heat Ablation")
	FGameplayAttributeData HeatAblation;
	ATTRIBUTE_ACCESSORS_BASIC(UItemAttributes, HeatAblation)
	
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData Heat;
	ATTRIBUTE_ACCESSORS_BASIC(UItemAttributes, Heat)
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData MaxHeat;
	ATTRIBUTE_ACCESSORS_BASIC(UItemAttributes, MaxHeat)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
