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
 * Last Edited: 10/15/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS(PrioritizeCategories="Default Default|Stats Default|Segments")
class BANANANAUTS_API AShipManager : public AActor, public IAbilitySystemInterface, public IShipCommunication,
                                     public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	AShipManager();
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutContainer) const override
	{ OutContainer = GameplayTags; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	virtual void BeginLaunchSequence_Implementation() override;
	virtual void BeginFlightSequence_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	bool UseBananas(int Amount) const;
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	bool AddBananas(int Amount) const;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
	/*
	 * Blueprint Implemented Funcs
	 */
	
	UFUNCTION(BlueprintNativeEvent, Category = "Default|Flight")
	void LaunchFailed();
	
	/*
	 * Actor Components
	 */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
    UAbilitySystemComponent* AbilitySystemComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS")
    UShipAttributes* Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGameplayTagContainer GameplayTags;
	
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
	
	UPROPERTY()
	TMap<EShipSection, AShipSegmentManager*> Segments;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;
	
	/*
	 * Stats/Default Variables
	 */
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	UDataTable* LevelsDT;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats")
	float MaxMass = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Stats")
	float MaxFuel = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Launch")
	bool bIsLaunching = false;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Launch", meta = (ToolTip = "The duration is seconds a launch takes"))
	float LaunchDuration;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Launch")
	float LaunchDelay;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight")
	bool bIsFlying = false;
	UPROPERTY(BlueprintReadOnly, Category = "Default|Flight")
	TWeakObjectPtr<AShipRouter> ShipRouter = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight",
		meta = (AllowPrivateAccess = "true", ToolTip = "The rate at which gallons of fuel is consumed per minute"))
	float FuelBurnRate = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight")
	float MinFlightTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Flight")
	float FlightLinearGrowthRate = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default|Stats")
	FFuelComposition FuelComposition;
	
	void SetupAttributeListeners();
	FDelegateHandle FuelListener;
	TMap<EShipSection, FDelegateHandle> HeatListeners;
	TMap<EShipSection, FDelegateHandle> StabilityListeners;
	
	void OnFuelChanged(const FOnAttributeChangeData& Data);
	void OnHeatChanged(const FOnAttributeChangeData& Data);
	void OnStabilityChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	int GetBananaCount();
	UFUNCTION(BlueprintCallable, Category = "Bananas")
	int GetBananaCapacity() const;
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetHeat() const;
	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetMaxHeat() const;
	UFUNCTION(BlueprintCallable, Category = "Stability")
	float GetStability() const;
	UFUNCTION(BlueprintCallable, Category = "Stability")
	float GetMaxStability() const;
	
	
	/*
	 * Initiation Stuff
	 */
	
	void InitSegments();
	void InitAttributes() const;
	void InitShipRouter();
	
	/*
	 * Systems
	 */
	
	void TickSystems();
	FTimerHandle TickHandle;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (ToolTip = "The rate in seconds at which ship systems tick"))
	float SystemTickRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Stats", meta = (ToolTip = "The base amount of heat gained per tick before any modifiers"))
	float BaseHeatGainPerTick;
	
	void UpdateHeat() const;
	void ConsumeFuel(const float& DeltaTime) const;
	
	/*
	 * Win/Fail
	 */
	
	UFUNCTION()
	void Success();
	UFUNCTION()
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
	void OnClearToFail();
	void OnWarningToClear();
	void OnWarningToFail();
	void OnFailToClear();
	void OnFailToWarning();
	
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	EShipStatus Status;
	
	void SetStatus(EShipStatus NewStatus);
	void UpdateStatus();
	bool CheckAttributeStatus() const;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Status")
	float StabilityThreshold;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Status")
	float FuelThreshold;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Status")
	float HeatThreshold;
	
	/*
	 * Events
	 */
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events",
		meta = (ToolTip = "Multipler for event timer when status is clear."))
	float ClearEventFrequency;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events",
		meta = (ToolTip = "Multipler for event timer when status is warning."))
	float WarningEventFrequency;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinFirstEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxFirstEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MinRandBetweenEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float MaxRandBetweenEvent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float ToWarningEventDelay;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Random Events")
	float ToClearEventDelay;
	float CurrEventTimerDuration = 0.0f;
	
	
	FTimerHandle RandomEventHandle;
	void BeginRandomEvents();
	void StartRandomEventTimer();
	void FirstEvent();
	void RandomEvent();
	
	void UpdateEventTimerRate(const float NewRate);
	
	void PopOffEvent(const EShipSection Category, const int Amount) const;
	
	bool bRandomEvents;
	bool bForcedEvent;
	
	/*
	 * Testing
	 */
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void BeginFlight() {Execute_BeginFlightSequence(this);}
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void PopOffTester() const {PopOffEvent(EShipSection::Whole, 1);}
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void StartRandomEvent(){{BeginRandomEvents();}};
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void SetStateClear() {SetStatus(EShipStatus::Clear);}
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void SetStateWarning() {SetStatus(EShipStatus::Warning);}
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Testing")
	void SetStateFail() {SetStatus(EShipStatus::Fail);}
	
};
