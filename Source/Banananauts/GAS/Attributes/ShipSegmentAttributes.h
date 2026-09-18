// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ShipSegmentAttributes.generated.h"

/**
 * Attributes and handling of them for ship segments.
 * 
 * Last Edited: 9/17/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UShipSegmentAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UShipSegmentAttributes();
	
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData Heat;
	ATTRIBUTE_ACCESSORS_BASIC(UShipSegmentAttributes, Heat);
	UPROPERTY(BlueprintReadOnly, Category = "Heat")
	FGameplayAttributeData MaxHeat;
	ATTRIBUTE_ACCESSORS_BASIC(UShipSegmentAttributes, MaxHeat);
	
	UPROPERTY(BlueprintReadOnly, Category = "Heat Ablation")
	FGameplayAttributeData HeatAblation;
	ATTRIBUTE_ACCESSORS_BASIC(UShipSegmentAttributes, HeatAblation);
	UPROPERTY(BlueprintReadOnly, Category = "Heat Ablation")
	FGameplayAttributeData MaxHeatAblation;
	ATTRIBUTE_ACCESSORS_BASIC(UShipSegmentAttributes, MaxHeatAblation);
	
	UPROPERTY(BlueprintReadOnly, Category = "Bananas")
	FGameplayAttributeData Bananas;
	ATTRIBUTE_ACCESSORS_BASIC(UShipSegmentAttributes, Bananas);
	UPROPERTY(BlueprintReadOnly, Category = "Bananas")
	FGameplayAttributeData MaxBananas;
	ATTRIBUTE_ACCESSORS_BASIC(UShipSegmentAttributes, MaxBananas);
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
