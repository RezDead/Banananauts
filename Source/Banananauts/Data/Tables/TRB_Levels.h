// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

//Used to identify general item information

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TRB_Levels.generated.h"

USTRUCT(BlueprintType)
struct FTRB_Levels : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Level Reference")
	TSoftObjectPtr<UWorld> Level;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Level Name")
	FName LevelName;
	
};