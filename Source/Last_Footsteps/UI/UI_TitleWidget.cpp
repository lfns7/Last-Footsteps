// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_TitleWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GI_GameInstance.h"
#include "Last_Footsteps_GamemodeBase.h"

void UUI_TitleWidget::NativeConstruct()
{
	PlayStartAnimation();
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
    GI_GameInstance = Cast<UGI_GameInstance>(GameInstance);


    if (GameStartButton)
    {
        GameStartButton->OnClicked.AddDynamic(this, &UUI_TitleWidget::OnStartClicked);
    }

    if (GameSettingButton)
    {
        GameSettingButton->OnClicked.AddDynamic(this, &UUI_TitleWidget::OnSettingsClicked);
    }

    if (GameQuitButton)
    {
        GameQuitButton->OnClicked.AddDynamic(this, &UUI_TitleWidget::OnQuitClicked);
    }
}

void UUI_TitleWidget::OnStartClicked()
{

    if (GI_GameInstance->GI_bTutorial && !GI_GameInstance->GI_bIntro)
    {
        UGameplayStatics::OpenLevel(this, FName("CineLevel"));
    }
    else if (GI_GameInstance->GI_bIntro && GI_GameInstance->GI_bTutorial)
    {
        UGameplayStatics::OpenLevel(this, FName("MainMap"));
    }
    else
    {
        UGameplayStatics::OpenLevel(this, FName("Tutorial"));
    }
}

void UUI_TitleWidget::OnSettingsClicked()
{
    AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
    if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
    {
        GM->ChangeMenuWidget(GM->UI_GameSettingsWidget);
    }
}

void UUI_TitleWidget::OnQuitClicked()
{
    // 게임 종료
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void UUI_TitleWidget::PlayStartAnimation()
{
	if (StartAnimation)
	{
		PlayAnimation(StartAnimation);
	}
}

