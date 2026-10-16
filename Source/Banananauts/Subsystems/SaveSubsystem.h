// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/Save/SaveData.h"
#include "Banananauts/Interfaces/Saveable.h"
#include "Subsystems/Subsystem.h"
#include "SaveSubsystem.generated.h"

/**
 * Manages the save data for the game.
 * 
 * Last Edited: 10/16/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USaveSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	
	UFUNCTION(BlueprintCallable)
	void RegisterSaveable(TScriptInterface<ISaveable> Saveable);
	
	UFUNCTION(BlueprintCallable)
	void UnregisterSaveable(TScriptInterface<ISaveable> Saveable);
	
protected:
	UPROPERTY()
	USaveData* SaveData;
	
	TArray<TWeakInterfacePtr<ISaveable>> RegisteredSaveables;
	
};
