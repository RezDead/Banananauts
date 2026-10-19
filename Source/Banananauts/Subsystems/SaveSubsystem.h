#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/Save/SaveData.h"
#include "Banananauts/Interfaces/Saveable.h"
#include "Subsystems/Subsystem.h"
#include "SaveSubsystem.generated.h"

/**
 * Manages the save data for the game.
 * 
 * Last Edited: 10/17/2026
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
	void RegisterSaveable(AActor* Saveable);
	
	UFUNCTION(BlueprintCallable)
	void UnregisterSaveable(AActor* Saveable);
	
	UFUNCTION(BlueprintCallable)
	void ClearSaveData();
	
protected:
	UPROPERTY()
	USaveData* SaveData;
	
	UPROPERTY()
	TSet<AActor*> RegisteredSaveables;
	
private:
	bool bLoadOnStart;
	
};
