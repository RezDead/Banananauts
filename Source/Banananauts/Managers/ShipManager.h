// Banananauts© 2026 by Monkey Business. Bananauts is a student project and is provided entirely not-for-profit. Bananauts uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Unreal® Engine, Copyright 1998 – 2026, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShipSegmentManager.h"
#include "UObject/Object.h"
#include "ShipManager.generated.h"

/**
 * 
 */
UCLASS()
class BANANANAUTS_API AShipManager : public AActor
{
	GENERATED_BODY()
	
public:
	AShipManager();
	
	UFUNCTION(BlueprintCallable, Category = "Management")
	void InitiateFlight();
	
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
	void TickSystems();
	FTimerHandle TickHandle;
	
	void UpdateHeat() const;
	static float CalculateHeatMult(UShipSegmentManager* Segment);
	
};
