// Fill out your copyright notice in the Description page of Project Settings.


#include "Last_Footsteps_GamemodeBase.h"
#include "UI_TitleWidget.h"
#include "UI_Tutorial.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>

void ALast_Footsteps_GamemodeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GameLoaded)
	{
		ChangeMenuWidget(UI_TitleWidget);
		GameLoaded = true;
	}
	else
	{
		if (UI_PlayerHUDWidget)
		{
			HUDWidget = CreateWidget<UUserWidget>(GetWorld(), UI_PlayerHUDWidget);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport(); // 화면에 표시
			}

			PauseWidget = CreateWidget<UUserWidget>(GetWorld(), UI_PauseWidget);
			if (PauseWidget)
			{
				PauseWidget->AddToViewport(); // 화면에 표시
				PauseWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		// ChangeMenuWidget(UI_PlayerHUDWidget);
		GetWorldSettings()->SetTimeDilation(0.25f);
	}
}

void ALast_Footsteps_GamemodeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ALast_Footsteps_GamemodeBase::PlayerDie()
{
	if (HP <= 0)
	{
		ChangeMenuWidget(UI_PlayerDied);
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;
		}
	}
}


void ALast_Footsteps_GamemodeBase::PlayerVictory()
{
	ChangeMenuWidget(UI_PlayerVictory);
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}
}

void ALast_Footsteps_GamemodeBase::HUDWidgetFunc(bool OnOff)
{
	if (OnOff && HUDWidget)
	{
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ALast_Footsteps_GamemodeBase::PauseWidgetFunc(bool OnOff)
{
	if (OnOff && PauseWidget)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PauseWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


