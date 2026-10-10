// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Enums/ShipSection.h"
#include "Banananauts/Managers/ShipSegmentManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShipStatsUtility.generated.h"

/**
 * Utility class for calculating ship stats/data and certain functions.
 * 
 * Last Edited: 9/21/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UShipStatsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static int GetBananaCount(const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static float GetBananaCapacity(const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static bool UseBananas(int& Amount, const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments);
	UFUNCTION(BlueprintCallable, Category = "Ship|Stats")
	static bool AddBananas(int& Amount, const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments);
	
	static void PopOffEvent(const EShipSection Segment, const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments);
	
	static 	TSoftObjectPtr<UWorld> GetRandomLevel(const UDataTable* LevelsDT);
	
	static float CalculateHeatMult(const AShipSegmentManager* Segment);
	
	static float CalculateShipProgressAdditive(const float& Mass, const float& Thrust, const float& MinTime,
	                                           const float& LinearGrowthRate, const float& DeltaSeconds);
	
private:
	static bool RemoveBananaHelper(int& Amount, AShipSegmentManager* Segment);
	static bool AddBananaHelper(int& Amount, AShipSegmentManager* Segment);
	
	static void PopOffItem(TArray<AActor*>& AttachedItems);
};
