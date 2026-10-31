// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.


#include "SnapPointManager.h"

void USnapPointManager::DisplaySnapPoint(const FGameplayTag TypeTag)
{
	if (TypeTag == CurrentTag) return;
	
	CurrentTag = TypeTag;
	
	if (CurrentTag.IsValid())
	{
		DisplayAttachTypeEvent.Broadcast(CurrentTag);
	}
	else if (bDisplaying)
	{
		bDisplaying = false;
		StopDisplayEvent.Broadcast();
	}
}

void USnapPointManager::StopDisplay()
{
	bDisplaying = false;
	CurrentTag = FGameplayTag::EmptyTag;
	StopDisplayEvent.Broadcast();
}
