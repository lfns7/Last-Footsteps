// Fill out your copyright notice in the Description page of Project Settings.


#include "CHR_Animation.h"
#include "CHR_Movement.h"
#include <GameFramework/CharacterMovementComponent.h>

void UCHR_Animation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//�÷��̾��� �̵��ӵ� �����ͼ� speed�� �Ҵ�
	//���� �� ���
	auto ownerPawn = TryGetPawnOwner();
	//�÷��̾�� ĳ����
	auto player = Cast<ACHR_Movement>(ownerPawn);
	//ĳ���� ������
	if (player)
	{
		//�̵��ӵ� �ʿ�
		FVector velocity = player->GetVelocity();

		//�÷��̾��� ���溤��
		FVector forwardVector = player->GetActorForwardVector();

		//speed�� �� �Ҵ�
		speed = FVector::DotProduct(forwardVector, velocity);

		//�÷��̾ ���� ���߿� �ִ°�?
		auto movement = player->GetCharacterMovement();
		bIsNotGroundedAnim = movement->IsFalling();

		bIsSlidingAnim = player->bIsSliding;
	}
}

