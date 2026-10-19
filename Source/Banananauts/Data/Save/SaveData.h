/**
 * @file SaveData.h
 * @author Julian Kroeger-Miller
 * @brief Declares the save game object used to persist player save data, including stored weapon inventory.
 */

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Structs//ItemSaveInfo.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

USTRUCT(Blueprintable)
struct FSectionSaveWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TArray<FItemSaveInfo> SectionItems;
};

/**
 * Save game object that stores persistent data for Banananauts.
 * 
 * Last Edited: 10/17/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API USaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	//These MUST be a UPROPERTY to save them
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TMap<EShipSection, FSectionSaveWrapper> ShipItems;
};
