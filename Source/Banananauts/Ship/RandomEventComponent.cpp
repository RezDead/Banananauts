// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "RandomEventComponent.h"
#include "Banananauts/Managers/ShipManager.h"


URandomEventComponent::URandomEventComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	ClearEventFrequency = 1.0f;
	WarningEventFrequency = 0.5f;
	MinFirstEvent = 12.0f;
	MaxFirstEvent = 50.0f;
	MinRandBetweenEvent = 35.0f;
	MaxRandBetweenEvent = 60.0f;
	ToWarningEventDelay = 10.0f;
	ToClearEventDelay = 5.0f;
	bRandomEvents = false;
	bForcedEvent = false;
}


void URandomEventComponent::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Sets ship manager and subscribes to status change
 * @param NewShipManager New ship manager
 */
void URandomEventComponent::SetShipManager(AShipManager* NewShipManager)
{
	if (!NewShipManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomEventComponent::SetShipManager - New ship is null"));
		return;
	}
	
	ShipManager = NewShipManager;
	ShipManager->OnShipStatusChanged.AddDynamic(this, &URandomEventComponent::HandleShipStatusChanged);
}

/**
 * Begins the random events timer
 */
void URandomEventComponent::BeginRandomEvents()
{
	CurrEventTimerDuration = FMath::RandRange(MinFirstEvent, MaxFirstEvent);
	GetWorld()->GetTimerManager().SetTimer(RandomEventHandle, this, &URandomEventComponent::FirstEvent, CurrEventTimerDuration, false);
}

/**
 * Begins the next random event based on the current status of the ship.
 */
void URandomEventComponent::StartRandomEventTimer()
{
	float Duration = 0.0f;
	bForcedEvent = false;
	
	switch (ShipManager->GetStatus()) {
	case EShipStatus::Clear:
		CurrEventTimerDuration = FMath::RandRange(MinRandBetweenEvent, MaxRandBetweenEvent);
		Duration = CurrEventTimerDuration * (1/ClearEventFrequency);
		break;
		
	case EShipStatus::Warning:
		CurrEventTimerDuration =  FMath::RandRange(MinRandBetweenEvent, MaxRandBetweenEvent);
		Duration = CurrEventTimerDuration * (1/WarningEventFrequency);
		break;
		
	case EShipStatus::Fail:
		CurrEventTimerDuration = 0.0f;
		break;
	}
	
	if (CurrEventTimerDuration > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomEventHandle);
		GetWorld()->GetTimerManager().SetTimer(RandomEventHandle, this, &URandomEventComponent::RandomEvent, Duration, false);
	}
}

/**
 * First event that occurs
 */
void URandomEventComponent::FirstEvent()
{
	bRandomEvents = true;
	RandomEvent();
}

/**
 * What happens when a random event occurs
 */
void URandomEventComponent::RandomEvent()
{
	ShipManager->TriggerEvent(EShipSection::Whole, 1);
	
	StartRandomEventTimer();
}

/**
 * Updates the rate of the current random event timer.
 * @param NewRate The rate the timer will now be set to.
 */
void URandomEventComponent::UpdateEventTimerRate(const float NewRate)
{
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(RandomEventHandle);
	float PercentRemaining = RemainingTime / CurrEventTimerDuration;
	float NewDuration = CurrEventTimerDuration * PercentRemaining * (1/NewRate);
	
	GetWorld()->GetTimerManager().ClearTimer(RandomEventHandle);
	GetWorld()->GetTimerManager().SetTimer(RandomEventHandle, this, &URandomEventComponent::RandomEvent, NewDuration, false);
}

/**
 * Handles when the ship status changes.
 * @param OldStatus The old status of the ship.
 * @param NewStatus The new status of the ship.
 */
void URandomEventComponent::HandleShipStatusChanged(EShipStatus OldStatus, EShipStatus NewStatus)
{
	if (!bRandomEvents) return;

	// Clear to Warning
	if (OldStatus == EShipStatus::Clear && NewStatus == EShipStatus::Warning)
	{
		if (!bForcedEvent)
		{
			UpdateEventTimerRate(WarningEventFrequency);
		}
	}
	
	// Any state to Fail
	else if (NewStatus == EShipStatus::Fail)
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomEventHandle);
		bForcedEvent = false;
	}
	
	// Warning to Clear or Fail to Clear
	else if (NewStatus == EShipStatus::Clear && (OldStatus == EShipStatus::Warning || OldStatus == EShipStatus::Fail))
	{
		if (!bForcedEvent)
		{
			bForcedEvent = true;
			CurrEventTimerDuration = ToClearEventDelay;
			GetWorld()->GetTimerManager().SetTimer(RandomEventHandle, this, &URandomEventComponent::RandomEvent, CurrEventTimerDuration, false);
		}
	}
	
	// Fail to Warning
	else if (OldStatus == EShipStatus::Fail && NewStatus == EShipStatus::Warning)
	{
		if (!bForcedEvent)
		{
			bForcedEvent = true;
			CurrEventTimerDuration = ToWarningEventDelay;
			GetWorld()->GetTimerManager().SetTimer(RandomEventHandle, this, &URandomEventComponent::RandomEvent, CurrEventTimerDuration, false);
		}
	}
}

