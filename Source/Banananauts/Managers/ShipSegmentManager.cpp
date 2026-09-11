// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentManager.h"

/**
 * Modifies the current total heat of the ship part and updates burn status as needed.
 * 
 * @param Delta Value to modify the current total heat by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyTotalHeat(float Delta)
{
	TotalHeat += Delta;
	
	if (TotalHeat > MaxTotalHeat)
	{
		TotalHeat = MaxTotalHeat;
	}
	else if (TotalHeat < 0)
	{
		TotalHeat = 0;
	}
	
	if ( TotalHeat/MaxTotalHeat > .99 && BurnStatus != EBurnStatus::Explosive)
	{
		BurnStatus = EBurnStatus::Explosive;
		OnHeatStatusChanged.Broadcast(BurnStatus);
	}
	else if ( TotalHeat/MaxTotalHeat > .66 && BurnStatus != EBurnStatus::Burning)
	{
		BurnStatus = EBurnStatus::Burning;
		OnHeatStatusChanged.Broadcast(BurnStatus);
	}
	else if ( TotalHeat/MaxTotalHeat > .33 && BurnStatus != EBurnStatus::Overheating)
	{
		BurnStatus = EBurnStatus::Overheating;
		OnHeatStatusChanged.Broadcast(BurnStatus);
	}
	else if  (BurnStatus != EBurnStatus::Optimal)
	{
		BurnStatus = EBurnStatus::Optimal;
		OnHeatStatusChanged.Broadcast(BurnStatus);
	}
}

/**
 * Modifies the maximum total heat of the ship part.
 * 
 * @param Delta Value to modify the maximum total heat by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyMaxTotalHeat(float Delta)
{
	MaxTotalHeat += Delta;
}

void UShipSegmentManager::ModifyHeatAblation(float Delta)
{
	HeatAblation += Delta;
	
	if (HeatAblation > MaxHeatAblation)
	{
		HeatAblation = MaxHeatAblation;
	}
	else if (HeatAblation < 0)
	{
		HeatAblation = 0;
	}
}

/**
 * Modifies the mass of the ship part.
 * 
 * @param Delta Value to modify the mass by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyMass(float Delta)
{
	Mass += Delta;
	
	if (Mass > MaxMass)
	{
		Mass = MaxMass;
	}
	else if (Mass < 0)
	{
		Mass = 0;
	}
}

/**
 * Modifies the fuel of the ship part.
 * 
 * @param Delta Value to modify the fuel by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyFuel(float Delta)
{
	if (!bHasFuel)
	{
		return;
	}
	
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
 * Modifies the number of bananas on the ship part.
 * 
 * @param Delta Value to modify the number of bananas by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyBananas(int Delta)
{
	if (Bananas + Delta > MaxBananas)
	{
		Bananas = MaxBananas;
	}
	else if (Bananas + Delta < 0)
	{
		if (Bananas != 0)
		{
			OnBananasEmpty.Broadcast(true);
		}
		Bananas = 0;
	}
	else
	{
		if (Bananas == 0)
		{
			OnBananasEmpty.Broadcast(false);
		}
		Bananas += Delta;
	}
}
