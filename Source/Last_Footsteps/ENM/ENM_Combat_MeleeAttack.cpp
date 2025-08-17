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

// ������: ƽ ��Ȱ��ȭ
UENM_Combat_MeleeAttack::UENM_Combat_MeleeAttack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// ���� ���� �� �ݸ��� �ڽ� �ʱ�ȭ, ���� ������Ʈ ����, Ÿ�̸� ����
void UENM_Combat_MeleeAttack::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // ���� ���Ϳ� �ִ� BoxComponent ����
    TArray<UBoxComponent*> BoxComponents;
    Owner->GetComponents<UBoxComponent>(BoxComponents);

    // �̸����� �ϴ�/�߰� �ݸ��� �ڽ� ����
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

    // �÷��̾� ĳ���� ���� ����
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // isRange ������Ʈ ���� ����
    IsRangeComponent = Owner->FindComponentByClass<UENM_Combat_isRange>();


    // 12�ʸ��� PerformMeleeAttack �Լ� ȣ���ϴ� Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(MeleeTimerHandle, this, &UENM_Combat_MeleeAttack::PerformMeleeAttack, 12.0f, true);
}

// ���� �ð����� ����Ǵ� ���� ���� ó�� �Լ�
void UENM_Combat_MeleeAttack::PerformMeleeAttack()
{
    // �⺻ üũ (�� ������ �Ǵ� ���� ���̸� ���� ����)
    if (!PlayerCharacter || !BottomRange || !MiddleRange || !IsRangeComponent) return;
    if (!IsRangeComponent->bPlayerInRange) return;
    
    // Bottom �Ǵ� Middle �� �ϳ��� �������� ����
    UBoxComponent* TargetBox = FMath::RandBool() ? BottomRange : MiddleRange;
    FName WarningName = (TargetBox == BottomRange) ? "Melee_Bottom" : "Melee_Middle";

    // UI ǥ��
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        TArray<UUserWidget*> FoundWidgets;
        UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUI_PlayerHUD::StaticClass(), false);

        if (FoundWidgets.Num() > 0)
        {
            UUI_PlayerHUD* HUD = Cast<UUI_PlayerHUD>(FoundWidgets[0]);
            if (HUD)
            {
                // �α�: ���õ� ���� ���
                UE_LOG(LogTemp, Warning, TEXT("[MeleeAttack] Selected Range: %s"), *TargetBox->GetName());

                HUD->ShowWarning(WarningName);
            }
        }
    }

    // 1�� �Ŀ� ������ ó��
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

// �÷��̾�� �˹� �������� �����ϴ� �Լ�
void UENM_Combat_MeleeAttack::ApplyDamageToPlayer(ACharacter* Player)
{
    if (!Player)
        return;

    // �����κ��� �÷��̾� ������ ���� ���
    ACHR_Movement* CHR_Player = Cast<ACHR_Movement>(Player);
    if (CHR_Player && CHR_Player->invincibility <= 0.f)
    {
        CHR_Player->CharacterDamaged();
        

        // �α� ���
        UE_LOG(LogTemp, Warning, TEXT("Melee attack launched player!"));
    }
}