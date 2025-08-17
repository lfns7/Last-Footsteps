// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/TextureDefines.h"
#include "ENM_BossManager.generated.h"

class UENM_Combat_isRange;
class UENM_Combat_MeleeAttack;
class UENM_Combat_Defence;
class UENM_Combat_Damaged;

UCLASS()
class LAST_FOOTSTEPS_API AENM_BossManager : public ACharacter
{
    GENERATED_BODY()

public:
    // �⺻�� ����
    AENM_BossManager();

    // ü��, �̵��ӵ�, ���ݷ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    float BossHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    float BossMoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
    float BossDamage;

    // �������� �޴� �Լ�
    virtual void ApplyDamage(float DamageAmount);

    // ��� ������Ʈ��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UENM_Combat_isRange* ENM_Combat_isRange;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UENM_Combat_MeleeAttack* ENM_Combat_MeleeAttack;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UENM_Combat_Defence* ENM_Combat_Defence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UENM_Combat_Damaged* ENM_Combat_Damaged;

    float ENM_BossManager_Get_BOSSHP();
};
