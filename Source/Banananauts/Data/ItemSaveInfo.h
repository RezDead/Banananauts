#pragma once
#include "CoreMinimal.h"
#include "Banananauts/Enums/ShipSection.h"
#include "Banananauts/Items/ItemBase.h"
#include "ItemSaveInfo.generated.h"

/**
 * Struct for saving item data.
 * 
 * Last Edited: 10/16/2026
 * Author: Julian Kroeger-Miller
 */
USTRUCT(BlueprintType)
struct FItemSaveInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Type")
	TSoftClassPtr<AItemBase> ItemType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Data")
	EShipSection AttachedTo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Data")
	FTransform RelativeTransform;
	
};
