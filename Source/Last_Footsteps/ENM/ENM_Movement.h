// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ENM_BossManager.h"
#include "Components/PrimitiveComponent.h"
#include "ENM_Movement.generated.h"

UCLASS()
class LAST_FOOTSTEPS_API AENM_Movement : public AENM_BossManager
{
    GENERATED_BODY()

public:
    // 기본값 설정
    AENM_Movement();

protected:
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

public:
    // 매 프레임 호출
    virtual void Tick(float DeltaTime) override;

    // 피격 처리
    void OnDamaged();

    // 감속 회복용 타이머 함수
    void ResetBossSpeed();

public:
    class AAIController* AIController;

    //감속 타이머
    FTimerHandle SpeedResetTimerHandle;
    //정지 타이머
    FTimerHandle HoldTimerHandle;
    FVector CurrentPlatformCenter;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
    FRotator TargetRotation;


    //상태 변수
    bool bIsTurning;
    bool bIsHolding;
    bool bIsStopped;

    // 회전 처리
    void TurnToNextRotation(float DeltaTime);

    // 플랫폼 중심 좌표 계산
    void FindPlatformCenter();

    // 중심으로 보간 이동
    void CenterLerpToPlatform(float DeltaTime);



    // 공격 범위 판단용 거리
    float DistanceToPlayer;
    bool bIsInMeleeRange;

    // 아이템 충돌 처리
    UFUNCTION()
    void OnAttackItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    //재이동 함수
    void ResumeMovement();

public:
    float BossHPTemp;


    // 소리
public:
    void CallDamagedSound();
    void CallAttackSound();
};
