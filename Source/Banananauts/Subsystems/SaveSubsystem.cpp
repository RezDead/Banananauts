#include "SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

USaveSubsystem::USaveSubsystem()
{
	
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
	
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateWeakLambda(this, [this](float)
		{
			this->LoadGame(); 
			return false;	// one-shot
		}),
	0.0f
	);
}

/**
 * Saves the current game state.
 *
 * The method iterates through all registered saveable objects and invokes their `OnSaveGame` method, passing the save data object.
 *
 * Logs an error if the save data object cannot be created and a success message upon a successful save.
 */
void USaveSubsystem::SaveGame()
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
	
	for (const auto& Saveable : RegisteredSaveables)
	{
		ISaveable* Interface = Saveable.Get();
		if (!Interface) continue;
		
		Interface->OnSaveGame(SaveData);
	}
	
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
void USaveSubsystem::LoadGame()
{
	SaveData = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot0"), 0));
	
	if (!SaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Load Failed"));
		return;
	}
	
	for (auto Saveable : RegisteredSaveables)
	{
		ISaveable* Interface = Saveable.Get();
		if (!Interface) continue;
		
		Interface->OnLoadGame(SaveData);
	}
	
	UE_LOG(LogTemp, Display, TEXT("Game Loaded"));
}

/**
 * Registers a saveable object with the save subsystem.
 */
void USaveSubsystem::RegisterSaveable(TScriptInterface<ISaveable> Saveable)
{
	if (!Saveable || !IsValid(Saveable.GetObject()))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Saveable passed to RegisterSaveable"));
		return;
	}

	RegisteredSaveables.Add(Saveable);
}

/**
 * Unregisters a saveable object from the save subsystem.
 */
void USaveSubsystem::UnregisterSaveable(TScriptInterface<ISaveable> Saveable)
{
	if (!Saveable || !IsValid(Saveable.GetObject())) return;
	
	RegisteredSaveables.Remove(Saveable);
}
