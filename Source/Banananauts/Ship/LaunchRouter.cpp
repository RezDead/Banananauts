// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "LaunchRouter.h"
#include "Banananauts/Subsystems/LevelManagerSubsystem.h"
#include "Banananauts/Subsystems/SaveSubsystem.h"


// Sets default values
ALaunchRouter::ALaunchRouter()
{
	RouteType = EFlightType::Launch;
}

/**
 * What happens when the ship reaches the end of the route.
 */
void ALaunchRouter::Success()
{
	Super::Success();
	
	//Launch Complete, load flight level
		
	if (USaveSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>())
	{
		Subsystem->SaveGame();
	}
		
	if (ULevelManagerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULevelManagerSubsystem>())
	{
		Subsystem->LoadLevel(FlightLevel);
	}
}

/**
 * What happens when the ship fails the route.
 */
void ALaunchRouter::Failure()
{
	Super::Failure();
}

void ALaunchRouter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Begins flight route
 */
void ALaunchRouter::BeginRoute() const
{
	Super::BeginRoute();
	
	if (!FlightLevel.ToSoftObjectPath().IsValid())
	{
		UE_LOG(LogTemp, Fatal, TEXT("AShipRouter::BeginRoute(): Invalid FlightLevel reference"));
	}
}

