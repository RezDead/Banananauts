// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "FlightRouter.h"

#include "Banananauts/Data/Tables/TRB_Levels.h"
#include "Banananauts/Interfaces/ShipCommunication.h"
#include "Banananauts/Subsystems/LevelManagerSubsystem.h"


AFlightRouter::AFlightRouter()
{
	RouteType = EFlightType::Flight;
}

/**
 * What happens when the ship reaches the end of the route.
 */
void AFlightRouter::Success()
{
	Super::Success();
	
	//Game Won
}

/**
 * What happens when the ship fails the route.
 */
void AFlightRouter::Failure()
{
	Super::Failure();
	
	const TSoftObjectPtr<UWorld> LevelToLoad = GetRandomLevel();
	
	if (ULevelManagerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULevelManagerSubsystem>())
	{
		Subsystem->LoadLevel(LevelToLoad);
	}
}

void AFlightRouter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Begins flight route
 */
void AFlightRouter::BeginRoute() const
{
	if (const IShipCommunication* Communication = Cast<IShipCommunication>(ShipRef))
	{
		Communication->Execute_BeginFlightSequence(ShipRef);
	}
}

TSoftObjectPtr<UWorld> AFlightRouter::GetRandomLevel() const
{
	if (!LevelsDT) {return nullptr;}
	TArray<FName> RowNames = LevelsDT->GetRowNames();
	
	if (RowNames.Num() == 0) {return nullptr;}
	static const FString Context = TEXT("Fetching random level");
	
	TSoftObjectPtr<UWorld> Level = LevelsDT->FindRow<FTRB_Levels>(RowNames[FMath::RandRange(0, RowNames.Num() - 1)], Context)->Level;
	
	return Level;
}

void AFlightRouter::TriggerEvent(const FGameplayTag EventAction)
{
	Super::TriggerEvent(EventAction);
	
	FGameplayTagContainer EventTag;
	EventTag.AddTag(EventAction);
	
	if (!EventTag.HasTag(FGameplayTag::RequestGameplayTag(FName("Event.Flight"))))
	{
		UE_LOG(LogTemp, Error, TEXT("FlightRouter::TriggerEvent - Invalid event tag, no event triggered"));
		return;
	}
	
	if (EventTag.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Event.Flight.Turbulence.1"))))
	{
		TriggerTurbulenceEvents(TurbulenceZone1Events);
	}
	else if (EventTag.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Event.Flight.Turbulence.2"))))
	{
		TriggerTurbulenceEvents(TurbulenceZone2Events);
	}
	else if (EventTag.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Event.Flight.Turbulence.3"))))
	{
		TriggerTurbulenceEvents(TurbulenceZone3Events);
	}
	else if (EventTag.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Event.Flight.FinalStretch"))))
	{
		TriggerTurbulenceEvents(FinalStretchEvents);
	}
	
}


void AFlightRouter::TriggerTurbulenceEvents(const TArray<FTurbulenceEvent>& Events) const
{
	if (ShipRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("FlightRouter::TriggerTurbulenceEvents - ShipRef is null"));
		return;
	}
	
	TMap<EEventType, int> EventsToTrigger;

	for (const auto& Event : Events)
	{
		//If fail chance check, continue
		if (FMath::FRandRange(0.0f, 1.0f) > Event.Chance)
			continue;
		
		if (!EventsToTrigger.Contains(Event.Type))
		{
			EventsToTrigger.Add(Event.Type, 1);
			continue;
		}
		
		EventsToTrigger[Event.Type]++;
	}
	
	for (const auto& Event : EventsToTrigger)
	{
		IShipCommunication::Execute_TriggerEvent(ShipRef, Event.Key, EShipSection::Whole, Event.Value);
	}
}

