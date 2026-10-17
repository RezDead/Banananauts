// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManagerSubsystem.generated.h"

/**
 * Manages level loading for the entire game.
 * 
 * Last Edited:10/17/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API ULevelManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void StartPreloadingLevel(TSoftObjectPtr<UWorld> Level);
	void LoadLevel(TSoftObjectPtr<UWorld> Level);

protected:
	TSoftObjectPtr<UWorld> CurrentTargetLevel;
	TSharedPtr<FStreamableHandle> LoadingHandle;
};
