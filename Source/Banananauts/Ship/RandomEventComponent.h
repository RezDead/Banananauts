// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Enums/ShipStatus.h"
#include "Components/ActorComponent.h"
#include "RandomEventComponent.generated.h"

class AShipManager;

UCLASS(meta=(BlueprintSpawnableComponent))
class BANANANAUTS_API URandomEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URandomEventComponent();

	virtual void BeginPlay() override;
	
	void BeginRandomEvents();
	void SetShipManager(AShipManager* NewShipManager);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events",
		meta = (ToolTip = "Multipler for event timer when status is clear."))
	float ClearEventFrequency;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events",
		meta = (ToolTip = "Multipler for event timer when status is warning."))
	float WarningEventFrequency;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinFirstEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxFirstEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinRandBetweenEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxRandBetweenEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float ToWarningEventDelay;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float ToClearEventDelay;
	float CurrEventTimerDuration = 0.0f;
	
	
	FTimerHandle RandomEventHandle;
	void StartRandomEventTimer();
	void FirstEvent();
	void RandomEvent();
	
	void UpdateEventTimerRate(const float NewRate);
	UFUNCTION()
	void HandleShipStatusChanged(EShipStatus OldStatus, EShipStatus NewStatus);
	
	bool bRandomEvents;
	bool bForcedEvent;
	
	UPROPERTY()
	AShipManager* ShipManager;
};
