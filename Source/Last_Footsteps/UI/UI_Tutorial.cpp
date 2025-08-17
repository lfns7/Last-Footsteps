// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_Tutorial.h"
#include "GI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"


void UUI_Tutorial::NativeConstruct()
{
	TutorialImageBox = Cast<UImage>(GetWidgetFromName(TEXT("TutorialImageBox")));

	PlayerStateCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("PlayerStateCanvas")));
	BossCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("BossCanvas")));
	RecordCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("RecordCanvas")));
	
	Black = Cast<UImage>(GetWidgetFromName(TEXT("Black")));
	Background = Cast<UBackgroundBlur>(GetWidgetFromName(TEXT("Background")));

	TutorialText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TutorialText")));
}

void UUI_Tutorial::PlayerStateTutorial()
{
	if (PlayerStateCanvas && TutorialText)
	{
		FString Text = FString::Printf(TEXT(""));
		TutorialText->SetText(FText::FromString(Text));

		PlayerStateCanvas->SetVisibility(ESlateVisibility::Visible);
	}
}


void UUI_Tutorial::BossTutorial()
{
	if (PlayerStateCanvas)
	{
		PlayerStateCanvas->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BossCanvas)
	{
		BossCanvas->SetVisibility(ESlateVisibility::Visible);
	}
}


void UUI_Tutorial::RecordTutorial()
{
	if (BossCanvas)
	{
		BossCanvas->SetVisibility(ESlateVisibility::Hidden);
	}


	if (RecordCanvas)
	{
		RecordCanvas->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_Tutorial::RecordTutorial2()
{
	if (RecordCanvas)
	{
		RecordCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UUI_Tutorial::offImage()
{
	if (PlayerStateCanvas && BossCanvas && RecordCanvas)
	{
		PlayerStateCanvas->SetVisibility(ESlateVisibility::Hidden);
		BossCanvas->SetVisibility(ESlateVisibility::Hidden);
		RecordCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UUI_Tutorial::MoveTutorial()
{
	if (TutorialImageBox && TutorialText)
	{
		TutorialImageBox->SetOpacity(1.0f);
		Background->SetVisibility(ESlateVisibility::Visible);
		TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
		TutorialImageBox->SetBrushFromTexture(MoveImage);

		FString Text = FString::Printf(TEXT("[좌, 우 Swipe]\n장애물을 피하지 않으면 HP가 닳고\n1.5초의 무적시간이 주어집니다."));
		TutorialText->SetText(FText::FromString(Text));
	}
}

void UUI_Tutorial::JumpTutorial()
{
	if (TutorialImageBox && TutorialText)
	{
		Background->SetVisibility(ESlateVisibility::Visible);
		TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
		TutorialImageBox->SetBrushFromTexture(JumpImage);

		FString Text = FString::Printf(TEXT("[위로 Swipe]\n점프 중에도 좌, 우 이동이 가능합니다."));
		TutorialText->SetText(FText::FromString(Text));
	}
}

void UUI_Tutorial::RopeTutorial()
{
	if (TutorialImageBox && TutorialText)
	{
		Background->SetVisibility(ESlateVisibility::Visible);
		TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
		TutorialImageBox->SetBrushFromTexture(RopeImage);

		FString Text = FString::Printf(TEXT("[위로 Swipe]\n집중하세요!\n로프를 쓸 수 있는 시간은 매우 짧습니다."));
		TutorialText->SetText(FText::FromString(Text));
	}
}


void UUI_Tutorial::SlideTutorial()
{
	Background->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetBrushFromTexture(SlideImage);

	FString Text = FString::Printf(TEXT("[아래로 Swipe]\n슬라이드 도중에는 점프 및 로프가 불가능합니다."));
	TutorialText->SetText(FText::FromString(Text));
}


void UUI_Tutorial::ItemTutorial()
{
	Background->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetBrushFromTexture(ItemImage);

	FString Text = FString::Printf(TEXT("[양 손으로 Tap]\n여러 아이템은 생존에 도움이 되는 효과를 줍니다.\n보스는 30%%의 확률로 절반의 피해만 입습니다."));
	TutorialText->SetText(FText::FromString(Text));
}


void UUI_Tutorial::WarningTutorial()
{
	Background->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetBrushFromTexture(WarningImage);

	FString Text = FString::Printf(TEXT("일정 시간마다 보스는 거리에 따라 원거리/근거리 공격을 합니다.\n좌, 우 이동 및 점프/슬라이드로 공격을 회피하세요."));
	TutorialText->SetText(FText::FromString(Text));
}

void UUI_Tutorial::SettingTutorial()
{
	Background->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetVisibility(ESlateVisibility::Visible);
	TutorialImageBox->SetBrushFromTexture(SettingImage);

	FString Text = FString::Printf(TEXT("튜토리얼 또는 인트로 재생 여부는 게임 설정에서 On/Off 할 수 있습니다."));
	TutorialText->SetText(FText::FromString(Text));
}


void UUI_Tutorial::ByeTutorial()
{
	FString Text = FString::Printf(TEXT("모든 튜토리얼이 끝났습니다.\n곧 게임이 시작됩니다."));
	TutorialText->SetText(FText::FromString(Text));
}


void UUI_Tutorial::EndTutorial()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	GI_GameInstance = Cast<UGI_GameInstance>(GameInstance);

	GI_GameInstance->GI_bTutorialCheck = true;

	Black->SetVisibility(ESlateVisibility::Visible);
	RemoveAll();
}


void UUI_Tutorial::RemoveAll()
{
	FString RemoveText = FString::Printf(TEXT(" "));

	if (TutorialImageBox && TutorialText)
	{
		Background->SetVisibility(ESlateVisibility::Hidden);
		TutorialImageBox->SetVisibility(ESlateVisibility::Hidden);
		TutorialText->SetText(FText::FromString(RemoveText));
	}
}
