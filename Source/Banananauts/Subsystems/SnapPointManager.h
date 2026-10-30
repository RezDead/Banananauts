// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "SnapPointManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisplaySnapType, FGameplayTag, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopDisplay);

/**
 * Manages global event dispatches for attach snap points.
 * 
 * Last Edited:10/29/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API USnapPointManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FDisplaySnapType DisplayAttachTypeEvent;
	
	UPROPERTY(BlueprintAssignable)
	FStopDisplay StopDisplayEvent;
	
	UFUNCTION(BlueprintCallable)
	void DisplaySnapPoint(FGameplayTag TypeTag);
	
	UFUNCTION(BlueprintCallable)
	void StopDisplay();
	
protected:
	FGameplayTag CurrentTag;
	bool bDisplaying = false;
};
