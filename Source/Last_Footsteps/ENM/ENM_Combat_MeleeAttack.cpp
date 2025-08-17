// Fill out your copyright notice in the Description page of Project Settings.

#include "ENM_Combat_MeleeAttack.h"
#include "ENM_Movement.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "CHR_Movement.h"
#include "UI_PlayerHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ENM_Combat_isRange.h"

// 생성자: 틱 비활성화
UENM_Combat_MeleeAttack::UENM_Combat_MeleeAttack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// 게임 시작 시 콜리전 박스 초기화, 범위 컴포넌트 연결, 타이머 시작
void UENM_Combat_MeleeAttack::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 오너 액터에 있는 BoxComponent 수집
    TArray<UBoxComponent*> BoxComponents;
    Owner->GetComponents<UBoxComponent>(BoxComponents);

    // 이름으로 하단/중간 콜리전 박스 구분
    for (UBoxComponent* Box : BoxComponents)
    {
        if (Box->GetName().Contains(TEXT("Bottom Range")))
        {
            BottomRange = Box;
        }
        else if (Box->GetName().Contains(TEXT("Middle Range")))
        {
            MiddleRange = Box;
        }
    }

    // 플레이어 캐릭터 참조 저장
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // isRange 컴포넌트 참조 저장
    IsRangeComponent = Owner->FindComponentByClass<UENM_Combat_isRange>();


    // 12초마다 PerformMeleeAttack 함수 호출하는 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(MeleeTimerHandle, this, &UENM_Combat_MeleeAttack::PerformMeleeAttack, 12.0f, true);
}

// 일정 시간마다 실행되는 근접 공격 처리 함수
void UENM_Combat_MeleeAttack::PerformMeleeAttack()
{
    // 기본 체크 (널 포인터 또는 범위 밖이면 실행 중지)
    if (!PlayerCharacter || !BottomRange || !MiddleRange || !IsRangeComponent) return;
    if (!IsRangeComponent->bPlayerInRange) return;
    
    // Bottom 또는 Middle 중 하나를 무작위로 선택
    UBoxComponent* TargetBox = FMath::RandBool() ? BottomRange : MiddleRange;
    FName WarningName = (TargetBox == BottomRange) ? "Melee_Bottom" : "Melee_Middle";

    // UI 표시
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        TArray<UUserWidget*> FoundWidgets;
        UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUI_PlayerHUD::StaticClass(), false);

        if (FoundWidgets.Num() > 0)
        {
            UUI_PlayerHUD* HUD = Cast<UUI_PlayerHUD>(FoundWidgets[0]);
            if (HUD)
            {
                // 로그: 선택된 범위 출력
                UE_LOG(LogTemp, Warning, TEXT("[MeleeAttack] Selected Range: %s"), *TargetBox->GetName());

                HUD->ShowWarning(WarningName);
            }
        }
    }

    // 1초 후에 데미지 처리
    FTimerHandle DamageDelayHandle;
    TWeakObjectPtr<UENM_Combat_MeleeAttack> WeakThis(this);

    GetWorld()->GetTimerManager().SetTimer(
        DamageDelayHandle,
        FTimerDelegate::CreateLambda([WeakThis, TargetBox]()
            {
                if (!WeakThis.IsValid())
                {
                    UE_LOG(LogTemp, Error, TEXT("[MeleeAttack] WeakThis is invalid!"));
                    return;
                }

                UENM_Combat_MeleeAttack* Self = WeakThis.Get();

                if (TargetBox && Self->PlayerCharacter && TargetBox->IsOverlappingActor(Self->PlayerCharacter))
                {
                    UE_LOG(LogTemp, Warning, TEXT("[MeleeAttack] Player is overlapping with %s - Applying damage"), *TargetBox->GetName());

                    Self->ApplyDamageToPlayer(Self->PlayerCharacter);
                }
            }),
        1.0f,
        false
    );


    AENM_Movement* Boss = Cast<AENM_Movement>(GetOwner());
    if (Boss)
    {
        Boss->CallAttackSound();
    }
}

// 플레이어에게 넉백 데미지를 적용하는 함수
void UENM_Combat_MeleeAttack::ApplyDamageToPlayer(ACharacter* Player)
{
    if (!Player)
        return;

    // 보스로부터 플레이어 방향의 벡터 계산
    ACHR_Movement* CHR_Player = Cast<ACHR_Movement>(Player);
    if (CHR_Player && CHR_Player->invincibility <= 0.f)
    {
        CHR_Player->CharacterDamaged();
        

        // 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("Melee attack launched player!"));
    }
}