// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipStatsUtility.h"

#include "Banananauts/Data/TRB_Levels.h"
#include "Banananauts/GAS/Attributes/Items/ItemStability.h"
#include "Banananauts/GAS/Effects/EModifyBananas.h"


/**
 * Get the total count of all bananas on the ship.
 * 
 * @param Segments Map of ship section types to segment instances.
 * @return Total count of bananas on the ship.
 */
int UShipStatsUtility::GetBananaCount(const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments)
{
	int Total = 0;
	for (const TPair<EShipSection, TObjectPtr<AShipSegmentManager>>& SegmentPair : Segments)
	{
		const AShipSegmentManager* Segment = SegmentPair.Value.Get();
		if (!Segment)
		{
			continue;
		}

		Total += Segment->GetAbilitySystemComponent()->GetNumericAttribute(UShipSegmentAttributes::GetBananasAttribute());
	}
	return Total;
}

/**
 * Get the total capacity of all bananas on the ship.
 * 
 * @param Segments Map of ship section types to segment instances.
 * @return Total capacity of bananas on the ship.
 */
float UShipStatsUtility::GetBananaCapacity(const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments)
{
	float Total = 0;
	for (const TPair<EShipSection, TObjectPtr<AShipSegmentManager>>& SegmentPair : Segments)
	{
		const AShipSegmentManager* Segment = SegmentPair.Value.Get();
		if (!Segment)
		{
			continue;
		}

		Total += Segment->GetAbilitySystemComponent()->GetNumericAttribute(UShipSegmentAttributes::GetMaxBananasAttribute());
	}
	return Total;
}

/**
 * Use bananas from the ship and its segments if available.
 * 
 * @param Amount Number of bananas to use. Must be greater than 0.
 * @param Segments Map of ship section types to segment instances.
 * @return True if the operation was successful, false if bananas are insufficient or invalid amount.
 */
bool UShipStatsUtility::UseBananas(int& Amount, const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments)
{
	if (Amount <= 0) {return false;}
	if (GetBananaCount(Segments) < Amount){return false;}
	
	for (const TPair<EShipSection, TObjectPtr<AShipSegmentManager>>& SegmentPair : Segments)
	{
		AShipSegmentManager* Segment = SegmentPair.Value.Get();
		if (!Segment)
		{
			continue;
		}

		if (RemoveBananaHelper(Amount, Segment)) {return true;}
	}
	return false;
}

/**
 * Try to add bananas to the ship and its segments if you can add.
 * 
 * @param Amount Number of bananas to add. Must be greater than 0.
 * @param Segments Map of ship section types to segment instances.
 * @return True if the operation was successful, false if bananas are full or invalid amount added.
 */
bool UShipStatsUtility::AddBananas(int& Amount, const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments)
{
	if (Amount <= 0) {return false;}
	if (GetBananaCount(Segments) >= GetBananaCapacity(Segments)){return false;}
	
	for (const TPair<EShipSection, TObjectPtr<AShipSegmentManager>>& SegmentPair : Segments)
	{
		AShipSegmentManager* Segment = SegmentPair.Value.Get();
		if (!Segment)
		{
			continue;
		}

		if (AddBananaHelper(Amount, Segment)) {return true;}
	}
	return false;
}

void UShipStatsUtility::PopOffEvent(const EShipSection Section, const TMap<EShipSection, TObjectPtr<AShipSegmentManager>>& Segments)
{
	TArray<AActor*> AttachedItems;
	
	switch (Section)
	{
	case EShipSection::Whole:
		AttachedItems.Append(Segments[EShipSection::Nose].Get()->GetAttachedItems());
		AttachedItems.Append(Segments[EShipSection::Body].Get()->GetAttachedItems());
		AttachedItems.Append(Segments[EShipSection::Tail].Get()->GetAttachedItems());
		break;
	case EShipSection::Nose:
		AttachedItems.Append(Segments[EShipSection::Nose].Get()->GetAttachedItems());
		break;
	case EShipSection::Body:
		AttachedItems.Append(Segments[EShipSection::Body].Get()->GetAttachedItems());
		break;
	case EShipSection::Tail:
		AttachedItems.Append(Segments[EShipSection::Tail].Get()->GetAttachedItems());
		break;
	}
	
	PopOffItem(AttachedItems);
}

TSoftObjectPtr<UWorld> UShipStatsUtility::GetRandomLevel(const UDataTable* LevelsDT)
{
	if (!LevelsDT) {return nullptr;}
	TArray<FName> RowNames = LevelsDT->GetRowNames();
	
	if (RowNames.Num() == 0) {return nullptr;}
	static const FString Context = TEXT("Fetching random level");
	
	TSoftObjectPtr<UWorld> Level = LevelsDT->FindRow<FTRB_Levels>(RowNames[FMath::RandRange(0, RowNames.Num() - 1)], Context)->Level;
	
	return Level;
	
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

void UShipStatsUtility::PopOffItem(TArray<AActor*>& AttachedItems)
{
	TArray<float> Weight;
	float TotalWeight = 0;
	
	for (int i = 0; i < AttachedItems.Num(); i++)
	{
		UAbilitySystemComponent* ItemASC;

		if (const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(AttachedItems[i]))
		{
			ItemASC = ASCInterface->GetAbilitySystemComponent();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ShipStatsUtility::PopOffItem - Failed to get ASC from actor %s"), *AttachedItems[i]->GetName());
			continue;
		}
		
		Weight.Add(100 - ItemASC->GetNumericAttribute(UItemStability::GetStabilityAttribute()));
		TotalWeight += Weight[i];
	}
	
	float CurrentWeight = 0;
	float TargetWeight = FMath::FRandRange(0.0f, TotalWeight);
	
	for (int i = 0; i < AttachedItems.Num(); i++)
	{
		
	}
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
	//Time at a given moment (60 is to convert min to seconds)
	const float Time = 60 * (MinTime + LinearGrowthRate * Mass - Thrust);
	
	return DeltaSeconds / Time;
}

