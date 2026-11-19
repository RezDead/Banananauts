#pragma once

#include "CoreMinimal.h"
#include "FailState.generated.h"

UENUM(BlueprintType)
enum class EFailState : uint8
{
	Fall		UMETA(DisplayName = "Fall"),
	Explosion	UMETA(DisplayName = "Explosion"),
	NoFuel		UMETA(DisplayName = "NoFuel"),
};
