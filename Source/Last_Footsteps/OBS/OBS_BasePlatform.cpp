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
		// Trigger�� nullptr�� �ƴ϶�� Trigger�� ���� ������ ������ �ʰԲ� ����
		Trigger->bHiddenInGame = true;
	}
}

// TriggerZone�� ��ġ�� ��ȯ
UBoxComponent* AOBS_BasePlatform::GetTrigger()
{
	return Trigger;
}

// SpawnPoint1�� ��ġ�� ��ȯ
UBoxComponent* AOBS_BasePlatform::GetSpawnTarget_1()
{
	return ObsSpawnTarget_1;
}