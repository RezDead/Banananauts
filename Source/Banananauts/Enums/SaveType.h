#pragma once

#include "CoreMinimal.h"
#include "Savetype.generated.h"

UENUM(BlueprintType)
enum class ESaveType : uint8
{
	Start	UMETA(DisplayName = "Start"),
	Flight	UMETA(DisplayName = "Flight"),
	Fail	UMETA(DisplayName = "Fail"),
};
