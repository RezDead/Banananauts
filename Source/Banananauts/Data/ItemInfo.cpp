// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"

// Sets default values
AItemInfo::AItemInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemInfo::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Working successfully"));
}

// Called every frame
void AItemInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

