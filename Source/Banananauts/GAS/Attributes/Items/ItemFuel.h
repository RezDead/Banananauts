// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ItemFuel.generated.h"

/**
 * Item fuel attribute set
 * 
 * Last Edited: 11/2/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UItemFuel : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Fuel")
	FGameplayAttributeData Fuel;
	ATTRIBUTE_ACCESSORS_BASIC(UItemFuel, Fuel)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
