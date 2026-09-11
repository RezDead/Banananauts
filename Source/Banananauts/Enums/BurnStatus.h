#pragma once

#include "CoreMinimal.h"
#include "BurnStatus.generated.h"

UENUM(BlueprintType)
enum class EBurnStatus : uint8
{
	Optimal      UMETA(DisplayName = "Optimal"),
	Overheating  UMETA(DisplayName = "Overheating"),
	Burning      UMETA(DisplayName = "Burning"),
	Explosive    UMETA(DisplayName = "Explosive"),
};
