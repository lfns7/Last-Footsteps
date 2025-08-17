// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Navmove.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h" 

AENM_Navmove::AENM_Navmove()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AENM_Navmove::BeginPlay()
{
    Super::BeginPlay();
}

void AENM_Navmove::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // NavMesh 위치 갱신
    UpdateNavMeshLocation();
}

void AENM_Navmove::UpdateNavMeshLocation()
{
    // 플레이어 위치 가져오기
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter) return;

    FVector NewLocation = PlayerCharacter->GetActorLocation();

    // 첫 번째 NavMeshBoundsVolume을 찾아 이동
    for (TActorIterator<ANavMeshBoundsVolume> It(GetWorld()); It; ++It)
    {
        ANavMeshBoundsVolume* NavBounds = *It;
        if (NavBounds)
        {
            NavBounds->SetActorLocation(NewLocation);

            // 내비게이션 시스템에 위치 변경 알리기
            UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
            if (NavSys)
            {
                NavSys->OnNavigationBoundsUpdated(NavBounds);
            }
            break;
        }
    }
}
