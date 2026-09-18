// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipManager.h"

#include "Banananauts/Utilities/ShipStatsUtility.h"

AShipManager::AShipManager()
{
	TickRate = 5.0f;
	BaseHeatGain = 1.0f;
	Nose = CreateDefaultSubobject<AShipSegmentManager>(TEXT("NoseSegment"));
	Body = CreateDefaultSubobject<AShipSegmentManager>(TEXT("BodySegment"));
	Tail = CreateDefaultSubobject<AShipSegmentManager>(TEXT("TailSegment"));
	
	Segments.Add(Nose); Segments.Add(Body); Segments.Add(Tail);
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
	return UShipStatsUtility::GetShipMass(Segments);
}

/**
 * Calculates the maximum mass of the ship.
 * 
 * @return The maximum mass of the ship.
 */
float AShipManager::GetMaxShipMass()
{
	return UShipStatsUtility::GetMaxShipMass(Segments);
}

/**
 * Calculates the number of bananas on the ship.
 * 
 * @return The number of bananas on the ship.
 */
int AShipManager::GetBananaCount()
{
	return UShipStatsUtility::GetBananaCount(Segments);
}

/**
 * Calculates the maximum number of bananas the ship can hold.
 * 
 * @return The maximum number of bananas the ship can hold.
 */
int AShipManager::GetBananaCapacity()
{
	return UShipStatsUtility::GetBananaCapacity(Segments);
}

/**
 * Uses the specified number of bananas from the ship.
 * 
 * @param Amount The number of bananas to use. Must be greater than 0.
 * @return True if the operation was successful, false otherwise.
 */
bool AShipManager::UseBananas(int Amount)
{
	return UShipStatsUtility::UseBananas(Amount, Segments);
}

/**
 * Adds the specified number of bananas to the ship.
 * 
 * @param Amount The number of bananas to add. Must be greater than 0.
 * @return True if any bananas were added, false if already at capacity or invalid input.
 */
bool AShipManager::AddBananas(int Amount)
{
	return UShipStatsUtility::AddBananas(Amount, Segments);
}

void AShipManager::ModifyFuel(float Delta)
{
	if (Fuel + Delta > MaxFuel)
	{
		Fuel = MaxFuel;
	}
	else if (Fuel + Delta < 0)
	{
		if (Fuel != 0)
		{
			OnFuelEmpty.Broadcast();
		}
		Fuel = 0;
	}
	else
	{
		Fuel += Delta;
	}
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
	float NoseHeatMult = UShipStatsUtility::CalculateHeatMult(Nose);
	Nose->ModifyHeat(BaseHeatGain * NoseHeatMult);
	
	float BodyHeatMult = UShipStatsUtility::CalculateHeatMult(Body);
	Body->ModifyHeat(BaseHeatGain * BodyHeatMult);
	
	float TailHeatMult = UShipStatsUtility::CalculateHeatMult(Tail);
	Tail->ModifyHeat(BaseHeatGain * TailHeatMult);
}
