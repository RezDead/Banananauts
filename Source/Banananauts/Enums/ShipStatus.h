#pragma once

#include "CoreMinimal.h"
#include "ShipStatus.generated.h"

UENUM(BlueprintType)
enum class EShipStatus : uint8
{
	Clear      UMETA(DisplayName = "Clear"),
	Warning	   UMETA(DisplayName = "Warning"),
	Fail	   UMETA(DisplayName = "Fail"),
};
