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

/**
 * Initiates the flight of the ship and all in-flight systems.
 */
void AShipManager::InitiateFlight()
{
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &AShipManager::TickSystems, TickRate, true);
}

/**
 * Calculates the mass of the ship.
 * 
 * @return The mass of the ship.
 */
float AShipManager::GetShipMass()
{
	return Nose->GetMass() + Body->GetMass() + Tail->GetMass();
}

/**
 * Calculates the maximum mass of the ship.
 * 
 * @return The maximum mass of the ship.
 */
float AShipManager::GetMaxShipMass()
{
	return Nose->GetMaxMass() + Body->GetMaxMass() + Tail->GetMaxMass();
}

/**
 * Calculates the number of bananas on the ship.
 * 
 * @return The number of bananas on the ship.
 */
int AShipManager::GetBananaCount()
{
	return Nose->GetBananas() + Body->GetBananas() + Tail->GetBananas();
}

/**
 * Calculates the maximum number of bananas the ship can hold.
 * 
 * @return The maximum number of bananas the ship can hold.
 */
int AShipManager::GetBananaCapacity()
{
	return Nose->GetMaxBananas() + Body->GetMaxBananas() + Tail->GetMaxBananas();
}

/**
 * Uses the specified number of bananas from the ship.
 * 
 * @param Amount The number of bananas to use. Must be greater than 0.
 * @return True if the operation was successful, false otherwise.
 */
bool AShipManager::UseBananas(int Amount)
{
	if (Amount <= 0) {return false;}
	if (GetBananaCount() < Amount) {return false;}
	
	Amount = RemoveBananaHelper(Amount, Body);
	if (Amount <= 0) {return true;}
	Amount = RemoveBananaHelper(Amount, Nose);
	if (Amount <= 0) {return true;}
	Amount = RemoveBananaHelper(Amount, Tail);
	if (Amount <= 0) {return true;}
	
	//Error occurred, check logic
	return false;
}

/**
 * Removes the specified number of bananas from the given segment.
 * 
 * @param Amount The number of bananas to remove.
 * @param Segment The segment from which to remove bananas.
 * @return The remaining number of bananas to remove.
 */
int AShipManager::RemoveBananaHelper(int Amount, UShipSegmentManager* Segment)
{
	int Num = Segment->GetBananas();
	
	if (Num < Amount)
	{
		Segment->ModifyBananas(-Num);
		return Amount - Num;
	}

	Segment->ModifyBananas(-Amount);
	return 0;
}

/**
 * Adds the specified number of bananas to the ship.
 * 
 * @param Amount The number of bananas to add. Must be greater than 0.
 * @return True if any bananas were added, false if already at capacity or invalid input.
 */
bool AShipManager::AddBananas(int Amount)
{
	if (Amount <= 0) {return false;}
	if (GetBananaCount() == GetBananaCapacity()){return false;}
	
	Amount = AddBananaHelper(Amount, Body);
	if (Amount <= 0) {return true;}
	Amount = AddBananaHelper(Amount, Nose);
	if (Amount <= 0) {return true;}
	AddBananaHelper(Amount, Tail);
	
	return true;
}

/**
 * Adds the specified number of bananas to the given segment.
 * 
 * @param Amount The number of bananas to add.
 * @param Segment The segment to add bananas to.
 * @return The remaining number of bananas to add.
 */
int AShipManager::AddBananaHelper(int Amount, UShipSegmentManager* Segment)
{
	int Num = Segment->GetMaxBananas() - Segment->GetBananas();
	
	if (Num < Amount)
	{
		Segment->ModifyBananas(Num);
		return Amount - Num;
	}
	
	Segment->ModifyBananas(Amount);
	return 0;
}

/**
 * Ticks all systems on the ship.
 */
void AShipManager::TickSystems()
{
	UpdateHeat();
}

/**
 * Updates the heat of all segments on the ship.
 */
void AShipManager::UpdateHeat() const
{
	float NoseHeatMult = CalculateHeatMult(Nose);
	Nose->ModifyHeat(BaseHeatGain * NoseHeatMult);
	
	float BodyHeatMult = CalculateHeatMult(Body);
	Body->ModifyHeat(BaseHeatGain * BodyHeatMult);
	
	float TailHeatMult = CalculateHeatMult(Tail);
	Tail->ModifyHeat(BaseHeatGain * TailHeatMult);
}

/**
 * Calculates the heat multiplier for a given ship segment.
 * 
 * @param Segment The ship segment to calculate the heat multiplier for.
 * @return The heat multiplier for the given ship segment.
 */
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
