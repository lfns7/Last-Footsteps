// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ENM_Combat_Damaged.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAST_FOOTSTEPS_API UENM_Combat_Damaged : public USceneComponent
{
    GENERATED_BODY()

public:
    void ApplyDamage(float DamageValue, float SlowValue);

    FTimerHandle TimerHandle;

    UFUNCTION()
    void RollBackSpeed();
};