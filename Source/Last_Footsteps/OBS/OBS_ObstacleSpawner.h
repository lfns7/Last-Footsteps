// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "OBS_ObstacleSpawner.generated.h"

USTRUCT(BlueprintType)
struct FRandomObstacleData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	UStaticMesh* ObsObstacleMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	FVector ObsObstacleLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	FRotator ObsObstacleRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	FVector ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	bool ObsObstacleRope = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LAST_FOOTSTEPS_API UOBS_ObstacleSpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOBS_ObstacleSpawner();
	
	const int MAX_LANE_SIZE = 3;

	int SelectedPosition;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector InitOneLaneObstaclePosition = FVector::ZeroVector;
	FVector InitThreeLaneObstaclePosition = FVector::ZeroVector;

	float InitOneLanePosition = 0.f;
	float InitThreeLanePosition = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	TArray<FRandomObstacleData> OneLaneObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomObstacle")
	TArray<FRandomObstacleData> ThreeLaneObstacle;

	UPROPERTY(EditAnywhere, Category = "RandomObstacle")
	TSubclassOf<AActor> RopeObstacleSwingPoint;


private:
	UFUNCTION(Category = "RandomObstacle")
	FVector SpawnOneLaneObstacle(int SpawnDistance);

	UFUNCTION(Category = "RandomObstacle")
	FVector SpawnThreeLaneObstacle(int SpawnDistance);


	UFUNCTION(Category = "RandomObstacle")
	void SpawnOneLaneRandomMesh(FVector ObstaclePosition);
	
	UFUNCTION(Category = "RandomObstacle")
	void SpawnThreeLaneRandomMesh(FVector ObstaclePosition);

	int RandomSelect(int RandSize);	

	int MaxIndex = 1;
	int RandIndex = 6;
	int ObstacleRange = 1000;
};