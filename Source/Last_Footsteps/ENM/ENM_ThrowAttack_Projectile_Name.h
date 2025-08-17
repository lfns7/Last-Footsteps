// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ENM_ThrowAttack_Projectile_Name.generated.h"

UCLASS()
class LAST_FOOTSTEPS_API AENM_ThrowAttack_Projectile_Name : public AActor
{
	GENERATED_BODY()

public:
	// UI 경고를 출력할 때 사용할 WarningName (예: Throw_Center_Middle)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName WarningName;

	UPROPERTY()
	TSet<UPrimitiveComponent*> CurrentSensorsOverlapping;

	UPROPERTY()
	bool bRearSensorDetected = false;

	UPROPERTY()
	float LastSensorEnterTime = -1.0f;

	bool bIsInitialized = false;
};
