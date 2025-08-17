// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ENM_Combat_isRange.generated.h"

class UBoxComponent;
class ACharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LAST_FOOTSTEPS_API UENM_Combat_isRange : public USceneComponent
{
    GENERATED_BODY()

public:
    // ������: ������Ʈ�� �⺻���� ����
    UENM_Combat_isRange();

protected:
    // ���� ���� �� ȣ��Ǵ� �Լ� (������Ʈ �ʱ�ȭ��)
    virtual void BeginPlay() override;

    // �� �����Ӹ��� ����Ǵ� �Լ� (�÷��̾ ���� ���� �ִ��� �˻�)
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // �÷��̾ �ݸ��� ����(BottomRange �Ǵ� MiddleRange)�� ������ �� ȣ��Ǵ� �Լ�
    UFUNCTION()
    void OnAnyRangeBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // �÷��̾ �ݸ��� ������ ����� �� ȣ��Ǵ� �Լ�
    UFUNCTION()
    void OnAnyRangeEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    // �÷��̾ ���� ���� �ִ����� ��Ÿ���� �÷���
    bool bPlayerInRange;

private:
    // ���� ���� ���� �ִ� �÷��̾ �����ϴ� ����
    ACharacter* CachedPlayer;

    // �ϴ� ������ ��Ÿ���� �浹 �ڽ�
    UBoxComponent* BottomRange;

    // �߰� ������ ��Ÿ���� �浹 �ڽ�
    UBoxComponent* MiddleRange;
};