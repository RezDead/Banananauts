// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

//Used to identify general item information

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TRB_ItemInformation.generated.h"

USTRUCT(BlueprintType)
struct FTRB_ItemInformation : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Item ID")
	FName ItemID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display Name")
	FName DisplayName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Model")
	FString ItemModel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Does Highlight?")
	bool DoesHighlight;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Highlight Color")
	FLinearColor HighlightColor;
};