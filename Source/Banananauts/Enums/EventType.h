#pragma once

#include "CoreMinimal.h"
#include "EventType.generated.h"

UENUM(BlueprintType)
enum class EEventType : uint8
{
	All      UMETA(DisplayName = "All"),
	PopOff	   UMETA(DisplayName = "Pop-Off"),
};
