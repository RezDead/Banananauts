// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FuelComposition.generated.h"

USTRUCT(BlueprintType)
struct BANANANAUTS_API FFuelComposition
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Thrust = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Volatility = 0.0f;
};
