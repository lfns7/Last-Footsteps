// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Movement.h"
#include "CHR_Movement.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include <random>

// 기본값 설정
AENM_Movement::AENM_Movement()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsTurning = false;
	bIsStopped = false;
	bIsHolding = false;
}

// 게임 시작 시 호출
void AENM_Movement::BeginPlay()
{
	Super::BeginPlay();

	// 속도 설정


	// 충돌 이벤트 바인딩
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AENM_Movement::OnAttackItemOverlap);

	BossHPTemp = ENM_BossManager_Get_BOSSHP();
	TargetRotation = GetActorRotation(); // 처음 방향 저장
}

// 매 프레임마다 호출
void AENM_Movement::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	ACharacter* PL = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PL)
	{
		if (ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(PL))
		{
			if (FMath::Abs(FVector::Dist(FVector(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, 0), FVector(GetActorLocation().X, GetActorLocation().Y, 0))) <= 500.0f)
			{
				GetCharacterMovement()->MaxWalkSpeed = BossMoveSpeed / 10.0f;
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = BossMoveSpeed;
			}
		}
	}

	// 보스 체력 갱신
	if (BossHPTemp != ENM_BossManager_Get_BOSSHP())
	{
		BossHPTemp = ENM_BossManager_Get_BOSSHP();
	}


	// 회전 중이면 보간 회전 처리
	if (bIsTurning)
	{
		TurnToNextRotation(DeltaTime);
	}
	else
	{
		// 플랫폼 중심 계산 함수 호출
		FindPlatformCenter();

		// 중심으로 보간 이동
		CenterLerpToPlatform(DeltaTime);

		// 전방 이동 처리
		AddMovementInput(GetActorForwardVector(), 1.0f);
	}
}

// 플랫폼의 중심 좌표를 향해 부드럽게 이동
void AENM_Movement::CenterLerpToPlatform(float DeltaTime)
{
	if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 90.f, 1.f))
	{
		// X축 중심으로 보간
		float NewX = FMath::FInterpTo(GetActorLocation().X, CurrentPlatformCenter.X, DeltaTime, 7.5f);
		SetActorLocation(FVector(NewX, GetActorLocation().Y, GetActorLocation().Z));
	}
	else if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 0.f, 1.f) || FMath::IsNearlyEqual(GetActorRotation().Yaw, 180.f, 1.f))
	{
		// Y축 중심으로 보간
		float NewY = FMath::FInterpTo(GetActorLocation().Y, CurrentPlatformCenter.Y, DeltaTime, 7.5f);
		SetActorLocation(FVector(GetActorLocation().X, NewY, GetActorLocation().Z));
	}
}

// 바닥에 있는 플랫폼 액터의 중심 좌표 계산
void AENM_Movement::FindPlatformCenter()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 600.f); // 아래 방향 Raycast

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신 제외

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* Platform = Hit.GetActor();
		if (Platform)
		{
			CurrentPlatformCenter = Platform->GetActorLocation();
		}
	}
}

// 회전 처리 함수
void AENM_Movement::TurnToNextRotation(float DeltaTime)
{
	FRotator CurrentRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaTime, 10.0f);
	SetActorRotation(NewRot);

	// 회전이 거의 끝났다면 고정
	if (FMath::Abs(NewRot.Yaw - TargetRotation.Yaw) <= 3.0f)
	{
		SetActorRotation(TargetRotation);
		bIsTurning = false;
	}
}

// 공격 아이템 충돌 처리
void AENM_Movement::OnAttackItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("AttackItem"))
	{
		ApplyDamage(20.0f);
		bIsStopped = true;

		GetWorldTimerManager().SetTimer(HoldTimerHandle, this, &AENM_Movement::ResumeMovement, 2.0f, false);
		OtherActor->Destroy();
	}
}

// 멈춤 해제
void AENM_Movement::ResumeMovement()
{
	bIsStopped = false;
}

// 감속 해제
void AENM_Movement::ResetBossSpeed()
{
	bIsHolding = false;
}

// 데미지 처리
void AENM_Movement::OnDamaged()
{
	ApplyDamage(10.0f);
}

void AENM_Movement::CallDamagedSound()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 2);
	auto RandResult = dist(mt);

	switch (RandResult)
	{
		case 0:
		{
			USoundBase* Damaged_1 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Enemy/Damaged/Monster_Alien_Roar_Aggressive_1.Monster_Alien_Roar_Aggressive_1"));
			if (Damaged_1)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Damaged_1, GetActorLocation());
			}
			break;
		}

		case 1:
		{
			USoundBase* Damaged_2 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Enemy/Damaged/Monster_Alien_Roar_Aggressive_2.Monster_Alien_Roar_Aggressive_2"));
			if (Damaged_2)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Damaged_2, GetActorLocation());
			}
			break;
		}

		case 2:
		{
			USoundBase* Damaged_3 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Enemy/Damaged/Monster_Alien_Roar_Aggressive_3.Monster_Alien_Roar_Aggressive_3"));
			if (Damaged_3)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Damaged_3, GetActorLocation());
			}
			break;
		}
	}
}

void AENM_Movement::CallAttackSound()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 2);
	auto RandResult = dist(mt);

	switch (RandResult)
	{
	case 0:
	{
		USoundBase* Attack_1 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Enemy/Attack/Female_Monster_Attacking__1.Female_Monster_Attacking__1"));
		if (Attack_1)
		{
			UGameplayStatics::PlaySound2D(this, Attack_1);
		}
		break;
	}

	case 1:
	{
		USoundBase* Attack_2 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Enemy/Attack/Female_Monster_Attacking__2.Female_Monster_Attacking__2"));
		if (Attack_2)
		{
			UGameplayStatics::PlaySound2D(this, Attack_2);
		}
		break;
	}

	case 2:
	{
		USoundBase* Attack_3 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Enemy/Attack/Female_Monster_Attacking__3.Female_Monster_Attacking__3"));
		if (Attack_3)
		{
			UGameplayStatics::PlaySound2D(this, Attack_3);
		}
		break;
	}
	}
}


