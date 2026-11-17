// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Enums/FailState.h"
#include "GameFramework/Actor.h"
#include "EndStateManager.generated.h"

UCLASS()
class BANANANAUTS_API AEndStateManager : public AActor
{
	GENERATED_BODY()

public:
	AEndStateManager();
	
	UFUNCTION(BlueprintNativeEvent)
	void BeginFailState(EFailState State);
	UFUNCTION(BlueprintNativeEvent)
	void CancelFailState(EFailState State);
	UFUNCTION(BlueprintNativeEvent)
	void Success();
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInFailState(const EFailState State) const {return ActiveFailStates.Contains(State);}
	
	UPROPERTY()
	TSet<EFailState> ActiveFailStates;
	
};
