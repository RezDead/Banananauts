#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/Save/SaveData.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class USaveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can be saved. The save system calls functions provided by this interface.
 * 
 * Last Edited: 10/17/2026
 * Author: Julian Kroeger-Miller
 */
class BANANANAUTS_API ISaveable
{
	GENERATED_BODY()

public:
	// Adds save data to the save game.
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveGame(USaveData* SaveGameObject);
	
	// Loads save data from the save game.
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadGame(USaveData* SaveGameObject);

	UFUNCTION(BlueprintNativeEvent)
	void InitSaveSubsystem();
};
