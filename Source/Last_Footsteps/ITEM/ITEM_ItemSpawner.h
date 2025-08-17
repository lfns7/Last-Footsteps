// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "ITEM_ItemSpawner.generated.h"

/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UITEM_ItemSpawner : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UITEM_ItemSpawner();
	virtual void BeginPlay() override;

public:
	// ����
	const int MAX_LANE_SIZE = 3;
	
	// 0 ����, 1 �߾�, 2 ������
	int SelectedPosition = 1;

public:
	// ������ ���� �κ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomItem")
	TArray<TSubclassOf<AActor>> ITEM_RandomItems;

	UPROPERTY(EditAnywhere, Category = "RandomItem")
	TArray<TSubclassOf<AActor>> ITEM_RandomWeapons;
	
	FVector InitItemSpawnPosition = FVector::ZeroVector;
	FVector GetSpawnItemPosition(int SpawnDistance);

	void SpawnRandomItem(FVector ObstaclePosition);

	float InitPosition = 0.f;

	bool bItemSpawned = false;
	
private:
	int RandomSelect(int RandSize);
};
