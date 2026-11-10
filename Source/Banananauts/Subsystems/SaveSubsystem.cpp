#include "SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

USaveSubsystem::USaveSubsystem()
{
	bLoadOnStart = true;
}

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	SaveData = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot0"), 0));
	
	if (!SaveData)
	{
		UE_LOG(LogTemp, Display, TEXT("No default save, creating new."));
		SaveData = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveData, TEXT("Slot0"), 0);
	}
	
	if (bLoadOnStart)
	{
		FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateWeakLambda(this, [this](float)
			{
				this->LoadGame(Start); 
				return false;	// one-shot
			}),
		0.0f
		);
	}
}

void USaveSubsystem::UpdateToBeSavedData(const ESaveType SaveEvent)
{
	const FActorSetWrapper* Saveables = RegisteredSaveables.Find(SaveEvent);
	
	if (!Saveables) return;
	
	for (const auto& Saveable : *Saveables)
	{
		ISaveable::Execute_OnSaveGame(Saveable, SaveData, SaveEvent);
	}
}

void USaveSubsystem::CommitSave()
{
	if (!IsValid(SaveData))
	{
		SaveData = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
		if (!IsValid(SaveData))
		{
			UE_LOG(LogTemp, Error, TEXT("SaveGame failed: could not create SaveData."));
			return;
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveData, TEXT("Slot0"), 0);
	
	UE_LOG(LogTemp, Display, TEXT("Game Saved"));
}


void USaveSubsystem::UpdateAndCommitSave(const ESaveType SaveEvent)
{
	if (!IsValid(SaveData))
	{
		SaveData = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
		if (!IsValid(SaveData))
		{
			UE_LOG(LogTemp, Error, TEXT("SaveGame failed: could not create SaveData."));
			return;
		}
	}
	
	UpdateToBeSavedData(SaveEvent);
	
	UGameplayStatics::SaveGameToSlot(SaveData, TEXT("Slot0"), 0);
	
	UE_LOG(LogTemp, Display, TEXT("Game Saved"));
}

/**
 * Loads the game state from the save data object.
 * 
 * If the save data object is not valid, logs an error and returns.
 * 
 * Otherwise, iterates through all registered saveable objects and invokes their `OnLoadGame` method, passing the save data object.
 */
void USaveSubsystem::LoadGame(const ESaveType LoadEvent)
{
	SaveData = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot0"), 0));
	
	if (!SaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Load Failed"));
		return;
	}

	if (const FActorSetWrapper* Saveables = RegisteredSaveables.Find(LoadEvent))
	{
		for (const auto& Saveable : *Saveables)
		{
			ISaveable::Execute_OnLoadGame(Saveable, SaveData, LoadEvent);
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT("Game Loaded"));
}

/**
 * Registers a saveable object with the save subsystem.
 */
void USaveSubsystem::RegisterSaveable(AActor* Saveable, const ESaveType SaveEvent)
{
	if (!IsValid(Saveable))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Saveable passed to RegisterSaveable"));
		return;
	}
	
	if (RegisteredSaveables.Contains(SaveEvent))
	{
		RegisteredSaveables[SaveEvent].Actors.Add(Saveable);
	}
	else
	{
		FActorSetWrapper NewSet;
		NewSet.Actors.Add(Saveable);
		RegisteredSaveables.Add(SaveEvent, NewSet);
	}
}

/**
 * Unregisters a saveable object from the save subsystem.
 */
void USaveSubsystem::UnregisterSaveable(AActor* Saveable, const ESaveType SaveEvent)
{
	if (!Saveable) return;
	
	if (RegisteredSaveables.Contains(SaveEvent))
	{
		RegisteredSaveables[SaveEvent].Actors.Remove(Saveable);
	}
}

void USaveSubsystem::ClearSaveData()
{
	SaveData = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
	
	UGameplayStatics::SaveGameToSlot(SaveData, TEXT("Slot0"), 0);
}
