// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"


AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

