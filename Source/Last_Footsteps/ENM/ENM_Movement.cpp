// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Movement.h"
#include "CHR_Movement.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include <random>

// �⺻�� ����
AENM_Movement::AENM_Movement()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsTurning = false;
	bIsStopped = false;
	bIsHolding = false;
}

// ���� ���� �� ȣ��
void AENM_Movement::BeginPlay()
{
	Super::BeginPlay();

	// �ӵ� ����


	// �浹 �̺�Ʈ ���ε�
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AENM_Movement::OnAttackItemOverlap);

	BossHPTemp = ENM_BossManager_Get_BOSSHP();
	TargetRotation = GetActorRotation(); // ó�� ���� ����
}

// �� �����Ӹ��� ȣ��
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

	// ���� ü�� ����
	if (BossHPTemp != ENM_BossManager_Get_BOSSHP())
	{
		BossHPTemp = ENM_BossManager_Get_BOSSHP();
	}


	// ȸ�� ���̸� ���� ȸ�� ó��
	if (bIsTurning)
	{
		TurnToNextRotation(DeltaTime);
	}
	else
	{
		// �÷��� �߽� ��� �Լ� ȣ��
		FindPlatformCenter();

		// �߽����� ���� �̵�
		CenterLerpToPlatform(DeltaTime);

		// ���� �̵� ó��
		AddMovementInput(GetActorForwardVector(), 1.0f);
	}
}

// �÷����� �߽� ��ǥ�� ���� �ε巴�� �̵�
void AENM_Movement::CenterLerpToPlatform(float DeltaTime)
{
	if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 90.f, 1.f))
	{
		// X�� �߽����� ����
		float NewX = FMath::FInterpTo(GetActorLocation().X, CurrentPlatformCenter.X, DeltaTime, 7.5f);
		SetActorLocation(FVector(NewX, GetActorLocation().Y, GetActorLocation().Z));
	}
	else if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 0.f, 1.f) || FMath::IsNearlyEqual(GetActorRotation().Yaw, 180.f, 1.f))
	{
		// Y�� �߽����� ����
		float NewY = FMath::FInterpTo(GetActorLocation().Y, CurrentPlatformCenter.Y, DeltaTime, 7.5f);
		SetActorLocation(FVector(GetActorLocation().X, NewY, GetActorLocation().Z));
	}
}

// �ٴڿ� �ִ� �÷��� ������ �߽� ��ǥ ���
void AENM_Movement::FindPlatformCenter()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 600.f); // �Ʒ� ���� Raycast

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // �ڱ� �ڽ� ����

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* Platform = Hit.GetActor();
		if (Platform)
		{
			CurrentPlatformCenter = Platform->GetActorLocation();
		}
	}
}

// ȸ�� ó�� �Լ�
void AENM_Movement::TurnToNextRotation(float DeltaTime)
{
	FRotator CurrentRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaTime, 10.0f);
	SetActorRotation(NewRot);

	// ȸ���� ���� �����ٸ� ����
	if (FMath::Abs(NewRot.Yaw - TargetRotation.Yaw) <= 3.0f)
	{
		SetActorRotation(TargetRotation);
		bIsTurning = false;
	}
}

// ���� ������ �浹 ó��
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

// ���� ����
void AENM_Movement::ResumeMovement()
{
	bIsStopped = false;
}

// ���� ����
void AENM_Movement::ResetBossSpeed()
{
	bIsHolding = false;
}

// ������ ó��
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


