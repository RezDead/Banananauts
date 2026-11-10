#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Data/Save/SaveData.h"
#include "Banananauts/Enums/SaveType.h"
#include "Banananauts/Interfaces/Saveable.h"
#include "Subsystems/Subsystem.h"
#include "SaveSubsystem.generated.h"

USTRUCT()
struct FActorSetWrapper
{
	GENERATED_BODY()
	
	UPROPERTY()
	TSet<AActor*> Actors;
};

/**
 * Manages the save data for the game.
 * 
 * Last Edited: 11/9/2026
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
	void UpdateToBeSavedData(ESaveType SaveEvent);
	
	UFUNCTION(BlueprintCallable)
	void CommitSave();
	
	UFUNCTION(BlueprintCallable)
	void UpdateAndCommitSave(ESaveType SaveEvent);
	
	UFUNCTION(BlueprintCallable)
	void LoadGame(ESaveType LoadEvent);
	
	UFUNCTION(BlueprintCallable)
	void RegisterSaveable(AActor* Saveable, ESaveType SaveEvent);
	
	UFUNCTION(BlueprintCallable)
	void UnregisterSaveable(AActor* Saveable, ESaveType SaveEvent);
	
	UFUNCTION(BlueprintCallable)
	void ClearSaveData();
	
protected:
	UPROPERTY()
	USaveData* SaveData;
	
	UPROPERTY()
	TMap<ESaveType, FActorSetWrapper> RegisteredSaveables;
	
	void PreSaveUpdate(ESaveType SaveEvent) const;
	
private:
	bool bLoadOnStart;
	
};
