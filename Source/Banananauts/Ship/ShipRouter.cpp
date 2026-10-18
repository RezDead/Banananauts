// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipRouter.h"

// Sets default values
AShipRouter::AShipRouter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(Spline);
}

/**
 * Gets the transform along the spline at a certain percentage of its length.
 * 
 * @param Alpha Percent at which to get transform.
 * @return Transform along spline at the given percentage.
 */
FTransform AShipRouter::GetTransformAtPercent(const float Alpha) const
{
	const float Distance = FMath::Lerp(0.0f, SplineLength, Alpha);
	
	FTransform Ret;
	const FVector Location = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	Ret.SetLocation(Location);
	
	FQuat Rot = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).ToOrientationQuat();
	FQuat MeshOffset = FQuat(FRotator(-90.0f, 0.0f, 0.0f));
	
	Ret.SetRotation(Rot * MeshOffset);
	
	return Ret;
}

/**
 * Adds progress to the ship along the route
 * @param DeltaProgress Amount of progress to add
 */
void AShipRouter::AddShipProgress(const float DeltaProgress)
{
	ShipProgress += DeltaProgress;
	
	CheckIfRouteEventTriggered();
	
	if (ShipProgress >= 1)
	{
		const FTransform NewTransform = GetTransformAtPercent(1.0f);
		Ship->SetActorLocationAndRotation(NewTransform.GetLocation(), NewTransform.GetRotation());
			
		Success();
			
		return;
	}
		
	//Update position
	const FTransform NewTransform = GetTransformAtPercent(ShipProgress);
	Ship->SetActorLocationAndRotation(NewTransform.GetLocation(), NewTransform.GetRotation());
}

/**
 * --To be implemented in Children--
 *
 * Begins flight route.
 */
void AShipRouter::BeginRoute() const
{

}

/**
 * Debug Function
 *
 * Draws debug lines and spheres for route events.
 */
void AShipRouter::ShowEventRanges()
{
	for (const auto& SetEvent : SetRouteEvents)
	{
		FColor Color = FColor::MakeRandomColor();
		FVector Min = Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() * SetEvent.MinPercentTrigger, ESplineCoordinateSpace::World);
		FVector Max = Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() * SetEvent.MaxPercentTrigger, ESplineCoordinateSpace::World);
		DrawDebugSphere(GetWorld(), Min, 50, 4, Color, false, 10.0f, 0, 5);
		DrawDebugSphere(GetWorld(), Max, 50, 4, Color, false, 10.0f, 0, 5);
		DrawDebugLine(GetWorld(), Min, Max, Color, false, 10.0f, 0, 5);
	}
}

/**
 * Initializes the route events.
 */
void AShipRouter::InitRouteEvents()
{
	if (SetRouteEvents.IsEmpty()) {return;}
	
	for (const auto& SetEvent : SetRouteEvents)
	{
		FRouteEvent Event;
		Event.EventAction = SetEvent.EventAction;
		const float MinPercentTrigger = SetEvent.MinPercentTrigger <= SetEvent.MaxPercentTrigger ? SetEvent.MinPercentTrigger : SetEvent.MaxPercentTrigger;
		const float MaxPercentTrigger = SetEvent.MinPercentTrigger <= SetEvent.MaxPercentTrigger ? SetEvent.MaxPercentTrigger : SetEvent.MinPercentTrigger;
		Event.PercentTrigger = FMath::FRandRange(MinPercentTrigger, MaxPercentTrigger);
		Event.bTriggered = false;
		RouteEvents.Add(Event);
	}
	
	RouteEvents.Sort([](const FRouteEvent& A, const FRouteEvent& B) {return A.PercentTrigger < B.PercentTrigger;});
	for (int i = 0; i < RouteEvents.Num(); i++)
		UE_LOG(LogTemp, Log, TEXT("Route Event: %f"), RouteEvents[i].PercentTrigger);
	CurrentEventIndex = 0;
}

/**
 * Checks if a route event should trigger, if so trigger it.
 */
void AShipRouter::CheckIfRouteEventTriggered()
{
	if (CurrentEventIndex >= RouteEvents.Num()) {return;}
	
	if (RouteEvents[CurrentEventIndex].PercentTrigger <= ShipProgress && !RouteEvents[CurrentEventIndex].bTriggered)
	{
		RouteEvents[CurrentEventIndex].bTriggered = true;
		TriggerEvent(RouteEvents[CurrentEventIndex].EventAction);
		CurrentEventIndex++;
	}
}

/**
 * --To be implemented in Children--
 * 
 * What happens when a route event is triggered.
 * @param EventAction The action of the event.
 */
void AShipRouter::TriggerEvent(const FGameplayTag EventAction)
{
}

void AShipRouter::BeginPlay()
{
	Super::BeginPlay();
	
	if (Spline)
	{
		SplineLength = Spline->GetSplineLength();
	}
	
	InitRouteEvents();
	
	Ship = ShipRef.Get();
	
	if (IsValid(Ship))
	{
		BeginRoute();
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("AShipRouter::BeginPlay(): Ship reference is nullptr, make sure this is set in editor"));
	}
}

/**
 * --To be implemented in Children--
 *
 * What happens when the ship reaches the end of the route.
 */
void AShipRouter::Success()
{
	SuccessDelegate.Broadcast();
}

/**
 * --To be implemented in Children--
 *
 * What happens when the ship fails the route.
 */
void AShipRouter::Failure()
{
	FailureDelegate.Broadcast();
}
