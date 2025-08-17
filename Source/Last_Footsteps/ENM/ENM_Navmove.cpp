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

    // NavMesh ��ġ ����
    UpdateNavMeshLocation();
}

void AENM_Navmove::UpdateNavMeshLocation()
{
    // �÷��̾� ��ġ ��������
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter) return;

    FVector NewLocation = PlayerCharacter->GetActorLocation();

    // ù ��° NavMeshBoundsVolume�� ã�� �̵�
    for (TActorIterator<ANavMeshBoundsVolume> It(GetWorld()); It; ++It)
    {
        ANavMeshBoundsVolume* NavBounds = *It;
        if (NavBounds)
        {
            NavBounds->SetActorLocation(NewLocation);

            // ������̼� �ý��ۿ� ��ġ ���� �˸���
            UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
            if (NavSys)
            {
                NavSys->OnNavigationBoundsUpdated(NavBounds);
            }
            break;
        }
    }
}
