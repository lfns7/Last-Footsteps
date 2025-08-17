// Fill out your copyright notice in the Description page of Project Settings.

#include "ENM_Combat_isRange.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// 생성자: 컴포넌트의 기본값 초기화
UENM_Combat_isRange::UENM_Combat_isRange()
{
    PrimaryComponentTick.bCanEverTick = true; // 틱 활성화
    bPlayerInRange = false;                    // 기본적으로 플레이어는 범위 밖
    CachedPlayer = nullptr;                    // 캐시된 플레이어 초기화
}

// 게임 시작 시 콜리전 박스를 찾아서 오버랩 이벤트 바인딩
void UENM_Combat_isRange::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 소유 액터의 BoxComponent들을 가져옴
    TArray<UBoxComponent*> BoxComponents;
    Owner->GetComponents<UBoxComponent>(BoxComponents);

    // 이름 기준으로 각 범위 구분
    for (UBoxComponent* Box : BoxComponents)
    {
        if (Box->GetName().Contains(TEXT("Middle Range")))
        {
            MiddleRange = Box;
        }
        else if (Box->GetName().Contains(TEXT("Bottom Range")))
        {
            BottomRange = Box;
        }
    }

    // 처음 한 번만 Player 찾아서 캐싱
    if (!CachedPlayer)
    {
        ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (Player) CachedPlayer = Player;
    }

    // 로그 확인용 오버랩 이벤트 바인딩
    if (MiddleRange)
    {
        MiddleRange->OnComponentBeginOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeBegin);
        MiddleRange->OnComponentEndOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeEnd);
    }
    if (BottomRange)
    {
        BottomRange->OnComponentBeginOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeBegin);
        BottomRange->OnComponentEndOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeEnd);
    }
}

// 매 프레임마다 범위 안에 있는지 직접 검사
void UENM_Combat_isRange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!CachedPlayer && !BottomRange && !MiddleRange) return;

    // 각각의 박스와 플레이어가 겹쳐있는지 체크
    bool bIsInBottom = BottomRange && BottomRange->IsOverlappingActor(CachedPlayer);
    bool bIsInMiddle = MiddleRange && MiddleRange->IsOverlappingActor(CachedPlayer);

    // 하나라도 겹쳐있으면 true
    bPlayerInRange = bIsInBottom || bIsInMiddle;
}

// 플레이어가 콜리전 범위에 들어왔을 때 호출
void UENM_Combat_isRange::OnAnyRangeBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Player = Cast<ACharacter>(OtherActor);
    if (Player)
    {

        // 어떤 콜리전 박스인지 로그 출력
        FString ColliderName = OverlappedComponent ? OverlappedComponent->GetName() : TEXT("Unknown");
        UE_LOG(LogTemp, Warning, TEXT("Player entered combat range: %s"), *ColliderName);
    }
}

// 플레이어가 콜리전 범위를 벗어났을 때 호출
void UENM_Combat_isRange::OnAnyRangeEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* Player = Cast<ACharacter>(OtherActor);
    if (Player)
    {

        // 어떤 콜리전 박스인지 로그 출력
        FString ColliderName = OverlappedComponent ? OverlappedComponent->GetName() : TEXT("Unknown");
        UE_LOG(LogTemp, Warning, TEXT("Player exited combat range: %s"), *ColliderName);
    }
}