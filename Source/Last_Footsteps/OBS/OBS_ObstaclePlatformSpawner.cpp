#include "OBS_ObstaclePlatformSpawner.h"
#include "OBS_BasePlatform.h"
#include "ENM_Movement.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

AOBS_ObstaclePlatformSpawner::AOBS_ObstaclePlatformSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	isTurned = false;
}

void AOBS_ObstaclePlatformSpawner::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� �÷��� 2�� ����
	SpawnPlatform(true);
	SpawnPlatform(false);
}

void AOBS_ObstaclePlatformSpawner::SpawnPlatform(bool isFirst)
{
	ObsPlatformSpawnLocation = FVector(0.f, 4000.f, 0.f);
	ObsPlatformSpawnRotation = FRotator(0, 0, 0);

	// �÷��� ȸ���� ���� ���� ����
	FVector TempRotation = FVector::ZeroVector;

	if (!isFirst)
	{
		// ������(��)�� ��ġ�� ������ �÷����� ��ġ�� ��ȯ�ޱ� ���� AOBS_BasePlatform ��ü
		AOBS_BasePlatform* LastPlatform = PlatformList.Last();
		ObsPlatformSpawnLocation = LastPlatform->GetSpawnTarget_1()->GetComponentTransform().GetTranslation();

		// LastPlatform�� ȸ����
		TempRotation = ObsPlatformSpawnLocation;


		// ����, ������, ���� �б� �÷��� ������ ���� ����
		RandomRotation = FMath::RandRange(1, 9);

		// ���� �÷��� ������ �� �Ǵ� �� ȸ���� ��츦 ���� �ڵ� (���� Ȯ���� ���ۺ��� ���� ���� �����ϱ� ����)
		if (isTurned)
			RandomRotation = 10;

		switch (RandomRotation)
		{
			// �÷��� ���� ���� ����
			case 1:
				isTurned = true;
				PlatformBridge = 90;

				ObsPlatformSpawnLocation += FVector(500, 0, 0);
				ObsPlatformSpawnRotation = FRotator(0, -90, 0);
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Left Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				break;

			// �÷��� ������ ���� ����
			case 2:
				isTurned = true;
				PlatformBridge = 0;

				ObsPlatformSpawnLocation += FVector(-500.f, 0, 0);
				ObsPlatformSpawnRotation = FRotator(0, 90, 0);
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Right Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				break;

				// �÷��� ���� ���� ����
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				isTurned = false;
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Forward Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				ObsPlatformSpawnLocation += FVector(0, -500.f, 0);
				ObsPlatformSpawnRotation = FRotator(0, 0, 0);
				break;

				// �÷��� ȸ�� ���� ����
			case 10:
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Turn After Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				
				// �÷����� �÷����� �մ� ���� �÷��� ����
				GetWorld()->SpawnActor<AActor>(TurnPlatform, TempRotation, FRotator(0, PlatformBridge+180, 0));
				isTurned = false;
				ObsPlatformSpawnLocation += FVector(0, +500.f, 0);
				break;
		}
	}

	// �÷����� ������ ȸ���� ��쿡 ���� Rotation �� Yaw ���� �ٲ� �Ϳ� ���� ��ġ ���̸�ŭ�� �����ϰ�
	// �÷����� �÷����� �մ� ���� �÷��� ����
	if (isTurned)
	{
		GetWorld()->SpawnActor<AActor>(TurnPlatform, TempRotation, FRotator(0, PlatformBridge, 0));
	}

	// ������ �÷��� ������ �����ϵ��� ����
	// ��� �÷����� ������ ���� �����ϴ� �κ�
	RandomPlatform = FMath::RandRange(1, 8);
	AOBS_BasePlatform* NewPlatform = nullptr;

	if (RandomPlatform == 1)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform01, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}

	if (RandomPlatform == 2)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform02, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}

	if (RandomPlatform == 3)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform03, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}

	if (RandomPlatform == 4)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform04, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}

	if (RandomPlatform == 5)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform05, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}

	if (RandomPlatform == 6)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform06, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}

	if (RandomPlatform == 7)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform07, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}
	
	if (RandomPlatform == 8)
	{
		NewPlatform = GetWorld()->SpawnActor<AOBS_BasePlatform>(Platform08, ObsPlatformSpawnLocation, ObsPlatformSpawnRotation, SpawnInfo);
	}


	// �� �÷����� � ���𰡰� ����� ��� �� �÷��� ����
	// ��� ����
	if (NewPlatform)
	{
		if (NewPlatform->GetTrigger())
		{
			NewPlatform->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AOBS_ObstaclePlatformSpawner::OnOverlapBegin);
			NewPlatform->GetSpawnTarget_1()->OnComponentBeginOverlap.AddDynamic(this, &AOBS_ObstaclePlatformSpawner::BossOnOverlapBegin);
		}
	}

	// �÷��� ��Ͽ� �� �÷����� �߰�
	PlatformList.Add(NewPlatform);

	// �÷��� ����� ���� 5���� �ѱ�� ����� 0��° ����
	if (PlatformList.Num() > 5)
	{
		PlatformList.RemoveAt(0);

	}
}

// BasePlatform�� TriggerZone�� ����� ��� �ߵ�
// �� �÷����� �����ϵ��� �Լ� ȣ��
void AOBS_ObstaclePlatformSpawner::OnOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnPlatform(false);
}

void AOBS_ObstaclePlatformSpawner::BossOnOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AENM_Movement::StaticClass()))
	{
		AOBS_BasePlatform* TargetPlatform = *PlatformList.begin();
		TargetPlatform->Destroy();
	}
}
