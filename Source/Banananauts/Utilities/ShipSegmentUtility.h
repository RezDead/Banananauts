// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShipSegmentUtility.generated.h"

/**
 * Utility functions for ship segments to help handle data.
 * 
 * Last Edited: 9/28/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UShipSegmentUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static void AddItemMassToShip(UAbilitySystemComponent* ItemASC, UAbilitySystemComponent* ShipASC);
	
};
