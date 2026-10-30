// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "SnapPoint.generated.h"

UCLASS(Abstract)
class BANANANAUTS_API ASnapPoint : public AActor, public IGameplayTagAssetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASnapPoint();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UAbilitySystemComponent* AbilitySystemComponent;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGameplayTagContainer GameplayTags;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }
};
