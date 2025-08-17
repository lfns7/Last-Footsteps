#include "OBS_BasePlatform.h"
#include "Components/BoxComponent.h"

AOBS_BasePlatform::AOBS_BasePlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOBS_BasePlatform::BeginPlay()
{
	Super::BeginPlay();
	if (Trigger != nullptr)
	{
		// Trigger가 nullptr가 아니라면 Trigger를 게임 내에서 보이지 않게끔 설정
		Trigger->bHiddenInGame = true;
	}
}

// TriggerZone의 위치를 반환
UBoxComponent* AOBS_BasePlatform::GetTrigger()
{
	return Trigger;
}

// SpawnPoint1의 위치를 반환
UBoxComponent* AOBS_BasePlatform::GetSpawnTarget_1()
{
	return ObsSpawnTarget_1;
}