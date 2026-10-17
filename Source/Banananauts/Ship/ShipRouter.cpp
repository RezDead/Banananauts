// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipRouter.h"
#include "Banananauts/Interfaces/ShipCommunication.h"

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

void AShipRouter::AddShipProgress(const float DeltaProgress)
{
	ShipProgress += DeltaProgress;
	
	if (ShipProgress >= 1)
	{
		const FTransform NewTransform = GetTransformAtPercent(1.0f);
		Ship->SetActorLocationAndRotation(NewTransform.GetLocation(), NewTransform.GetRotation());
			
		SuccessDelegate.Broadcast();
			
		return;
	}
		
	//Update position
	const FTransform NewTransform = GetTransformAtPercent(ShipProgress);
	Ship->SetActorLocationAndRotation(NewTransform.GetLocation(), NewTransform.GetRotation());
}

/**
 * Begins flight route based on what route type is specified
 */
void AShipRouter::BeginRoute() const
{
	switch (RouteType) {
	case EFlightType::Launch:
		break;
	case EFlightType::Flight:
		const IShipCommunication* Communication = Cast<IShipCommunication>(Ship);
		if (Communication)
		{
			Communication->Execute_BeginFlightSequence(Ship);
		}
		break;
	}
}

// Called when the game starts or when spawned
void AShipRouter::BeginPlay()
{
	Super::BeginPlay();
	
	if (Spline)
	{
		SplineLength = Spline->GetSplineLength();
	}
	
	Ship = ShipRef.Get();
	
	if (IsValid(Ship))
	{
		BeginRoute();
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("AShipRouter::BeginPlay(): Ship reference is nullptr, make sure this is set in editor"));
	}
}

