#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

/*
 * Enum used to categorize items.
 */
UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	BoltOn      UMETA(DisplayName = "Bolt-On"),
	Wall		UMETA(DisplayName = "Overheating"),
	Activated   UMETA(DisplayName = "Burning"),
	Fuel		UMETA(DisplayName = "Explosive"),
	Catalyst    UMETA(DisplayName = "Catalyst"),
	Engine		UMETA(DisplayName = "Engine"),
	Thruster    UMETA(DisplayName = "Thruster"),
};
