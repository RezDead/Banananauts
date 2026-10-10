#pragma once

#include "CoreMinimal.h"
#include "ShipSection.generated.h"

UENUM(BlueprintType)
enum class EShipSection : uint8
{
	Whole      UMETA(DisplayName = "Whole"),
	Nose	   UMETA(DisplayName = "Nose"),
	Body	   UMETA(DisplayName = "Body"),
	Tail	   UMETA(DisplayName = "Tail"),
};
