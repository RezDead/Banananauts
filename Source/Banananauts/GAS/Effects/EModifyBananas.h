// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EModifyBananas.generated.h"

/**
 * Adds or removes bananas from a ship segment. Only intended for use with the ship's segment manager.
 * 
 * Last Edited: 9/17/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API UEModifyBananas : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UEModifyBananas();
};
