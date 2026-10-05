// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipRouter.h"

#include "Kismet/KismetMathLibrary.h"


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

// Called when the game starts or when spawned
void AShipRouter::BeginPlay()
{
	Super::BeginPlay();
	
	if (Spline)
	{
		SplineLength = Spline->GetSplineLength();
	}
}

