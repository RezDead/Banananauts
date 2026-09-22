// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentAttributes.h"

UShipSegmentAttributes::UShipSegmentAttributes()
{
	Heat = 0;
	MaxHeat = 0;
	HeatAblation = 0;
	MaxHeatAblation = 0;
	Bananas = 0;
	MaxBananas = 0;
}

void UShipSegmentAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	//Heat handling + burn state
	if (Attribute == GetHeatAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHeat());

		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			if (const float HeatPercent = GetHeat()/GetMaxHeat(); HeatPercent > .99 && !ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Explosive")))
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Explosive"));
			}
			else if (HeatPercent > .66 && !ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Burning")))
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Burning"));
			}
			else if (HeatPercent > .33 && !ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Overheating")))
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Overheating"));
			}
			else if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Optimal")))
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Status.Heat.Optimal"));
			}
		}
	}
	
	//Ablation handling
	else if (Attribute == GetHeatAblationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHeatAblation());
	}
	
	//Banana handling
	else if (Attribute == GetBananasAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxBananas());
	}
}
