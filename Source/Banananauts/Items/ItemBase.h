// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class BANANANAUTS_API AItemBase : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AItemBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
