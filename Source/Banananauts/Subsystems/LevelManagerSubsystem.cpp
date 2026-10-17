// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "LevelManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"

void ULevelManagerSubsystem::StartPreloadingLevel(TSoftObjectPtr<UWorld> Level)
{	
	if (!Level.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManagerSubsystem::StartPreloadingLevel: Invalid Level Passed"));
		return;
	} 
	
	if (LoadingHandle.IsValid() && LoadingHandle->IsActive())
	{
		LoadingHandle->ReleaseHandle();
	}
		
	CurrentTargetLevel = Level;
	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	FSoftObjectPath LevelPath = Level.ToSoftObjectPath();
		
	UE_LOG(LogTemp, Log, TEXT("LevelManagerSubsystem::StartPreloadingLevel - Starting lazy load of level: %s"), *LevelPath.ToString());
		
	LoadingHandle = StreamableManager.RequestAsyncLoad(LevelPath);
}

void ULevelManagerSubsystem::LoadLevel(const TSoftObjectPtr<UWorld> Level)
{
	if (!Level.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManagerSubsystem::LoadLevel: Invalid Level Passed"));
		return;
	} 
	
	UE_LOG(LogTemp, Log, TEXT("LevelManagerSubsystem::LoadLevel - Loading level: %s"), *Level.ToSoftObjectPath().ToString());
	
	if (CurrentTargetLevel != Level)
	{
		if (LoadingHandle.IsValid())
		{
			LoadingHandle->ReleaseHandle();
		}
		CurrentTargetLevel = Level;
		
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		LoadingHandle = StreamableManager.RequestSyncLoad(CurrentTargetLevel.ToSoftObjectPath());
	}
	else if (LoadingHandle.IsValid() && !LoadingHandle->HasLoadCompleted())
	{
		LoadingHandle->WaitUntilComplete(); // block the thread until it finishes
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),CurrentTargetLevel);
	
	if (LoadingHandle.IsValid())
	{
		LoadingHandle->ReleaseHandle();
	}
}
