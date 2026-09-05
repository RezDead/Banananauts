// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WSS_FocusManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnterFocusModeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExitFocusModeSignature);

/**
 * 
 */
UCLASS()
class BANANANAUTS_API UWSS_FocusManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Focus Mode")
	FEnterFocusModeSignature OnEnterFocusMode;
	UPROPERTY(BlueprintAssignable, Category = "Focus Mode")
	FExitFocusModeSignature OnExitFocusMode;
	
	UFUNCTION(BlueprintCallable, Category = "Focus Mode")
	void EnterFocusMode() { OnEnterFocusMode.Broadcast(); }
	
	UFUNCTION(BlueprintCallable, Category = "Focus Mode")
	void ExitFocusMode() { OnExitFocusMode.Broadcast(); }
};
