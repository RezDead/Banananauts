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
	if (const IShipCommunication* Communication = Cast<IShipCommunication>(Ship))
	{
		Communication->Execute_BeginFlightSequence(Ship);
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

