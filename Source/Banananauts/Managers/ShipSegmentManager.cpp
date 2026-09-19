// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentManager.h"

UShipSegmentManager::UShipSegmentManager()
{
	BurnStatus = EBurnStatus::Optimal;
}

/**
 * Modifies the current heat of the ship segment and updates burn status as needed.
 * 
 * @param Delta Value to modify the current heat by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyHeat(float Delta)
{
	Heat += Delta;
	
	if (Heat > MaxHeat)
	{
		Heat = MaxHeat;
	}
	else if (Heat < 0)
	{
		Heat = 0;
	}
	
	if ( Heat/MaxHeat > .99 && BurnStatus != EBurnStatus::Explosive)
	{
		BurnStatus = EBurnStatus::Explosive;
		OnHeatStatusChanged.Broadcast(BurnStatus);
	}
	else if ( Heat/MaxHeat > .66 && BurnStatus != EBurnStatus::Burning)
	{
		BurnStatus = EBurnStatus::Burning;
		OnHeatStatusChanged.Broadcast(BurnStatus);
	}
	else if ( Heat/MaxHeat > .33 && BurnStatus != EBurnStatus::Overheating)
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
 * Modifies the maximum heat of the ship segment.
 * 
 * @param Delta Value to modify the maximum heat by. Can be positive or negative.
 */
void UShipSegmentManager::ModifyMaxHeat(float Delta)
{
	MaxHeat += Delta;
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
 * Modifies the mass of the ship segment.
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
 * Modifies the number of bananas on the ship segment.
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
