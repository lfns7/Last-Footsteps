// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Combat_Damaged.h"
#include "ENM_BossManager.h"
#include "ENM_Combat_Defence.h"
#include "CHR_Movement.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "Kismet/GameplayStatics.h"

void UENM_Combat_Damaged::ApplyDamage(float DamageValue, float SlowValue)
{
    AENM_BossManager* Boss = Cast<AENM_BossManager>(GetOwner());
    if (!Boss)
        return;

    ACharacter* PL = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PL)
    {
        if (ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(PL))
        {
            if (PlayerCharacter->bPowerUp)
            {
                DamageValue *= 2.0f;
                SlowValue *= 2.0f;
                PlayerCharacter->bPowerUp = false;
            }
        }
    }


    UENM_Combat_Defence* DefenceComponent = Boss->FindComponentByClass<UENM_Combat_Defence>();
    if (DefenceComponent)
    {
        DefenceComponent->TryActivateDefence(); // 이번 공격에서 방어 여부 판정

        if (DefenceComponent->bIsDefending)
        {
            DamageValue *= 0.5f; // 데미지 절반
            UE_LOG(LogTemp, Warning, TEXT("Boss defended. Damage halved: %.1f"), DamageValue);
            DefenceComponent->bIsDefending = false; // 상태 초기화
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Boss failed to defend. Full damage applied: %.1f"), DamageValue);
        }
    }

    // 보스 HP 감소
    Boss->BossHP -= DamageValue;

    // 감속 처리 부분
    Boss->BossMoveSpeed -= SlowValue;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UENM_Combat_Damaged::RollBackSpeed, 1.5f, false);

    UE_LOG(LogTemp, Warning, TEXT("Boss remaining HP: %.1f"), Boss->BossHP);

    if (Boss->BossHP <= 0.0f)
    {
        ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(UGameplayStatics::GetGameMode(this));
        GM->PlayerVictory();
    }
}

// 1초 뒤 이동속도 복구
void UENM_Combat_Damaged::RollBackSpeed()
{
    AENM_BossManager* Boss = Cast<AENM_BossManager>(GetOwner());
    Boss->BossMoveSpeed = 2480.0f;
}

