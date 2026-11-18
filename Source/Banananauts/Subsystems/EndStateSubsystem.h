// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Enums/FailState.h"
#include "Subsystems/WorldSubsystem.h"
#include "EndStateSubsystem.generated.h"

class AEndStateManager;

/**
 * Allows any actor to call fail state conditions.
 */
UCLASS()
class BANANANAUTS_API UEndStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	UEndStateSubsystem();
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	UFUNCTION(BlueprintCallable)
	void BeginFailState(EFailState State) const;
	UFUNCTION(BlueprintCallable)
	void CancelFailState(EFailState State) const;
	UFUNCTION(BlueprintCallable)
	void TriggerSuccess() const;
	
	UFUNCTION(BlueprintCallable)
	bool CheckIsInFailState(EFailState State) const;
	
protected:
	
	bool bManagerActive;
	UPROPERTY()
	AEndStateManager* EndStateManager;
};
