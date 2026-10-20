// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShipRouter.h"
#include "Banananauts/Enums/EventType.h"
#include "FlightRouter.generated.h"

USTRUCT(BlueprintType)
struct FTurbulenceEvent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float Chance = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEventType Type = EEventType::All;
};

/**
 * Handles the route and flight of the ship.
 *
 * Last Edited:10/19/2026
 * Author: Julian Kroeger-Miller
 */
UCLASS()
class BANANANAUTS_API AFlightRouter : public AShipRouter
{
	GENERATED_BODY()

public:
	AFlightRouter();
	
	virtual void Success() override;
	virtual void Failure() override;
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default")
	UDataTable* LevelsDT;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Turbulence")
	TArray<FTurbulenceEvent> TurbulenceZone1Events;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Turbulence")
	TArray<FTurbulenceEvent> TurbulenceZone2Events;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Turbulence")
	TArray<FTurbulenceEvent> TurbulenceZone3Events;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Turbulence")
	TArray<FTurbulenceEvent> FinalStretchEvents;
	
	virtual void TriggerEvent(const FGameplayTag EventAction) override;
	
	virtual void BeginRoute() const override;
	
private:
	TSoftObjectPtr<UWorld> GetRandomLevel() const;

	void TriggerTurbulenceEvents(const TArray<FTurbulenceEvent>& Events) const;
};
