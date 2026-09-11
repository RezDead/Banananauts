// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Banananauts/Enums/BurnStatus.h"
#include "UObject/Object.h"
#include "ShipSegmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeatStatusChanged, EBurnStatus, NewStatus)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuelEmpty)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBananasEmpty, bool, Empty)

/**
 * Manages stats and the state of the ship segments.
 */
UCLASS()
class BANANANAUTS_API UShipSegmentManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ModifyTotalHeat(float Delta);
	UFUNCTION(BlueprintCallable)
	void ModifyMaxTotalHeat(float Delta);
	
	UFUNCTION(BlueprintListenable)
	FOnHeatStatusChanged OnHeatStatusChanged;
	
	UFUNCTION(BlueprintCallable)
	void ModifyHeatAblation(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void ModifyMass(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void ModifyFuel(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void ModifyBananas(int Delta);
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float TotalHeat = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	float MaxTotalHeat = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat", meta = (AllowPrivateAccess = "true"))
	EBurnStatus BurnStatus;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat Ablation", meta = (AllowPrivateAccess = "true"))
	float HeatAblation = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Heat Ablation", meta = (AllowPrivateAccess = "true"))
	float MaxHeatAblation = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mass", meta = (AllowPrivateAccess = "true"))
	float Mass = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mass", meta = (AllowPrivateAccess = "true"))
	float MaxMass = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Fuel", meta = (AllowPrivateAccess = "true"))
	float Fuel = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Fuel", meta = (AllowPrivateAccess = "true"))
	float MaxFuel = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Bananas", meta = (AllowPrivateAccess = "true"))
	int Bananas = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Bananas", meta = (AllowPrivateAccess = "true"))
	int MaxBananas = 0;
};
