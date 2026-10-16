#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/Save/SaveData.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USaveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can be saved. The save system calls functions provided by this interface.
 * 
 * Last Edited: 10/16/2026
 * Author: Julian Kroeger-Miller
 */
class BANANANAUTS_API ISaveable
{
	GENERATED_BODY()

public:
	// Adds save data to the save game.
	virtual void OnSaveGame(USaveData* SaveGameObject) = 0;
	
	// Loads save data from the save game.
	virtual void OnLoadGame(USaveData* SaveGameObject) = 0;
};
