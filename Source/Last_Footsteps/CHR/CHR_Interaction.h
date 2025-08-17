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
	// ��, ���� ��ֹ� ���� ���� �Ǵ�
	void FindLeftObstacle();
	void FindRightObstacle();

	// ���� ��ֹ� ���� ���� �Ǵ� (�ǰ�)
	void FindForwardObstacle();

private:
	// Collider�� �������� �����ߴ��� ���� �Ǵ�
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
