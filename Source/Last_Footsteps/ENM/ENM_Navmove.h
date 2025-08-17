// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "ENM_Navmove.generated.h"

UCLASS()
class LAST_FOOTSTEPS_API AENM_Navmove : public AActor
{
    GENERATED_BODY()

public:
    AENM_Navmove();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    // NavMeshBoundsVolume을 플레이어 위치로 이동
    void UpdateNavMeshLocation();
};
