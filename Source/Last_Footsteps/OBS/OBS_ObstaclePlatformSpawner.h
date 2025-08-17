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
	// 플랫폼 생성을 위함
	UFUNCTION()
	void SpawnPlatform(bool isFirst);

	// Collider에 접촉했는지 여부 판단
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BossOnOverlapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// 생성되는 Platform의 종류
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TurnPlatform;

	// 생성되는 Platform의 종류
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

	// Platform을 담는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TArray<AOBS_BasePlatform*> PlatformList;

public:
	// 생성되는 플랫폼으로 무엇을 할 지 RandRange로 뽑기 위한 변수
	int RandomPlatform;

	// 생성할 플랫폼이 어디로 회전할 지 RandRange로 뽑기 위한 변수
	int RandomRotation;

	// 생성할 플랫폼의 회전 방향
	int CurrentPlatformRotation;

	// 방향 전환 플랫폼의 중간 다리를 위한 회전 방향
	int PlatformBridge;

	// 플랫폼이 회전되었는지 여부 판단
	bool isTurned;

	// 생성할 위치
	FVector ObsPlatformSpawnLocation = FVector();

	// 생성할 회전 정보
	FRotator ObsPlatformSpawnRotation = FRotator();
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
};
