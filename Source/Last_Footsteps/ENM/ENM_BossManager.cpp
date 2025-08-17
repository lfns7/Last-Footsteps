// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_BossManager.h"
#include "ENM_Combat_isRange.h"
#include "ENM_Combat_MeleeAttack.h"
#include "ENM_Combat_Defence.h"
#include "ENM_Combat_Damaged.h"



// �⺻�� ����
AENM_BossManager::AENM_BossManager()
{
    PrimaryActorTick.bCanEverTick = true;

    BossHP = 1000.0f;
    BossMoveSpeed = 2475.0f;
    //BossDamage = 20.0f;

    // ��� ������Ʈ ���� �� ����
    ENM_Combat_isRange = CreateDefaultSubobject<UENM_Combat_isRange>(TEXT("ENM_Combat_isRange"));
    ENM_Combat_isRange->SetupAttachment(RootComponent);

    ENM_Combat_MeleeAttack = CreateDefaultSubobject<UENM_Combat_MeleeAttack>(TEXT("ENM_Combat_MeleeAttack"));
    ENM_Combat_MeleeAttack->SetupAttachment(RootComponent);

    ENM_Combat_Defence = CreateDefaultSubobject<UENM_Combat_Defence>(TEXT("ENM_Combat_Defence"));
    ENM_Combat_Defence->SetupAttachment(RootComponent);

    ENM_Combat_Damaged = CreateDefaultSubobject<UENM_Combat_Damaged>(TEXT("ENM_Combat_Damaged"));
    ENM_Combat_Damaged->SetupAttachment(RootComponent);
}

// �������� �޴� �Լ�
void AENM_BossManager::ApplyDamage(float DamageAmount)
{
    BossHP -= DamageAmount;
}

float AENM_BossManager::ENM_BossManager_Get_BOSSHP()
{
    return BossHP;
}


