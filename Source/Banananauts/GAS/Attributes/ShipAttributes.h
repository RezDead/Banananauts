// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ShipAttributes.generated.h"

/**
 * Attributes and handling of them for the ship as a whole.
 * 
 * Last Edited: 9/17/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UShipAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Mass")
	FGameplayAttributeData Mass;
	ATTRIBUTE_ACCESSORS_BASIC(UShipAttributes, Mass);
	UPROPERTY(BlueprintReadOnly, Category = "Mass")
	FGameplayAttributeData MaxMass;
	ATTRIBUTE_ACCESSORS_BASIC(UShipAttributes, MaxMass);
	
	UPROPERTY(BlueprintReadOnly, Category = "Fuel")
	FGameplayAttributeData Fuel;
	ATTRIBUTE_ACCESSORS_BASIC(UShipAttributes, Fuel);
	UPROPERTY(BlueprintReadOnly, Category = "Fuel")
	FGameplayAttributeData MaxFuel;
	ATTRIBUTE_ACCESSORS_BASIC(UShipAttributes, MaxFuel);
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
