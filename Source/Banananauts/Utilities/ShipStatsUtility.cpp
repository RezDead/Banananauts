// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipStatsUtility.h"

/**
 * Get total ship mass by summing the mass of all ship segments.
 * 
 * @param Segments Array of ship segments.
 * @return Total mass of the ship.
 */
float UShipStatsUtility::GetShipMass(const TArray<AShipSegmentManager*>& Segments)
{
	float Total = 0;
	for (const AShipSegmentManager* Segment : Segments)
	{
		Total += Segment->GetMass();
	}
	return Total;
}

/**
 * Get maximum ship mass by summing the maximum mass of all ship segments.
 * 
 * @param Segments Array of ship segments.
 * @return Maximum mass of the ship.
 */
float UShipStatsUtility::GetMaxShipMass(const TArray<AShipSegmentManager*>& Segments)
{
	float Total = 0;
	for (const AShipSegmentManager* Segment : Segments)
	{
		Total += Segment->GetMaxMass();
	}
	return Total;
}

/**
 * Get the total count of all bananas on the ship.
 * 
 * @param Segments Array of ship segments.
 * @return Total count of bananas on the ship.
 */
int UShipStatsUtility::GetBananaCount(const TArray<AShipSegmentManager*>& Segments)
{
	int Total = 0;
	for (const AShipSegmentManager* Segment : Segments)
	{
		Total += Segment->GetBananas();
	}
	return Total;
}

/**
 * Get the total capacity of all bananas on the ship.
 * 
 * @param Segments Array of ship segments.
 * @return Total capacity of bananas on the ship.
 */
float UShipStatsUtility::GetBananaCapacity(const TArray<AShipSegmentManager*>& Segments)
{
	float Total = 0;
	for (const AShipSegmentManager* Segment : Segments)
	{
		Total += Segment->GetMaxBananas();
	}
	return Total;
}

/**
 * Use bananas from the ship and its segments if available.
 * 
 * @param Amount Number of bananas to use. Must be greater than 0.
 * @param Segments Array of ship segments.
 * @return True if the operation was successful, false if bananas are insufficient or invalid amount.
 */
bool UShipStatsUtility::UseBananas(int& Amount, const TArray<AShipSegmentManager*>& Segments)
{
	if (Amount <= 0) {return false;}
	if (GetBananaCount(Segments) < Amount){return false;}
	
	int i = 0;
	while (Amount != 0 && i < Segments.Num())
	{
		if (RemoveBananaHelper(Amount, Segments[i])) {return true;}
		i++;
	}
	return false;
}

/**
 * Try to add bananas to the ship and its segments if you can add.
 * 
 * @param Amount Number of bananas to add. Must be greater than 0.
 * @param Segments Array of ship segments.
 * @return True if the operation was successful, false if bananas are full or invalid amount added.
 */
bool UShipStatsUtility::AddBananas(int& Amount, const TArray<AShipSegmentManager*>& Segments)
{
	if (Amount <= 0) {return false;}
	if (GetBananaCount(Segments) >= GetBananaCapacity(Segments)){return false;}
	
	int i = 0;
	while (Amount != 0 && i < Segments.Num())
	{
		if (AddBananaHelper(Amount, Segments[i])) {return true;}
		i++;
	}
	return false;
}

/**
 * Helper function to remove bananas from a ship segment.
 * 
 * @param Amount Number of bananas remaining to remove.
 * @param Segment Array of ship segments.
 * @return True if all remaining bananas were removed, false otherwise.
 */
bool UShipStatsUtility::RemoveBananaHelper(int& Amount, AShipSegmentManager* Segment)
{
	int Num = Segment->GetBananas();
	
	if (Num < Amount)
	{
		Segment->ModifyBananas(-Num);
		Amount -= Num;
		return false;
	}

	Segment->ModifyBananas(-Amount);
	return true;
}

/**
 * Helper function to add bananas to a ship segment.
 * 
 * @param Amount Number of bananas remaining to add.
 * @param Segment Array of ship segments.
 * @return True if all remaining bananas were added, false otherwise.
 */
bool UShipStatsUtility::AddBananaHelper(int& Amount, AShipSegmentManager* Segment)
{
	int Num = Segment->GetMaxBananas() - Segment->GetBananas();
	
	if (Num < Amount)
	{
		Segment->ModifyBananas(Num);
		Amount -= Num;
		return false;
	}
	
	Segment->ModifyBananas(Amount);
	return true;
}

/**
* Calculates the heat multiplier for a given ship segment.
* 
* @param Segment The ship segment to calculate the heat multiplier for.
* @return The heat multiplier for the given ship segment.
*/
float UShipStatsUtility::CalculateHeatMult(const AShipSegmentManager* Segment)
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

