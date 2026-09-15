// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Managers/ShipSegmentManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShipStatsUtility.generated.h"

/**
 * Utility class for calculating ship stats/data and certain functions.
 * 
 * Last Edited: 9/15/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UShipStatsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static float GetShipMass(const TArray<UShipSegmentManager*>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static float GetMaxShipMass(const TArray<UShipSegmentManager*>& Segments);
	
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static int GetBananaCount(const TArray<UShipSegmentManager*>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static float GetBananaCapacity(const TArray<UShipSegmentManager*>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static bool UseBananas(int& Amount, const TArray<UShipSegmentManager*>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static bool AddBananas(int& Amount, const TArray<UShipSegmentManager*>& Segments);
	
	static float CalculateHeatMult(const UShipSegmentManager* Segment);
	
private:
	static bool RemoveBananaHelper(int& Amount, UShipSegmentManager* Segment);
	static bool AddBananaHelper(int& Amount, UShipSegmentManager* Segment);
	
};
