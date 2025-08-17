// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_PauseWidget.h"
#include "CHR_Movement.h"
#include "Components/Button.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "kismet/GameplayStatics.h"


void UUI_PauseWidget::NativeConstruct()
{
    PlayStartAnimation();

    // �̾��ϱ�
    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UUI_PauseWidget::OnContinueClicked);
    }

    // �����
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UUI_PauseWidget::OnRestartClicked);
    }

    // ���θ޴� ȣ��
    if (MenuButton)
    {
        MenuButton->OnClicked.AddDynamic(this, &UUI_PauseWidget::OnMenuClicked);
    }
}

void UUI_PauseWidget::OnContinueClicked()
{
    AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
    if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
    {
        ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
        if (ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(Character))
        {
            if (PlayerCharacter != nullptr)
            {
                // �÷��̾� �ð� �帧 ���� TimeDilation(1.0f)
                PlayerCharacter->bStopTime = false;
                GM->HUDWidgetFunc(true);
                GM->PauseWidgetFunc(false);
            }
        }
    }
}

void UUI_PauseWidget::OnRestartClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainMap"));

    AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
    if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
    {
        GM->ChangeMenuWidget(GM->UI_PlayerHUDWidget);
    }
}

void UUI_PauseWidget::OnMenuClicked()
{
    UGameplayStatics::OpenLevel(this, FName("TitleMenu"));


    AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
    if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
    {
        GM->ChangeMenuWidget(GM->UI_TitleWidget);
    }
}

void UUI_PauseWidget::PlayStartAnimation()
{
    if (PauseWidgetAnimation)
    {
        PlayAnimation(PauseWidgetAnimation);
    }
}