// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShipSegmentManager.h"
#include "UObject/Object.h"
#include "ShipManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuelEmpty);

/**
 * Manages the ship's segments and systems. Also provides access points to vital ship stats and functions.
 * 
 * Last Edited: 9/15/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API AShipManager : public AActor
{
	GENERATED_BODY()
	
public:
	AShipManager();
	
	UFUNCTION(BlueprintCallable, Category = "Management")
	void InitiateFlight();
	
	UFUNCTION(BlueprintCallable, Category = "Stats|Mass")
	float GetShipMass();
	UFUNCTION(BlueprintCallable, Category = "Stats|Mass")
	float GetMaxShipMass();
	
	UFUNCTION(BlueprintCallable, Category = "Stats|Bananas")
	int GetBananaCount();
	UFUNCTION(BlueprintCallable, Category = "Stats|Bananas")
	int GetBananaCapacity();
	UFUNCTION(BlueprintCallable, Category = "Stats|Bananas")
	bool UseBananas(int Amount);
	UFUNCTION(BlueprintCallable, Category = "Stats|Bananas")
	bool AddBananas(int Amount);
	
	/**
	 * Event that is broadcasted when fuel is empty.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stats|Fuel")
	FFuelEmpty OnFuelEmpty;
	UFUNCTION(BlueprintCallable, Category = "Stats|Fuel")
	void ModifyFuel(float Delta);
	UFUNCTION(Category = "Stats|Fuel")
	float GetFuel() const { return Fuel; }
	UFUNCTION(Category = "Stats|Fuel")
	float GetMaxFuel() const { return MaxFuel; }
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = "Segments")
	TObjectPtr<UShipSegmentManager> Nose;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = "Segments")
	TObjectPtr<UShipSegmentManager> Body;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = "Segments")
	TObjectPtr<UShipSegmentManager> Tail;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Management")
	float TickRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Management")
	float BaseHeatGain;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Stats|Fuel", meta = (AllowPrivateAccess = "true"))
	float Fuel = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stats|Fuel", meta = (AllowPrivateAccess = "true"))
	float MaxFuel = 0.0f;
	
	void TickSystems();
	FTimerHandle TickHandle;
	
	UPROPERTY()
	TArray<UShipSegmentManager*> Segments;
	
	void UpdateHeat() const;
};
