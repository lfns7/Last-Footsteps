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
	//�÷��̾� �̵��ӵ�*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;


	//�������� �����Լ�*
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	//�÷��̾ ���߿� �ִ��� ����*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool bIsNotGroundedAnim = false;

	//�÷��̾ �����̵������� ����*
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool bIsSlidingAnim = false;
};
