// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "AbilitySystemComponent.h"


AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (!ItemInfoDT)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoDT is not set in %s"), *GetName());
		return;
	}

	const FString ContextString((TEXT("Looking up item info in class %s"), *GetName()));
	
	FItemInfo* ItemRow = ItemInfoDT->FindRow<FItemInfo>(*GetName(), ContextString);
	
	if (!ItemRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoDT does not contain a row for %s"), *GetName());
		return;
	}
	
	//Need work here
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

