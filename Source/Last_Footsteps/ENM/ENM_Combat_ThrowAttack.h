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
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

    // 일정 주기로 투사체를 던지는 함수
    void PerformThrow();

    // 특정 레인에 투사체를 생성하는 함수
    void SpawnProjectileInLane(int32 LaneIndex, bool bUseBottomBox);

    // 투사체가 플레이어에 충돌했을 때의 처리
    UFUNCTION()
    void OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    // 플레이어가 범위에 있는지 체크하는 컴포넌트
    UENM_Combat_isRange* RangeChecker;

    // 발사에 사용할 박스 콜리전 (생성 위치 기준)
    UBoxComponent* BottomRange;
    UBoxComponent* MiddleRange;

    // 투사체 클래스
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    TSubclassOf<AActor> ProjectileClass;

    // 투사체 수명
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float ProjectileLifeSpan;

    // 투사 주기
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float ThrowInterval;

    // 투사 거리 (사용 안함, 옵션 보관)
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float ThrowDistance;

    // 각 레인 간격
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float LaneSpacing;

    // 투사체 속도
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    float Projectile_Speed;

    // 카메라 흔들림 클래스
    UPROPERTY(EditAnywhere, Category = "Throw Attack")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    // 타이머 핸들
    FTimerHandle ThrowTimerHandle;
};
