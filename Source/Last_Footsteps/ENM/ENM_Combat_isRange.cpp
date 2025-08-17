// Fill out your copyright notice in the Description page of Project Settings.

#include "ENM_Combat_isRange.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// ������: ������Ʈ�� �⺻�� �ʱ�ȭ
UENM_Combat_isRange::UENM_Combat_isRange()
{
    PrimaryComponentTick.bCanEverTick = true; // ƽ Ȱ��ȭ
    bPlayerInRange = false;                    // �⺻������ �÷��̾�� ���� ��
    CachedPlayer = nullptr;                    // ĳ�õ� �÷��̾� �ʱ�ȭ
}

// ���� ���� �� �ݸ��� �ڽ��� ã�Ƽ� ������ �̺�Ʈ ���ε�
void UENM_Combat_isRange::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // ���� ������ BoxComponent���� ������
    TArray<UBoxComponent*> BoxComponents;
    Owner->GetComponents<UBoxComponent>(BoxComponents);

    // �̸� �������� �� ���� ����
    for (UBoxComponent* Box : BoxComponents)
    {
        if (Box->GetName().Contains(TEXT("Middle Range")))
        {
            MiddleRange = Box;
        }
        else if (Box->GetName().Contains(TEXT("Bottom Range")))
        {
            BottomRange = Box;
        }
    }

    // ó�� �� ���� Player ã�Ƽ� ĳ��
    if (!CachedPlayer)
    {
        ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (Player) CachedPlayer = Player;
    }

    // �α� Ȯ�ο� ������ �̺�Ʈ ���ε�
    if (MiddleRange)
    {
        MiddleRange->OnComponentBeginOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeBegin);
        MiddleRange->OnComponentEndOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeEnd);
    }
    if (BottomRange)
    {
        BottomRange->OnComponentBeginOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeBegin);
        BottomRange->OnComponentEndOverlap.AddDynamic(this, &UENM_Combat_isRange::OnAnyRangeEnd);
    }
}

// �� �����Ӹ��� ���� �ȿ� �ִ��� ���� �˻�
void UENM_Combat_isRange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!CachedPlayer && !BottomRange && !MiddleRange) return;

    // ������ �ڽ��� �÷��̾ �����ִ��� üũ
    bool bIsInBottom = BottomRange && BottomRange->IsOverlappingActor(CachedPlayer);
    bool bIsInMiddle = MiddleRange && MiddleRange->IsOverlappingActor(CachedPlayer);

    // �ϳ��� ���������� true
    bPlayerInRange = bIsInBottom || bIsInMiddle;
}

// �÷��̾ �ݸ��� ������ ������ �� ȣ��
void UENM_Combat_isRange::OnAnyRangeBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Player = Cast<ACharacter>(OtherActor);
    if (Player)
    {

        // � �ݸ��� �ڽ����� �α� ���
        FString ColliderName = OverlappedComponent ? OverlappedComponent->GetName() : TEXT("Unknown");
        UE_LOG(LogTemp, Warning, TEXT("Player entered combat range: %s"), *ColliderName);
    }
}

// �÷��̾ �ݸ��� ������ ����� �� ȣ��
void UENM_Combat_isRange::OnAnyRangeEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* Player = Cast<ACharacter>(OtherActor);
    if (Player)
    {

        // � �ݸ��� �ڽ����� �α� ���
        FString ColliderName = OverlappedComponent ? OverlappedComponent->GetName() : TEXT("Unknown");
        UE_LOG(LogTemp, Warning, TEXT("Player exited combat range: %s"), *ColliderName);
    }
}