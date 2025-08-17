// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHR_Interaction.generated.h"


class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LAST_FOOTSTEPS_API UCHR_Interaction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCHR_Interaction();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	// 좌, 우측 장애물 존재 여부 판단
	void FindLeftObstacle();
	void FindRightObstacle();

	// 전방 장애물 존재 여부 판단 (피격)
	void FindForwardObstacle();

private:
	// Collider에 아이템이 접촉했는지 여부 판단
	UFUNCTION()
	void GetItem_OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UBoxComponent* ItemCatcher;

};
