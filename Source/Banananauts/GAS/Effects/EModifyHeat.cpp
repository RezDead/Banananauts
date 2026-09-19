// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "EModifyHeat.h"

#include "Banananauts/GAS/Attributes/ShipSegmentAttributes.h"

UEModifyHeat::UEModifyHeat()
{
	//Establish the duration policy
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//General Mod Info
	FGameplayModifierInfo ModInfo;
	ModInfo.Attribute = UShipSegmentAttributes::GetHeatAttribute();
	ModInfo.ModifierOp = EGameplayModOp::Additive;

	//Use gameplay tag "Data.Magnitude" to set the modifier magnitude
	FSetByCallerFloat SetByCallerData;
	SetByCallerData.DataTag = FGameplayTag::RequestGameplayTag("Data.Magnitude");

	//Add set by caller info to mod
	ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerData);

	Modifiers.Add(ModInfo);
	
}
