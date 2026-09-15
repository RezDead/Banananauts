// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipManager.h"

AShipManager::AShipManager()
{
	TickRate = 5.0f;
	BaseHeatGain = 1.0f;
	Nose = CreateDefaultSubobject<UShipSegmentManager>(TEXT("NoseSegment"));
	Body = CreateDefaultSubobject<UShipSegmentManager>(TEXT("BodySegment"));
	Tail = CreateDefaultSubobject<UShipSegmentManager>(TEXT("TailSegment"));
}

void AShipManager::BeginPlay()
{
	Super::BeginPlay();
	InitiateFlight();
}

void AShipManager::InitiateFlight()
{
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &AShipManager::TickSystems, TickRate, true);
}

void AShipManager::TickSystems()
{
	UpdateHeat();
}

void AShipManager::UpdateHeat() const
{
	float NoseHeatMult = CalculateHeatMult(Nose);
	Nose->ModifyHeat(BaseHeatGain * NoseHeatMult);
	
	float BodyHeatMult = CalculateHeatMult(Body);
	Body->ModifyHeat(BaseHeatGain * BodyHeatMult);
	
	float TailHeatMult = CalculateHeatMult(Tail);
	Tail->ModifyHeat(BaseHeatGain * TailHeatMult);
}

float AShipManager::CalculateHeatMult(UShipSegmentManager* Segment)
{
	if (Segment->GetMaxHeatAblation() == 0.0f) return .5f;

	const float HeatAblationPercent = Segment->GetHeatAblation() / Segment->GetMaxHeatAblation();
	
	//If less than 50% heat ablation
	if (HeatAblationPercent <= 0.5f)
	{
		// Mult by two to accomodate for lerp range
		return FMath::Lerp(2.0f, 1.0f, HeatAblationPercent * 2.0f);
	}
	
	// -0.5f because we want to remove the 0-50% range and just calculate the 50-100% range, *2 to compensate for loss
	return FMath::Lerp(1.0f, 0.5f, (HeatAblationPercent - 0.5f) * 2.0f);
	
}