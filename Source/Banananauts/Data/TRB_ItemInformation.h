// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

//Used to identify general item information

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Banananauts/Enums/ItemTypes.h"
#include "Engine/DataTable.h"
#include "TRB_ItemInformation.generated.h"

USTRUCT(BlueprintType)
struct FTRB_ItemInformation : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display Name")
	FName DisplayName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Model")
	UStaticMesh* Model;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Type")
	EItemTypes Type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tags")
	FGameplayTagContainer Tags;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mass")
	float Mass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Heat Ablation")
	float HeatAblation;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Min Heat")
	float MinHeat;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Max Heat")
	float MaxHeat;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stability")
	float Stability;
};