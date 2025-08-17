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

	// ÃÖÃÊ ½ÃÀÛ ½Ã ÇÃ·§Æû 2°³ »ý¼º
	SpawnPlatform(true);
	SpawnPlatform(false);
}

void AOBS_ObstaclePlatformSpawner::SpawnPlatform(bool isFirst)
{
	ObsPlatformSpawnLocation = FVector(0.f, 4000.f, 0.f);
	ObsPlatformSpawnRotation = FRotator(0, 0, 0);

	// ÇÃ·§Æû È¸ÀüÀ» À§ÇÑ º¤ÅÍ º¯¼ö
	FVector TempRotation = FVector::ZeroVector;

	if (!isFirst)
	{
		// ¸¶Áö¸·(³¡)ÀÇ À§Ä¡¿¡ »ý¼ºµÈ ÇÃ·§ÆûÀÇ À§Ä¡¸¦ ¹ÝÈ¯¹Þ±â À§ÇÑ AOBS_BasePlatform °´Ã¼
		AOBS_BasePlatform* LastPlatform = PlatformList.Last();
		ObsPlatformSpawnLocation = LastPlatform->GetSpawnTarget_1()->GetComponentTransform().GetTranslation();

		// LastPlatformÀÇ È¸Àü°ª
		TempRotation = ObsPlatformSpawnLocation;


		// ¿ÞÂÊ, ¿À¸¥ÂÊ, Á÷¼± ºÐ±â ÇÃ·§Æû »ý¼ºÀ» À§ÇÑ ³­¼ö
		RandomRotation = FMath::RandRange(1, 9);

		// ÀÌÀü ÇÃ·§Æû »ý¼ºÀÌ ÁÂ ¶Ç´Â ¿ì È¸ÀüÀÎ °æ¿ì¸¦ À§ÇÑ ÄÚµå (³·Àº È®·ü·Î ºù±Ûºù±Û µµ´Â °ÍÀ» ¹æÁöÇÏ±â À§ÇÔ)
		if (isTurned)
			RandomRotation = 10;

		switch (RandomRotation)
		{
			// ÇÃ·§Æû ¿ÞÂÊ ¹æÇâ »ý¼º
			case 1:
				isTurned = true;
				PlatformBridge = 90;

				ObsPlatformSpawnLocation += FVector(500, 0, 0);
				ObsPlatformSpawnRotation = FRotator(0, -90, 0);
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Left Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				break;

			// ÇÃ·§Æû ¿À¸¥ÂÊ ¹æÇâ »ý¼º
			case 2:
				isTurned = true;
				PlatformBridge = 0;

				ObsPlatformSpawnLocation += FVector(-500.f, 0, 0);
				ObsPlatformSpawnRotation = FRotator(0, 90, 0);
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Right Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				break;

				// ÇÃ·§Æû Á÷¼± ¹æÇâ »ý¼º
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

				// ÇÃ·§Æû È¸Àü ÀÌÈÄ »ý¼º
			case 10:
				//GEngine->AddOnScreenDebugMessage(2237, 2.f, FColor::Emerald, (FString::Printf(TEXT("Turn After Spawned, SpawnLocation : %s, SpawnRotation : %s"), *ObsPlatformSpawnLocation.ToString(), *ObsPlatformSpawnRotation.ToString())));
				
				// ÇÃ·§Æû°ú ÇÃ·§ÆûÀ» ÀÕ´Â ÀÛÀº ÇÃ·§Æû »ý¼º
				GetWorld()->SpawnActor<AActor>(TurnPlatform, TempRotation, FRotator(0, PlatformBridge+180, 0));
				isTurned = false;
				ObsPlatformSpawnLocation += FVector(0, +500.f, 0);
				break;
		}
	}

	// ÇÃ·§ÆûÀÌ Á÷Àü¿¡ È¸ÀüÇÑ °æ¿ì¿¡ ÇÑÇØ Rotation Áß Yaw °ªÀ» ¹Ù²Û °Í¿¡ ´ëÇÑ À§Ä¡ Â÷ÀÌ¸¸Å­À» º¸Á¤ÇÏ°í
	// ÇÃ·§Æû°ú ÇÃ·§ÆûÀ» ÀÕ´Â ÀÛÀº ÇÃ·§Æû »ý¼º
	if (isTurned)
	{
		GetWorld()->SpawnActor<AActor>(TurnPlatform, TempRotation, FRotator(0, PlatformBridge, 0));
	}

	// ¹«ÀÛÀ§ ÇÃ·§Æû Á¾·ù¸¦ »ý¼ºÇÏµµ·Ï ¸¸µê
	// ¾î¶°ÇÑ ÇÃ·§ÆûÀÌ »ý¼ºµÉ Áö¸¦ °áÁ¤ÇÏ´Â ºÎºÐ
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


	// »õ ÇÃ·§Æû¿¡ ¾î¶² ¹«¾ð°¡°¡ ´ê¾ÒÀ» °æ¿ì »õ ÇÃ·§Æû »ý¼º
	// Àç±Í ±¸Á¶
	if (NewPlatform)
	{
		if (NewPlatform->GetTrigger())
		{
			NewPlatform->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AOBS_ObstaclePlatformSpawner::OnOverlapBegin);
			NewPlatform->GetSpawnTarget_1()->OnComponentBeginOverlap.AddDynamic(this, &AOBS_ObstaclePlatformSpawner::BossOnOverlapBegin);
		}
	}

	// ÇÃ·§Æû ¸ñ·Ï¿¡ »õ ÇÃ·§ÆûÀ» Ãß°¡
	PlatformList.Add(NewPlatform);

	// ÇÃ·§Æû ¸ñ·ÏÀÇ ¼ö°¡ 5°³¸¦ ³Ñ±â¸é ¸ñ·ÏÀÇ 0¹øÂ° »èÁ¦
	if (PlatformList.Num() > 5)
	{
		PlatformList.RemoveAt(0);

	}
}

// BasePlatformÀÇ TriggerZone¿¡ ´ê¾ÒÀ» °æ¿ì ¹ßµ¿
// »õ ÇÃ·§ÆûÀ» »ý¼ºÇÏµµ·Ï ÇÔ¼ö È£Ãâ
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
