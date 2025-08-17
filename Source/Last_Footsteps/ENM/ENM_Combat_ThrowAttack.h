// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ENM_Combat_ThrowAttack.generated.h"

class UENM_Combat_isRange;
class UCameraShakeBase;
class ACHR_Movement;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAST_FOOTSTEPS_API UENM_Combat_ThrowAttack : public USceneComponent
{
    GENERATED_BODY()

public:
    UENM_Combat_ThrowAttack();

protected:
    // ���� ���� �� ȣ��
    virtual void BeginPlay() override;

    // ���� �ֱ�� ����ü�� ������ �Լ�
    void PerformThrow();

    // Ư�� ���ο� ����ü�� �����ϴ� �Լ�
    void SpawnProjectileInLane(int32 LaneIndex, bool bUseBottomBox);

    // ����ü�� �÷��̾ �浹���� ���� ó��
    UFUNCTION()
    void OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    // �÷��̾ ������ �ִ��� üũ�ϴ� ������Ʈ
    UENM_Combat_isRange* RangeChecker;

    // �߻翡 ����� �ڽ� �ݸ��� (���� ��ġ ����)
    UBoxComponent* BottomRange;
    UBoxComponent* MiddleRange;

    // ����ü Ŭ����
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    TSubclassOf<AActor> ProjectileClass;

    // ����ü ����
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float ProjectileLifeSpan;

    // ���� �ֱ�
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float ThrowInterval;

    // ���� �Ÿ� (��� ����, �ɼ� ����)
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float ThrowDistance;

    // �� ���� ����
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float LaneSpacing;

    // ����ü �ӵ�
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float Projectile_Speed;

    // ī�޶� ��鸲 Ŭ����
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    // Ÿ�̸� �ڵ�
    FTimerHandle ThrowTimerHandle;
};
