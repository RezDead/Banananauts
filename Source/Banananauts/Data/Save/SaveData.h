/**
 * @file SaveData.h
 * @author Julian Kroeger-Miller
 * @brief Declares the save game object used to persist player save data, including stored weapon inventory.
 */

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/ItemSaveInfo.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

/**
 * Save game object that stores persistent data for Banananauts.
 * 
 * Last Edited: 10/16/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API USaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	TArray<FItemSaveInfo> ShipItems;
};
