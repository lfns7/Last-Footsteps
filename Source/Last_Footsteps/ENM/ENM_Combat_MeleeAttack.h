// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ENM_Combat_MeleeAttack.generated.h"

class UBoxComponent;
class ACharacter;
class UENM_Combat_isRange;
class ACHR_Movement;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAST_FOOTSTEPS_API UENM_Combat_MeleeAttack : public USceneComponent
{
    GENERATED_BODY()

public:
    UENM_Combat_MeleeAttack();

protected:
    virtual void BeginPlay() override;
    void PerformMeleeAttack();
    void ApplyDamageToPlayer(ACharacter* Player);

private:
    FTimerHandle MeleeTimerHandle;
    UBoxComponent* BottomRange;
    UBoxComponent* MiddleRange;

    ACharacter* PlayerCharacter;

    UENM_Combat_isRange* IsRangeComponent;
};
