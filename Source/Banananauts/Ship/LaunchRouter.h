// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShipRouter.h"
#include "LaunchRouter.generated.h"

/**
 * Handles the route and launch of the ship.
 *
 * Last Edited:10/18/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API ALaunchRouter : public AShipRouter
{
	GENERATED_BODY()

public:
	ALaunchRouter();
	
	virtual void Success() override;
	virtual void Failure() override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TSoftObjectPtr<UWorld> FlightLevel = nullptr;
	
	virtual void BeginRoute() const override;
};
