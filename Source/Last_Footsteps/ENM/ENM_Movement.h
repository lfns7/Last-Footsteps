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
    // �⺻�� ����
    AENM_Movement();

protected:
    // ���� ���� �� ȣ��
    virtual void BeginPlay() override;

public:
    // �� ������ ȣ��
    virtual void Tick(float DeltaTime) override;

    // �ǰ� ó��
    void OnDamaged();

    // ���� ȸ���� Ÿ�̸� �Լ�
    void ResetBossSpeed();

public:
    class AAIController* AIController;

    //���� Ÿ�̸�
    FTimerHandle SpeedResetTimerHandle;
    //���� Ÿ�̸�
    FTimerHandle HoldTimerHandle;
    FVector CurrentPlatformCenter;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
    FRotator TargetRotation;


    //���� ����
    bool bIsTurning;
    bool bIsHolding;
    bool bIsStopped;

    // ȸ�� ó��
    void TurnToNextRotation(float DeltaTime);

    // �÷��� �߽� ��ǥ ���
    void FindPlatformCenter();

    // �߽����� ���� �̵�
    void CenterLerpToPlatform(float DeltaTime);



    // ���� ���� �Ǵܿ� �Ÿ�
    float DistanceToPlayer;
    bool bIsInMeleeRange;

    // ������ �浹 ó��
    UFUNCTION()
    void OnAttackItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    //���̵� �Լ�
    void ResumeMovement();

public:
    float BossHPTemp;


    // �Ҹ�
public:
    void CallDamagedSound();
    void CallAttackSound();
};
