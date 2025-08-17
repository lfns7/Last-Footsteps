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

	// 플레이어 캐릭터 캐스팅
	CHR_PlayerCharacter = Cast<ACHR_Movement>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ENM_BossCharacter = Cast<AENM_Movement>(UGameplayStatics::GetActorOfClass(GetWorld(), 0));

	// 회전 판정을 위한 Trigger
	if (Trigger)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOBS_Turn_Platform::OnBoxBeginOverlap);
	}
}

void AOBS_Turn_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 플레이어가 회전 중일 때 보간 처리
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
	// 보스가 회전 중일 때 보간 처리
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

// TriggerZone의 위치를 반환
UBoxComponent* AOBS_Turn_Platform::GetTrigger()
{
	return Trigger;
}

// TurnPlatform의 TriggerZone에서 나갔을 때 발동
// 새 플랫폼을 생성하도록 함수 호출
void AOBS_Turn_Platform::OnBoxBeginOverlap(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ArrowDirection 가져오기
	UArrowComponent* Arrow = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("ArrowDirection")));
	if (!Arrow)
		return;

	// 회전 방향 계산
	FRotator ArrowRotation = Arrow->GetComponentRotation();
	FRotator CalculatedRotation = FRotator(0, 90.f, 0);

	if (FMath::IsNearlyEqual(ArrowRotation.Yaw, -90.f, 1.f))
		CalculatedRotation = FRotator(0, 180.f, 0);
	else if (FMath::IsNearlyEqual(ArrowRotation.Yaw, 0.f, 1.f))
		CalculatedRotation = FRotator(0, 0.f, 0);

	PlatformRotation = CalculatedRotation;

	// 플레이어 회전 설정
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

	// 보스 회전 설정
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