// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "ShipSegmentManager.h"
#include "Banananauts/GAS/Attributes/ShipAttributes.h"
#include "Banananauts/Ship/ShipRouter.h"
#include "Banananauts/Structs/FuelComposition.h"
#include "UObject/Object.h"
#include "ShipManager.generated.h"

/**
 * Manages the ship's segments and systems. Also provides access points to vital ship stats and functions.
 * 
 * Last Edited: 9/21/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS(PrioritizeCategories="Default Default|Stats Default|Segments")
class BANANANAUTS_API AShipManager : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AShipManager();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UShipAttributes* Attributes;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	UFUNCTION(BlueprintCallable, Category = "Default|Management")
	void InitiateFlight();
	
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	int GetBananaCount();
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	int GetBananaCapacity();
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	bool UseBananas(int Amount);
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	bool AddBananas(int Amount);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Segments")
	TObjectPtr<UChildActorComponent> NoseSegmentComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Segments")
	TObjectPtr<UChildActorComponent> BodySegmentComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Segments")
	TObjectPtr<UChildActorComponent> TailSegmentComponent;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Default|Segments")
	TObjectPtr<AShipSegmentManager> Nose;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Default|Segments")
	TObjectPtr<AShipSegmentManager> Body;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Default|Segments")
	TObjectPtr<AShipSegmentManager> Tail;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (ToolTip = "The rate in seconds at which ship systems tick"))
	float SystemTickRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Stats", meta = (ToolTip = "The base amount of heat gained per tick before any modifiers"))
	float BaseHeatGainPerTick;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats", meta = (AllowPrivateAccess = "true"))
	float MaxMass = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats", meta = (AllowPrivateAccess = "true"))
	float MaxFuel = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight", meta = (AllowPrivateAccess = "true"))
	bool bIsFlying = false;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AShipRouter> ShipRouter = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight", meta = (AllowPrivateAccess = "true"))
	float ShipProgress = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight", meta = (AllowPrivateAccess = "true"))
	float MinFlightTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight", meta = (AllowPrivateAccess = "true"))
	float FlightLinearGrowthRate = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Stats", meta = (AllowPrivateAccess = "true"))
	FFuelComposition FuelComposition;
	
	void InitSegments();
	void InitAttributes() const;
	
	void TickSystems();
	FTimerHandle TickHandle;
	
	UPROPERTY()
	TArray<TObjectPtr<AShipSegmentManager>> Segments;
	
	void UpdateHeat() const;
};
