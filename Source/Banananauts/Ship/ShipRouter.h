// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "ShipRouter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSuccessDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFailureDelegate);

UENUM(BlueprintType)
enum class EFlightType : uint8
{
	Launch,
	Flight
};

/**
 * Handles the route, flight, and launch of the ship.
 *
 * Last Edited:10/17/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API AShipRouter : public AActor
{
	GENERATED_BODY()

public:
	AShipRouter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;
	
	UFUNCTION(BlueprintCallable)
	void AddShipProgress(const float DeltaProgress);

	//Delegates used to signal others that success/fail is coming
	UPROPERTY(BlueprintAssignable)
	FSuccessDelegate SuccessDelegate;
	UPROPERTY(BlueprintAssignable)
	FFailureDelegate FailureDelegate;
	
	void Success();
	void Failure();
	
protected:
	virtual void BeginPlay() override;
	
	FTransform GetTransformAtPercent(float Alpha) const;
	
	UPROPERTY(BlueprintReadOnly)
	float ShipProgress = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	EFlightType RouteType = EFlightType::Launch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSoftObjectPtr<AActor> ShipRef;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	TSoftObjectPtr<UWorld> FlightLevel = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	UDataTable* LevelsDT;
	
	UPROPERTY()
	AActor* Ship;
	
	float SplineLength;
	
private:
	void BeginRoute() const;
	
	TSoftObjectPtr<UWorld> GetRandomLevel() const;
};
