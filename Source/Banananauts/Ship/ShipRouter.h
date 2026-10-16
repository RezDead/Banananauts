// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "ShipRouter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSuccessDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFailureDelegate);

enum EFlightType
{
	Launch,
	Flight
};

/**
 * Handles the route, flight, and launch of the ship.
 *
 * Last Edited:10/16/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API AShipRouter : public AActor
{
	GENERATED_BODY()

public:
	AShipRouter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ShipProgress = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;
	
	UFUNCTION(BlueprintCallable)
	void SetShip(AActor* NewShip) { Ship = NewShip;}
	
	UFUNCTION(BlueprintCallable)
	void AddShipProgress(const float DeltaProgress);
	
	UPROPERTY(BlueprintAssignable)
	FSuccessDelegate SuccessDelegate;
	UPROPERTY(BlueprintAssignable)
	FFailureDelegate FailureDelegate;
	
protected:
	virtual void BeginPlay() override;
	
	FTransform GetTransformAtPercent(float Alpha) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Ship;
	
	float SplineLength;
};
