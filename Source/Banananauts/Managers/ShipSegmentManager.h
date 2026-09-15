// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Enums/BurnStatus.h"
#include "UObject/Object.h"
#include "ShipSegmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeatStatusChanged, EBurnStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuelEmpty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBananasEmpty, bool, Empty);

/**
 * Manages stats and the state of the ship segments.
 */
UCLASS()
class BANANANAUTS_API UShipSegmentManager : public UObject
{
	GENERATED_BODY()
	
public:
	UShipSegmentManager();
	
	UFUNCTION(BlueprintCallable)
	void ModifyHeat(float Delta);
	UFUNCTION(BlueprintCallable)
	void ModifyMaxHeat(float Delta);

	/**
	 * Event that broadcasts when heat status changes.
	 * @param NewStatus The new status of heat.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnHeatStatusChanged OnHeatStatusChanged;
	
	UFUNCTION(BlueprintCallable)
	void ModifyHeatAblation(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void ModifyMass(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void ModifyFuel(float Delta);

	/**
	 * Event that is broadcasted when fuel is empty.
	 */
	UPROPERTY(BlueprintAssignable)
	FFuelEmpty OnFuelEmpty;
	
	UFUNCTION(BlueprintCallable)
	void ModifyBananas(int Delta);
	
	/**
	 * Event that is broadcasted when banana empty status changes.
	 * @param Empty True if empty. False if not.
	 */
	UPROPERTY(BlueprintAssignable)
	FBananasEmpty OnBananasEmpty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Fuel")
	bool bHasFuel = false;
	
	UFUNCTION(Category = "Heat")
	float GetHeat() const { return Heat; }
	UFUNCTION(Category = "Heat")
	float GetMaxHeat() const { return MaxHeat; }
	UFUNCTION(Category = "Heat")
	EBurnStatus GetBurnStatus() const { return BurnStatus; }
	UFUNCTION(Category = "Heat Ablation")
	float GetHeatAblation() const { return HeatAblation; }
	UFUNCTION(Category = "Heat Ablation")
	float GetMaxHeatAblation() const { return MaxHeatAblation; }
	UFUNCTION(Category = "Mass")
	float GetMass() const { return Mass; }
	UFUNCTION(Category = "Mass")
	float GetMaxMass() const { return MaxMass; }
	UFUNCTION(Category = "Fuel")
	float GetFuel() const { return Fuel; }
	UFUNCTION(Category = "Fuel")
	float GetMaxFuel() const { return MaxFuel; }
	UFUNCTION(Category = "Bananas")
	float GetBananas() const { return Bananas; }
	UFUNCTION(Category = "Bananas")
	float GetMaxBananas() const { return MaxBananas; }
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float Heat = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float MaxHeat = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	EBurnStatus BurnStatus;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat Ablation", meta = (AllowPrivateAccess = "true"))
	float HeatAblation = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Heat Ablation", meta = (AllowPrivateAccess = "true"))
	float MaxHeatAblation = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mass", meta = (AllowPrivateAccess = "true"))
	float Mass = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mass", meta = (AllowPrivateAccess = "true"))
	float MaxMass = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Fuel", meta = (AllowPrivateAccess = "true"))
	float Fuel = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Fuel", meta = (AllowPrivateAccess = "true"))
	float MaxFuel = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Bananas", meta = (AllowPrivateAccess = "true"))
	int Bananas = 0;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Bananas", meta = (AllowPrivateAccess = "true"))
	int MaxBananas = 0;
};
