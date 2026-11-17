// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "EndStateSubsystem.h"
#include "Banananauts/Managers/EndStateManager.h"
#include "Kismet/GameplayStatics.h"

UEndStateSubsystem::UEndStateSubsystem()
{
	bManagerActive = false;
	EndStateManager = nullptr;
}

// Initializes subsystem
void UEndStateSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(this, AEndStateManager::StaticClass());
	
	EndStateManager = Cast<AEndStateManager>(FoundActor);
	
	if (EndStateManager)
	{
		bManagerActive = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("End State Subsystem is not active. Ensure fail manager is in world."));
	}
}

/**
 * Calls begin fail state in the end state manager actor
 * @param State Fail state to start
 */
void UEndStateSubsystem::BeginFailState(const EFailState State) const
{
	if (!bManagerActive) return;
	
	EndStateManager->BeginFailState(State);
}

/**
 * Calls cancel fail state in the end state manager actor
 * @param State 
 */
void UEndStateSubsystem::CancelFailState(const EFailState State) const
{
	if (!bManagerActive) return;
	
	EndStateManager->CancelFailState(State);
}

/**
 * Triggers success in the end state manager actor
 */
void UEndStateSubsystem::TriggerSuccess() const
{
	if (!bManagerActive) return;
	
	EndStateManager->Success();
}

/**
 * Checks if a fail state is currently active
 * @param State State to check
 * @return True if active, false if not or manager not active
 */
bool UEndStateSubsystem::CheckIsInFailState(const EFailState State) const
{
	if (!bManagerActive) return false;
	
	return EndStateManager->IsInFailState(State);
}
