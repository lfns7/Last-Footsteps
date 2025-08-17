// Fill out your copyright notice in the Description page of Project Settings.


#include "ITEM/ITEM_ItemBase.h"

// Sets default values
AITEM_ItemBase::AITEM_ItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AITEM_ItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AITEM_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(FRotator(0, GetActorRotation().Yaw + 1.0f, 0));
}