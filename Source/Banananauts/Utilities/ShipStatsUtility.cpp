// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipStatsUtility.h"

#include "Banananauts/GAS/Effects/EModifyBananas.h"


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
		Total += Segment->GetAbilitySystemComponent()->GetNumericAttribute(UShipSegmentAttributes::GetBananasAttribute());
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
		Total += Segment->GetAbilitySystemComponent()->GetNumericAttribute(UShipSegmentAttributes::GetMaxBananasAttribute());
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
 * @return True if all remaining bananas were removed, false if not or the ability system component is invalid.
 */
bool UShipStatsUtility::RemoveBananaHelper(int& Amount, AShipSegmentManager* Segment)
{
	UAbilitySystemComponent* ASC = Segment->GetAbilitySystemComponent();
	if (!ASC) {return false;}

	const int Num = ASC->GetNumericAttribute(UShipSegmentAttributes::GetBananasAttribute());

	const FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		UEModifyBananas::StaticClass(),
		1.0f,
		ContextHandle
	);
	
	if (Num < Amount)
	{
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Magnitude")), -Num);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		Amount -= Num;
		return false;
	}

	SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Magnitude")), -Amount);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return true;
}

/**
 * Helper function to add bananas to a ship segment.
 * 
 * @param Amount Number of bananas remaining to add.
 * @param Segment Array of ship segments.
 * @return True if all remaining bananas were added, false if not or the ability system component is invalid.
 */
bool UShipStatsUtility::AddBananaHelper(int& Amount, AShipSegmentManager* Segment)
{
	UAbilitySystemComponent* ASC = Segment->GetAbilitySystemComponent();
	if (!ASC) {return false;}
	
	//Max Bananas - Bananas
	int Num = ASC->GetNumericAttribute(UShipSegmentAttributes::GetMaxBananasAttribute()) - ASC->GetNumericAttribute(UShipSegmentAttributes::GetBananasAttribute());
	
	const FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		UEModifyBananas::StaticClass(),
		1.0f,
		ContextHandle
	);
	
	if (Num < Amount)
	{
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Magnitude")), Num);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		Amount -= Num;
		return false;
	}
	
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Magnitude")), Amount);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return true;
}

/**
* Calculates the heat multiplier for a given ship segment.
* 
* @param Segment The ship segment to calculate the heat multiplier for.
* @return The heat multiplier for the given ship segment, 0 if ASC is null.
*/
float UShipStatsUtility::CalculateHeatMult(const AShipSegmentManager* Segment)
{
	UAbilitySystemComponent* ASC = Segment->GetAbilitySystemComponent();
	if (!ASC) {return 0;}
	
	if (ASC->GetNumericAttribute(UShipSegmentAttributes::GetMaxHeatAblationAttribute()) == 0.0f) return .5f;

	const float HeatAblationPercent = ASC->GetNumericAttribute(UShipSegmentAttributes::GetHeatAblationAttribute()) / ASC->GetNumericAttribute(UShipSegmentAttributes::GetMaxHeatAblationAttribute());

	//If less than 50% heat ablation
	if (HeatAblationPercent <= 0.5f)
	{
		// Mult by two to accomodate for lerp range
		return FMath::Lerp(2.0f, 1.0f, HeatAblationPercent * 2.0f);
	}

	// -0.5f because we want to remove the 0-50% range and just calculate the 50-100% range, *2 to compensate for loss
	return FMath::Lerp(1.0f, 0.5f, (HeatAblationPercent - 0.5f) * 2.0f);
}

/**
 * Calculates the percent progress additive for the ship since last tick.
 * 
 * @param Mass Mass of the ship
 * @param Thrust Thrust of the ship
 * @param MinTime Minimum time for the ship to reach max progress
 * @param LinearGrowthRate Linear growth rate of the ship
 * @param DeltaSeconds Delta time since last tick
 * @return Percent progress additive for the ship since last tick
 */
float UShipStatsUtility::CalculateShipProgressAdditive(const float& Mass, const float& Thrust, const float& MinTime,
                                                       const float& LinearGrowthRate, const float& DeltaSeconds)
{
	//Time at a given moment
	const float Time = MinTime + LinearGrowthRate * Mass - Thrust;
	
	return DeltaSeconds / Time;
}

