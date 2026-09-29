// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "ShipSegmentUtility.h"

#include "Banananauts/GAS/Attributes/Items/ItemMass.h"
#include "Banananauts/GAS/Effects/EModifyShipMass.h"

void UShipSegmentUtility::AddItemMassToShip(UAbilitySystemComponent* ItemASC,
                                            UAbilitySystemComponent* ShipASC)
{
	FGameplayEffectContextHandle ContextHandle = ItemASC->MakeEffectContext();
	ContextHandle.AddInstigator(ItemASC->GetAvatarActor(), ItemASC->GetAvatarActor());
	
	FGameplayEffectSpecHandle SpecHandle = ItemASC->MakeOutgoingSpec(
		UEModifyShipMass::StaticClass(),
		1.0f,
		ContextHandle
	);
	
	const float Mass = ItemASC->GetNumericAttribute(UItemMass::GetMassAttribute());
	
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Magnitude")), Mass);

	ItemASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ShipASC);
}
