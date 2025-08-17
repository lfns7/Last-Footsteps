// Fill out your copyright notice in the Description page of Project Settings.


#include "ITEM_ItemSpawner.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "Kismet/GameplayStatics.h"
#include <random>

UITEM_ItemSpawner::UITEM_ItemSpawner()
{
	static ConstructorHelpers::FClassFinder<AActor> Score_Coin(TEXT("/Game/Blueprints/Items/BP_ITEM_Score_Coin.BP_ITEM_Score_Coin_C"));
	if (Score_Coin.Succeeded())
	{
		ITEM_RandomItems.Add(Score_Coin.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Health_Low(TEXT("/Game/Blueprints/Items/BP_ITEM_Health_Low.BP_ITEM_Health_Low_C"));
	if (Health_Low.Succeeded())
	{
		ITEM_RandomItems.Add(Health_Low.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Health_Large(TEXT("/Game/Blueprints/Items/BP_ITEM_Health_Large.BP_ITEM_Health_Large_C"));
	if (Health_Large.Succeeded())
	{
		ITEM_RandomItems.Add(Health_Large.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> PowerUp(TEXT("/Game/Blueprints/Items/BP_ITEM_PowerUp.BP_ITEM_PowerUp_C"));
	if (PowerUp.Succeeded())
	{
		ITEM_RandomItems.Add(PowerUp.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Shield(TEXT("/Game/Blueprints/Items/BP_ITEM_Shield.BP_ITEM_Shield_C"));
	if (Shield.Succeeded())
	{
		ITEM_RandomItems.Add(Shield.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> SuperShoes(TEXT("/Game/Blueprints/Items/BP_ITEM_SuperShoes.BP_ITEM_SuperShoes_C"));
	if (SuperShoes.Succeeded())
	{
		ITEM_RandomItems.Add(SuperShoes.Class);
	}


	static ConstructorHelpers::FClassFinder<AActor> Dynamite(TEXT("/Game/Blueprints/Items/BP_ITEM_Weapon_Dynamite.BP_ITEM_Weapon_Dynamite_C"));
	if (Dynamite.Succeeded())
	{
		ITEM_RandomWeapons.Add(Dynamite.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Grenade(TEXT("/Game/Blueprints/Items/BP_ITEM_Weapon_Grenade.BP_ITEM_Weapon_Grenade_C"));
	if (Grenade.Succeeded())
	{
		ITEM_RandomWeapons.Add(Grenade.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Molotov(TEXT("/Game/Blueprints/Items/BP_ITEM_Weapon_Molotov.BP_ITEM_Weapon_Molotov_C"));
	if (Molotov.Succeeded())
	{
		ITEM_RandomWeapons.Add(Molotov.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Stone(TEXT("/Game/Blueprints/Items/BP_ITEM_Weapon_Stone.BP_ITEM_Weapon_Stone_C"));
	if (Stone.Succeeded())
	{
		ITEM_RandomWeapons.Add(Stone.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Trap(TEXT("/Game/Blueprints/Items/BP_ITEM_Weapon_Trap.BP_ITEM_Weapon_Trap_C"));
	if (Trap.Succeeded())
	{
		ITEM_RandomWeapons.Add(Trap.Class);
	}
}

// Called when the game starts
void UITEM_ItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
	ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode);
	if (GM)
	{
		SelectedPosition = GM->GM_SelectedPosition;
	}


	for (int i = 0; i < 8500; i += 250)
	{
		SpawnRandomItem(GetSpawnItemPosition(i));

		if (i > 8500)
			GM->GM_SelectedPosition = SelectedPosition;
	}
}

// 단일 아이템 생성 부분
void UITEM_ItemSpawner::SpawnRandomItem(FVector ObstaclePosition)
{
	// TArray에 등록된 메시의 수 판별 (예외처리)
	if (ITEM_RandomItems.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ItemArraySize : 0")));
		return;
	}

	TSubclassOf<AActor> Selected;
	// 소환할 아이템 뽑기
	// 0.75% + 0.75%
	if (!bItemSpawned && (RandomSelect(9999) <= 74))
	{
		// 아이템 생성
		// 전체 배열의 수는 1 이상
		// 뽑는 배열의 수는 1 감소
		Selected = ITEM_RandomItems[1 + RandomSelect(ITEM_RandomItems.Num() - 2)];
		bItemSpawned = true;
	}

	else if (!bItemSpawned && (RandomSelect(9999) >= 9925))
	{
		Selected = ITEM_RandomWeapons[RandomSelect(ITEM_RandomWeapons.Num() - 1)];
		bItemSpawned = true;
	}

	else
	{
		// 코인 확정 생성
		Selected = ITEM_RandomItems[0];
	}
	
	if (!Selected)
		return;

	// 월드 컨텍스트 구하기
	UWorld* World = GetWorld();
	if (!World) return;

	// 스폰 트랜스폼 만들기
	FVector SpawnLocation = GetComponentLocation() + ObstaclePosition;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	// 액터 스폰
	AActor* SpawnedActor = World->SpawnActor<AActor>(Selected, SpawnLocation, SpawnRotation, SpawnParams);
	if (!SpawnedActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Actor Spawn Failed!"));
	}
}


// 랜덤 처리부분
int UITEM_ItemSpawner::RandomSelect(int RandSize)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, RandSize); // 무작위 범위
	auto RandResult = dist(mt); // 실제로 수를 뽑기

	return RandResult;
}


// 아이템 생성 부분
FVector UITEM_ItemSpawner::GetSpawnItemPosition(int SpawnDistance)
{
	// 33%의 확률로 생성 라인 변경 찬스, 0~2 추첨
	if (RandomSelect(2) == 1)
	{
		// 아이템, 코인이 생성되는 위치가
		// 왼쪽
		if (SelectedPosition == 0)
		{
			SelectedPosition = 1;
		}

		// 중앙
		else if (SelectedPosition == 1)
		{
			// 0~1 추첨
			if (RandomSelect(1) == 0)
			{
				SelectedPosition = 0;
			}
			else
			{
				SelectedPosition = 2;
			}
		}

		// 오른쪽
		else
		{
			SelectedPosition = 1;
		}
	}

	// 레인 뽑기
	switch (SelectedPosition)
	{
		// 현재 플랫폼의 각 레인을 기준으로 생성
	case 0:
		InitPosition = 250;
		break;
	case 1:
		InitPosition = 0;
		break;
	case 2:
		InitPosition = -250;
		break;
	}

	// 해당 Actor의 Rotation 값을 반환 받아서 장애물 위치 판별
	if (GetAttachmentRootActor())
	{
		if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, 0.f, 1.f))
		{
			InitItemSpawnPosition = FVector(InitPosition, SpawnDistance, 75);
		}
		else if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, -90.f, 1.f))
		{
			InitItemSpawnPosition = FVector(SpawnDistance, InitPosition, 75);
		}
		else if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, 90.f, 1.f))
		{
			InitItemSpawnPosition = FVector(-SpawnDistance, -InitPosition, 75);
		}
	}

	return InitItemSpawnPosition;
}