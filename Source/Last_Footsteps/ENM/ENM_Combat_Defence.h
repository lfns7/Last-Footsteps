// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ENM_BossManager.h"
#include "ENM_Combat_Defence.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAST_FOOTSTEPS_API UENM_Combat_Defence : public USceneComponent
{
    GENERATED_BODY()

public:
    UENM_Combat_Defence();

    void TryActivateDefence();

    bool bIsDefending;
};
