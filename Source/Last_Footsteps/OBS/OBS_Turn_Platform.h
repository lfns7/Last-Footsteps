// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHR_Movement.h"
#include "ENM_Movement.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "OBS_Turn_Platform.generated.h"

class UBoxComponent;

UCLASS()
class LAST_FOOTSTEPS_API AOBS_Turn_Platform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOBS_Turn_Platform();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Triggers")
	UBoxComponent* Trigger;

	UBoxComponent* GetTrigger();


	// 캐릭터 캐스팅을 위함
	ACHR_Movement* CHR_PlayerCharacter;
	AENM_Movement* ENM_BossCharacter;


	// 플랫폼의 회전 위치
	FRotator PlatformRotation;
	FRotator TempRotation;

	// Collider에 접촉했는지 여부 판단
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult);

	// 캐릭터 회전에 관한 bool
	bool CharacterRotate;
	bool IsTurned;

	//보스 회전 변수
	FRotator BossTargetRot;
	bool BossRotate = false;
};
