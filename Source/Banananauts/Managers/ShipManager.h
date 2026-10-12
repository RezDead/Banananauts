// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "ShipSegmentManager.h"
#include "Banananauts/Enums/ShipSection.h"
#include "Banananauts/Enums/ShipStatus.h"
#include "Banananauts/GAS/Attributes/ShipAttributes.h"
#include "Banananauts/Interfaces/ShipCommunication.h"
#include "Banananauts/Ship/ShipRouter.h"
#include "Banananauts/Structs/FuelComposition.h"
#include "ShipManager.generated.h"

/**
 * Manages the ship's segments and systems. Also provides access points to vital ship stats and functions.
 * 
 * Last Edited: 10/12/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS(PrioritizeCategories="Default Default|Stats Default|Segments")
class BANANANAUTS_API AShipManager : public AActor, public IAbilitySystemInterface, public IShipCommunication,
                                     public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	AShipManager();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
	UShipAttributes* Attributes;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGameplayTagContainer GameplayTags;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }
	
	virtual void BeginFlightSequence_Implementation() override;
	
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	UDataTable* LevelsDT;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats")
	float MaxMass = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats")
	float MaxFuel = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats",
		meta = (ToolTip = "Percentage chance of adhere status blocking pop-off"))
	float AdhereStatusStrength = 0.0f;
	
	void SetupAttributeListeners();
	FDelegateHandle FuelListener;
	TMap<EShipSection, FDelegateHandle> HeatListeners;
	TMap<EShipSection, FDelegateHandle> StabilityListeners;
	
	void OnFuelChanged(const FOnAttributeChangeData& Data);
	void OnHeatChanged(const FOnAttributeChangeData& Data);
	void OnStabilityChanged(const FOnAttributeChangeData& Data);
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight")
	bool bIsFlying = false;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight")
	TWeakObjectPtr<AShipRouter> ShipRouter = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight")
	float ShipProgress = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight",
		meta = (AllowPrivateAccess = "true", ToolTip = "The rate at which gallons of fuel is consumed per minute"))
	float FuelBurnRate = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight")
	float MinFlightTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight")
	float FlightLinearGrowthRate = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Stats")
	FFuelComposition FuelComposition;
	
	void InitSegments();
	void InitAttributes() const;
	
	void TickSystems();
	FTimerHandle TickHandle;
	
	UPROPERTY()
	TMap<EShipSection, AShipSegmentManager*> Segments;
	
	void UpdateHeat() const;
	void ConsumeFuel(const float& DeltaTime) const;
	
	/*
	 * Win/Fail
	 */
	
	void Success();
	void Failure();
	
	bool bIsChangingLevel = false;
	
	/*
	 * Status
	 */
	
	// A custom key combining (FromStatus << 8 | ToStatus) to map transitions efficiently
	typedef uint16 FTransitionKey;
	TMap<FTransitionKey, TFunction<void()>> TransitionRegistry;

	// Helper to generate unique key
	static FTransitionKey GetTransitionKey(EShipStatus From, EShipStatus To)
	{return (static_cast<uint16>(From) << 8) | static_cast<uint16>(To);}
	
	void DefineStateTransitions();
	
	//Transition specific functions
	void OnClearToWarning();
	void OnWarningToFail();
	void OnFailToWarning();
	void OnWarningToClear();
	
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	EShipStatus Status;
	
	void UpdateStatus(EShipStatus NewStatus);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Status")
	float StabilityThreshold;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Status")
	float FuelThreshold;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Status")
	float HeatThreshold;
	
	/*
	 * Events
	 */
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinFirstEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxFirstEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinClear;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxClear;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinWarning;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxWarning;
	
	FTimerHandle RandomEventHandle;
	void BeginRandomEventTimer();
	void RandomEvent();
	
	void PopOffEvent(const EShipSection Category, const int Amount) const;
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void BeginFlight() {Execute_BeginFlightSequence(this);}
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void PopOffTester() const {PopOffEvent(EShipSection::Whole, 1);}
};
