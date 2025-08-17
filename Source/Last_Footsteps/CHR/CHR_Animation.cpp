// Fill out your copyright notice in the Description page of Project Settings.


#include "CHR_Animation.h"
#include "CHR_Movement.h"
#include <GameFramework/CharacterMovementComponent.h>

void UCHR_Animation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어의 이동속도 가져와서 speed에 할당
	//소유 폰 얻기
	auto ownerPawn = TryGetPawnOwner();
	//플레이어로 캐스팅
	auto player = Cast<ACHR_Movement>(ownerPawn);
	//캐스팅 성공시
	if (player)
	{
		//이동속도 필요
		FVector velocity = player->GetVelocity();

		//플레이어의 전방벡터
		FVector forwardVector = player->GetActorForwardVector();

		//speed에 값 할당
		speed = FVector::DotProduct(forwardVector, velocity);

		//플레이어가 현재 공중에 있는가?
		auto movement = player->GetCharacterMovement();
		bIsNotGroundedAnim = movement->IsFalling();

		bIsSlidingAnim = player->bIsSliding;
	}
}

