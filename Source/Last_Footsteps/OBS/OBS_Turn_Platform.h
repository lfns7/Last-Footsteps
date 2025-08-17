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


	// ĳ���� ĳ������ ����
	ACHR_Movement* CHR_PlayerCharacter;
	AENM_Movement* ENM_BossCharacter;


	// �÷����� ȸ�� ��ġ
	FRotator PlatformRotation;
	FRotator TempRotation;

	// Collider�� �����ߴ��� ���� �Ǵ�
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult);

	// ĳ���� ȸ���� ���� bool
	bool CharacterRotate;
	bool IsTurned;

	//���� ȸ�� ����
	FRotator BossTargetRot;
	bool BossRotate = false;
};
