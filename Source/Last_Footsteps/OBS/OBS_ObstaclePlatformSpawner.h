// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OBS_ObstaclePlatformSpawner.generated.h"

class AOBS_BasePlatform;

UCLASS()
class LAST_FOOTSTEPS_API AOBS_ObstaclePlatformSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOBS_ObstaclePlatformSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// �÷��� ������ ����
	UFUNCTION()
	void SpawnPlatform(bool isFirst);

	// Collider�� �����ߴ��� ���� �Ǵ�
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BossOnOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// �����Ǵ� Platform�� ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TurnPlatform;

	// �����Ǵ� Platform�� ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform01;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform02;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform03;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform04;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform05;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform06;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform07;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AOBS_BasePlatform> Platform08;

	// Platform�� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TArray<AOBS_BasePlatform*> PlatformList;

public:
	// �����Ǵ� �÷������� ������ �� �� RandRange�� �̱� ���� ����
	int RandomPlatform;

	// ������ �÷����� ���� ȸ���� �� RandRange�� �̱� ���� ����
	int RandomRotation;

	// ������ �÷����� ȸ�� ����
	int CurrentPlatformRotation;

	// ���� ��ȯ �÷����� �߰� �ٸ��� ���� ȸ�� ����
	int PlatformBridge;

	// �÷����� ȸ���Ǿ����� ���� �Ǵ�
	bool isTurned;

	// ������ ��ġ
	FVector ObsPlatformSpawnLocation = FVector();

	// ������ ȸ�� ����
	FRotator ObsPlatformSpawnRotation = FRotator();
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
};
