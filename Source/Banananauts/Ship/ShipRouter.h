// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

USTRUCT(BlueprintType)
struct FSetRouteEvent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float MinPercentTrigger = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float MaxPercentTrigger = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag EventAction;
};

USTRUCT(BlueprintType)
struct FRouteEvent
{
	GENERATED_BODY()
	
	UPROPERTY()
	float PercentTrigger = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag EventAction;
	
	UPROPERTY()
	bool bTriggered = false;
};

/**
 * Handles a ship route and events that occur along the way.
 *
 * Last Edited:10/18/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS(Abstract)
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
	
	virtual void Success();
	virtual void Failure();
	
protected:
	virtual void BeginPlay() override;
	
	FTransform GetTransformAtPercent(float Alpha) const;
	
	UPROPERTY(BlueprintReadOnly)
	float ShipProgress = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TArray<FSetRouteEvent> SetRouteEvents;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	EFlightType RouteType = EFlightType::Launch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSoftObjectPtr<AActor> ShipRef;
	
	UPROPERTY()
	AActor* Ship;
	
	float SplineLength;
	
	virtual void BeginRoute() const;

	virtual void TriggerEvent(const FGameplayTag EventAction);
	
	UPROPERTY()
	TArray<FRouteEvent> RouteEvents;
	UPROPERTY()
	int CurrentEventIndex = 0;
	
	UFUNCTION(CallInEditor, Category = "Default")
	void ShowEventRanges();
	
private:
	void InitRouteEvents();
	void CheckIfRouteEventTriggered();
};
