// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CHR_Animation.generated.h"


/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UCHR_Animation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//플레이어 이동속도*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;


	//매프레임 갱신함수*
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	//플레이어가 공중에 있는지 여부*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool bIsNotGroundedAnim = false;

	//플레이어가 슬라이딩중인지 여부*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool bIsSlidingAnim = false;
};
