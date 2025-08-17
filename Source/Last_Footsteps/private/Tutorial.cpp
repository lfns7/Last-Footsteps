// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial.h"
#include "CHR_Movement.h"
#include "InputCoreTypes.h"
#include "UI_Tutorial.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "GI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Blueprint/UserWidget.h>

// Sets default values
ATutorial::ATutorial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATutorial::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	GI_GameInstance = Cast<UGI_GameInstance>(GameInstance);

	ACharacter* PL = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerCharacter = Cast<ACHR_Movement>(PL);

	PlayerController = GetWorld()->GetFirstPlayerController();

	AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
	GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode);

	TutorialWidget = CreateWidget<UUI_Tutorial>(GetWorld(), UI_Tutorial);	
	if (TutorialWidget)
	{
		TutorialWidget->AddToViewport(10);
	}
}

// Called every frame
void ATutorial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter && PlayerController)
	{
		if (PlayerController->IsInputKeyDown(EKeys::Android_Back))
		{
			TutorialWidget->EndTutorial();
			FTimerHandle THandle;
			GetWorld()->GetTimerManager().SetTimer(THandle, this, &ATutorial::GameStart, 0.33f, false, 0.33f);
		}


		// 튜토리얼 인사
		if (PlayerCharacter->GetActorLocation().Y >= -990 && !bHelloTutorial)
		{
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bHelloTutorial = true;
				TutorialWidget->RemoveAll();
			}
		}


		// 플레이어 정보 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 850 && PlayerCharacter->GetActorLocation().Y >= 750 && !bStateTutorial)
		{
			TutorialWidget->PlayerStateTutorial();
			FreezeWorld(true);
			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bStateTutorial = true;

				TutorialWidget->offImage();
			}
		}


		// 보스 정보 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 1600 && PlayerCharacter->GetActorLocation().Y >= 1500 && !bBossTutorial)
		{
			TutorialWidget->BossTutorial();
			FreezeWorld(true);
			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bBossTutorial = true;

				TutorialWidget->offImage();
			}
		}


		// 기록 정보 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 2350 && PlayerCharacter->GetActorLocation().Y >= 2250 && !bRecordTutorial)
		{
			TutorialWidget->RecordTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				TutorialWidget->RecordTutorial2();
				FreezeWorld(false);
				bRecordTutorial = true;

				TutorialWidget->offImage();
			}
		}


		// 이동 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 4600 && PlayerCharacter->GetActorLocation().Y >= 4500 && !bMoveTutorial)
		{
			TutorialWidget->MoveTutorial();
			FreezeWorld(true);
			
			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bMoveTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 점프 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 7600 && PlayerCharacter->GetActorLocation().Y >= 7500 && !bJumpTutorial)
		{
			TutorialWidget->JumpTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bJumpTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 슬라이드 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 10600 && PlayerCharacter->GetActorLocation().Y >= 10500 && !bSlideTutorial)
		{
			TutorialWidget->SlideTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bSlideTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 로프 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 12800 && PlayerCharacter->GetActorLocation().Y >= 12700 && !bRopeTutorial)
		{
			TutorialWidget->RopeTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bRopeTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 아이템 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 16600 && PlayerCharacter->GetActorLocation().Y >= 16500 && !bItemTutorial)
		{
			TutorialWidget->ItemTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bItemTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 보스 공격 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 19600 && PlayerCharacter->GetActorLocation().Y >= 19500 && !bWarningTutorial)
		{
			TutorialWidget->WarningTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bWarningTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}

		// 설정 튜토리얼
		if (PlayerCharacter->GetActorLocation().Y < 22600 && PlayerCharacter->GetActorLocation().Y >= 22500 && !bSettingTutorial)
		{
			TutorialWidget->SettingTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bSettingTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 튜토리얼 종료 알림
		if (PlayerCharacter->GetActorLocation().Y < 25600 && PlayerCharacter->GetActorLocation().Y >= 25500 && !bEndTutorial)
		{
			TutorialWidget->ByeTutorial();
			FreezeWorld(true);

			if (PlayerController->IsInputKeyDown(EKeys::AnyKey))
			{
				FreezeWorld(false);
				bEndTutorial = true;

				TutorialWidget->RemoveAll();
			}
		}


		// 튜토리얼 종료
		if (PlayerCharacter->GetActorLocation().Y < 28100 && PlayerCharacter->GetActorLocation().Y >= 28000)
		{
			TutorialWidget->EndTutorial();
			FTimerHandle THandle;
			GetWorld()->GetTimerManager().SetTimer(THandle, this, &ATutorial::GameStart, 0.33f, false, 0.33f);
		}
	}
}

void ATutorial::FreezeWorld(bool bFreeze)
{
	PlayerCharacter->bStopTime = bFreeze;
}


void ATutorial::GameStart()
{
	if (GI_GameInstance->GI_bIntro)
	{
		UGameplayStatics::OpenLevel(this, FName("MainMap"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("CineLevel"));
	}
}

