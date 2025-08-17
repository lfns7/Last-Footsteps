// Fill out your copyright notice in the Description page of Project Settings.


#include "OBS_Turn_Platform.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AOBS_Turn_Platform::AOBS_Turn_Platform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOBS_Turn_Platform::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ĳ���� ĳ����
	CHR_PlayerCharacter = Cast<ACHR_Movement>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ENM_BossCharacter = Cast<AENM_Movement>(UGameplayStatics::GetActorOfClass(GetWorld(), 0));

	// ȸ�� ������ ���� Trigger
	if (Trigger)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOBS_Turn_Platform::OnBoxBeginOverlap);
	}
}

void AOBS_Turn_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// �÷��̾ ȸ�� ���� �� ���� ó��
	if (CharacterRotate && CHR_PlayerCharacter)
	{
		FRotator NewRot = FMath::RInterpTo(CHR_PlayerCharacter->GetActorRotation(), PlatformRotation, DeltaTime, 20.f);
		CHR_PlayerCharacter->SetActorRotation(NewRot);

		if (FMath::Abs(NewRot.Yaw - PlatformRotation.Yaw) <= 3.0f)
		{
			CHR_PlayerCharacter->SetActorRotation(PlatformRotation);
			CharacterRotate = false;
			CHR_PlayerCharacter->bIsTurning = false;
		}
	}
	// ������ ȸ�� ���� �� ���� ó��
	if (BossRotate && ENM_BossCharacter)
	{
		FRotator NewRot = FMath::RInterpTo(ENM_BossCharacter->GetActorRotation(), PlatformRotation, DeltaTime, 20.f);
		ENM_BossCharacter->SetActorRotation(NewRot);

		if (FMath::Abs(NewRot.Yaw - PlatformRotation.Yaw) <= 3.0f)
		{
			ENM_BossCharacter->SetActorRotation(PlatformRotation);
			BossRotate = false;
			ENM_BossCharacter->bIsTurning = false;
		}
	}
}

// TriggerZone�� ��ġ�� ��ȯ
UBoxComponent* AOBS_Turn_Platform::GetTrigger()
{
	return Trigger;
}

// TurnPlatform�� TriggerZone���� ������ �� �ߵ�
// �� �÷����� �����ϵ��� �Լ� ȣ��
void AOBS_Turn_Platform::OnBoxBeginOverlap(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ArrowDirection ��������
	UArrowComponent* Arrow = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("ArrowDirection")));
	if (!Arrow)
		return;

	// ȸ�� ���� ���
	FRotator ArrowRotation = Arrow->GetComponentRotation();
	FRotator CalculatedRotation = FRotator(0, 90.f, 0);

	if (FMath::IsNearlyEqual(ArrowRotation.Yaw, -90.f, 1.f))
		CalculatedRotation = FRotator(0, 180.f, 0);
	else if (FMath::IsNearlyEqual(ArrowRotation.Yaw, 0.f, 1.f))
		CalculatedRotation = FRotator(0, 0.f, 0);

	PlatformRotation = CalculatedRotation;

	// �÷��̾� ȸ�� ����
	if (ACHR_Movement* Player = Cast<ACHR_Movement>(OtherActor))
	{
		Player->GetCharacterMovement()->StopMovementImmediately();
		if (Player->GetCharacterMovement()->IsFalling())
		{
			Player->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 82.15f));
		}
		CHR_PlayerCharacter = Player;
		CHR_PlayerCharacter->bIsTurning = true;
		CharacterRotate = true;
		UE_LOG(LogTemp, Warning, TEXT("Trigger Overlap: %s"), *OtherActor->GetName());
	}

	// ���� ȸ�� ����
	else if (AENM_Movement* Boss = Cast<AENM_Movement>(OtherActor))
	{
		Boss->GetCharacterMovement()->StopMovementImmediately();
		ENM_BossCharacter = Boss;
		ENM_BossCharacter->bIsTurning = true;
		BossRotate = true;
		UE_LOG(LogTemp, Warning, TEXT("Trigger Overlap: %s"), *OtherActor->GetName());
	}
	IsTurned = true;
}