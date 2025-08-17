// Fill out your copyright notice in the Description page of Project Settings.

#include "ENM_Combat_ThrowAttack.h"
#include "ENM_Combat_isRange.h"
#include "ENM_Movement.h"
#include "CHR_Movement.h"
#include "UI_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ENM_ThrowAttack_Projectile_Name.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/HUD.h"

UENM_Combat_ThrowAttack::UENM_Combat_ThrowAttack()
{
    PrimaryComponentTick.bCanEverTick = false;
    ProjectileLifeSpan = 5.0f;
    ThrowInterval = 15.0f;
    //ThrowDistance = 500.0f;
    LaneSpacing = 250.0f;
}

void UENM_Combat_ThrowAttack::BeginPlay()
{
    Super::BeginPlay();

    RangeChecker = GetOwner()->FindComponentByClass<UENM_Combat_isRange>();
   
    // BottomRange와 MiddleRange Box 찾기
    AActor* Owner = GetOwner();
    if (Owner)
    {
        TArray<UBoxComponent*> BoxComps;
        Owner->GetComponents<UBoxComponent>(BoxComps);

        for (UBoxComponent* Box : BoxComps)
        {
            if (Box->GetName().Contains(TEXT("Bottom Range")))
                BottomRange = Box;
            else if (Box->GetName().Contains(TEXT("Middle Range")))
                MiddleRange = Box;
        }
    }

    GetWorld()->GetTimerManager().SetTimer(ThrowTimerHandle, this, &UENM_Combat_ThrowAttack::PerformThrow, ThrowInterval, true);
}

void UENM_Combat_ThrowAttack::PerformThrow()
{
    if (!RangeChecker || RangeChecker->bPlayerInRange) return;

    TArray<int32> AllLanes = { 0, 1, 2 };
    int32 PlayerLane = 1; // 기본값: 중앙

    if (ACHR_Movement* PlayerChar = Cast<ACHR_Movement>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        PlayerLane = PlayerChar->CurrentLane;
    }

    // 플레이어 라인 + 랜덤 라인 설정
    TArray<int32> TargetLanes;
    TargetLanes.Add(PlayerLane);
    AllLanes.Remove(PlayerLane);
    TargetLanes.Add(AllLanes[FMath::RandRange(0, AllLanes.Num() - 1)]);

    // 2개의 라인으로 투사체 발사
    for (int32 LaneIdx : TargetLanes)
    {
        bool bUseBottom = FMath::RandBool();
        SpawnProjectileInLane(LaneIdx, bUseBottom);
    }
}

void UENM_Combat_ThrowAttack::SpawnProjectileInLane(int32 LaneIndex, bool bUseBottomBox)
{
    // 공격 전에 1초 동안 Warning UI 표시
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        TArray<UUserWidget*> FoundWidgets;
        FName WarningName;

        UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUI_PlayerHUD::StaticClass(), false);
        if (FoundWidgets.Num() > 0)
        {
            UUI_PlayerHUD* HUD = Cast<UUI_PlayerHUD>(FoundWidgets[0]);
            if (HUD)
            {

                if (LaneIndex == 0) // Left
                {
                    WarningName = bUseBottomBox ? "Throw_Left_Bottom" : "Throw_Left_Middle";
                }
                else if (LaneIndex == 1) // Center
                {
                    WarningName = bUseBottomBox ? "Throw_Center_Bottom" : "Throw_Center_Middle";
                }
                else if (LaneIndex == 2) // Right
                {
                    WarningName = bUseBottomBox ? "Throw_Right_Bottom" : "Throw_Right_Middle";
                }

                //HUD->ShowWarning(WarningName);
            }
        }

        // 투사체 발사는 1.5초 후에 실행
        FTimerHandle SpawnDelayHandle;
        TWeakObjectPtr<UENM_Combat_ThrowAttack> WeakThis(this);

        if (!WeakThis.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("[ThrowAttack] WeakThis is invalid!"));
            return;
        }

        UENM_Combat_ThrowAttack* Self = WeakThis.Get();

        if (!Self->ProjectileClass)
        {
            UE_LOG(LogTemp, Error, TEXT("[ThrowAttack] ProjectileClass is null!"));
            return;
        }

        if (!Self->BottomRange && !Self->MiddleRange)
        {
            UE_LOG(LogTemp, Error, TEXT("[ThrowAttack] Both BottomRange and MiddleRange are null!"));
            return;
        }

        AActor* BossActor = Self->GetOwner();
        if (!BossActor)
            return;

        UBoxComponent* LaunchBox = bUseBottomBox ? Self->BottomRange : Self->MiddleRange;
        FVector BoxLocation = LaunchBox->GetComponentLocation();

        float OffsetY = (LaneIndex - 1) * Self->LaneSpacing;
        FVector SpawnLocation;

        if (FMath::IsNearlyEqual(GetComponentRotation().Yaw, 90.f, 1.f))
        {
            SpawnLocation = BoxLocation + BossActor->GetActorRightVector() * OffsetY - FVector(0, 1000.f, 25.f);
        }

        else if (FMath::IsNearlyEqual(GetComponentRotation().Yaw, 0.f, 1.f))
        {
            SpawnLocation = BoxLocation + BossActor->GetActorRightVector() * OffsetY - FVector(1000.f, 0.f, 25.f);
        }

        else
        {
            SpawnLocation = BoxLocation + BossActor->GetActorRightVector() * OffsetY - FVector(-1000.f, 0.f, 25.f);
        }

        FVector Forward = BossActor->GetActorForwardVector();
        FRotator SpawnRotation = Forward.Rotation();

        FActorSpawnParameters Params;
        Params.Owner = BossActor;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* Projectile = Self->GetWorld()->SpawnActor<AActor>(Self->ProjectileClass, SpawnLocation, SpawnRotation, Params);
    
        if (!Projectile)
            return;

        if (Projectile)
        {
            //UE_LOG(LogTemp, Warning, TEXT("[1] Projectile Spawned: %s / WarningName: %s"), *Projectile->GetName(), *WarningName.ToString());

            // 생성한 투사체에 WarningName 저장
            if (AENM_ThrowAttack_Projectile_Name* Casted = Cast<AENM_ThrowAttack_Projectile_Name>(Projectile))
            {
                Casted->WarningName = WarningName;
                Casted->bIsInitialized = true;
                //UE_LOG(LogTemp, Warning, TEXT("[3] WarningName Set: %s on Projectile: %s"), *WarningName.ToString(), *Casted->GetName());
            }
        }

        AENM_Movement* Boss = Cast<AENM_Movement>(BossActor);
        if (Boss)
        {
            Boss->CallAttackSound();
        }

        if (Projectile)
        {
            if (UPrimitiveComponent* PCs = Projectile->FindComponentByClass<UPrimitiveComponent>())
            {
                PCs->SetGenerateOverlapEvents(true);
                PCs->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
                PCs->SetCollisionResponseToAllChannels(ECR_Ignore);

                PCs->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);

                //Sensor 박스 감지용
                PCs->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);

                // 바인딩: UENM_Combat_ThrowAttack::OnProjectileOverlap
                PCs->OnComponentBeginOverlap.AddDynamic(this, &UENM_Combat_ThrowAttack::OnProjectileOverlap);
            }

            if (UProjectileMovementComponent* MoveComp = Projectile->FindComponentByClass<UProjectileMovementComponent>())
            {
                MoveComp->Velocity = GetOwner()->GetActorForwardVector() * Projectile_Speed;
            }

            Projectile->SetLifeSpan(Self->ProjectileLifeSpan);
        }
    }
}

void UENM_Combat_ThrowAttack::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어가 아닌 경우 무시
    ACHR_Movement* Player = Cast<ACHR_Movement>(OtherActor);
    if (!Player)
        return;

    // 센서에 닿은 경우 무시
    if (OtherComp == Player->Sensor)
    {
        // 단순 경고 UI는 Sensor에서 처리되므로 여기선 무시
        return;
    }

    // HP 감소 처리
    if (Player->invincibility <= 0.f)
    {
        Player->CharacterDamaged();
    }

    // 카메라 흔들림이 설정되어 있다면 실행
    if (CameraShakeClass)
    {
        if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
        {
            PC->ClientStartCameraShake(CameraShakeClass);
        }
    }

    // 투사체 제거
    OverlappedComp->GetOwner()->Destroy();
}