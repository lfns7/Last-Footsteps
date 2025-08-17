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
    // 생성자: 컴포넌트의 기본값을 설정
    UENM_Combat_isRange();

protected:
    // 게임 시작 시 호출되는 함수 (컴포넌트 초기화용)
    virtual void BeginPlay() override;

    // 매 프레임마다 실행되는 함수 (플레이어가 범위 내에 있는지 검사)
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 플레이어가 콜리전 영역(BottomRange 또는 MiddleRange)에 들어왔을 때 호출되는 함수
    UFUNCTION()
    void OnAnyRangeBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 플레이어가 콜리전 영역을 벗어났을 때 호출되는 함수
    UFUNCTION()
    void OnAnyRangeEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    // 플레이어가 범위 내에 있는지를 나타내는 플래그
    bool bPlayerInRange;

private:
    // 현재 범위 내에 있는 플레이어를 저장하는 변수
    ACharacter* CachedPlayer;

    // 하단 범위를 나타내는 충돌 박스
    UBoxComponent* BottomRange;

    // 중간 범위를 나타내는 충돌 박스
    UBoxComponent* MiddleRange;
};