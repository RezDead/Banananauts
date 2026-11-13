#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/Save/SaveData.h"
#include "Banananauts/Enums/SaveType.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSaveGame(USaveData* SaveGameObject, ESaveType SaveEvent);
	
	// Loads save data from the save game.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLoadGame(USaveData* SaveGameObject, ESaveType LoadEvent);

protected:
	// Reminder to register this object with the save subsystem.
	// Register should happen on begin play.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RegisterToSaveSubsystem();
	
	// Reminder to unregister this object with the save subsystem.
	// Unregister should happen on end play.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnregisterFromSaveSubsystem();
};
