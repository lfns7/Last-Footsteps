// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_GameSettingsWidget.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "GI_GameInstance.h"
#include "Components/RadialSlider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"


void UUI_GameSettingsWidget::NativeConstruct()
{
	// 게임 인스턴스 캐스트
	// 볼륨 정보 등 실시간 반영 위함
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	GI_GameInstance = Cast<UGI_GameInstance>(GameInstance);


	// 볼륨 슬라이더 캐스트
	BGMVolumeSlider = Cast<URadialSlider>(GetWidgetFromName(TEXT("BGMVolumeSlider")));
	if (BGMVolumeSlider)
	{
		BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UUI_GameSettingsWidget::OnBGMVolumeChanged);
	}

	BGMVolumeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BGMVolumeText")));


	FXVolumeSlider = Cast<URadialSlider>(GetWidgetFromName(TEXT("FXVolumeSlider")));
	if (FXVolumeSlider)
	{
		FXVolumeSlider->OnValueChanged.AddDynamic(this, &UUI_GameSettingsWidget::OnFXVolumeChanged);
	}

	BGMVolumeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BGMVolumeText")));


	// 각종 버튼 캐스트
	ApplyButton = Cast<UButton>(GetWidgetFromName(TEXT("ApplyButton")));

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UUI_GameSettingsWidget::OnApplyClicked);
	}

	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UUI_GameSettingsWidget::OnCancelClicked);
	}

	// 튜토리얼 체크 박스 캐스트
	IntroSkip = Cast<UCheckBox>(GetWidgetFromName(TEXT("IntroSkip")));
	if (IntroSkip)
	{
		IntroSkip->SetIsChecked(GI_GameInstance->GI_bIntro);
	}

	TutorialSkip = Cast<UCheckBox>(GetWidgetFromName(TEXT("TutorialSkip")));
	if (TutorialSkip)
	{
		TutorialSkip->SetIsChecked(GI_GameInstance->GI_bTutorial);
	}

	// 콤보 버튼 캐스트
	ResolutionCombo = Cast<UComboBoxString>(GetWidgetFromName(TEXT("ResolutionCombo")));
	if (ResolutionCombo)
	{
		FString FullScreenText = "";
		if (GI_GameInstance->GI_bFullScreen)
		{
			FullScreenText = "(FullScreen)";
		}
		else
		{
			FullScreenText = "(Windowed)";
		}

		if (GI_GameInstance->GI_XResolution == 1920)
		{
			ResolutionCombo->SetSelectedOption(TEXT("1920 x 1080 " + FullScreenText));
		}

		else if (GI_GameInstance->GI_XResolution == 1280)
		{
			ResolutionCombo->SetSelectedOption(TEXT("1280 x 720 " + FullScreenText));
		}

		else if (GI_GameInstance->GI_XResolution == 1136)
		{
			ResolutionCombo->SetSelectedOption(TEXT("1136 x 640 " + FullScreenText));
		}

		ResolutionCombo->OnSelectionChanged.AddDynamic(this, &UUI_GameSettingsWidget::OnResolutionComboBoxSelectionChanged);
	}


	QualityCombo = Cast<UComboBoxString>(GetWidgetFromName(TEXT("QualityCombo")));
	if (QualityCombo)
	{
		// 게임 퀄리티 조절
		if (GI_GameInstance->GI_QualityValue == 0)
		{
			QualityCombo->SetSelectedOption(TEXT("Low"));
		}

		else if (GI_GameInstance->GI_QualityValue == 1)
		{
			QualityCombo->SetSelectedOption(TEXT("Medium"));
		}

		else if (GI_GameInstance->GI_QualityValue == 2)
		{
			QualityCombo->SetSelectedOption(TEXT("High"));
		}

		else if (GI_GameInstance->GI_QualityValue == 3)
		{
			QualityCombo->SetSelectedOption(TEXT("Epic"));
		}

		QualityCombo->OnSelectionChanged.AddDynamic(this, &UUI_GameSettingsWidget::OnQualityComboBoxSelectionChanged);
	}
	


	// 게임 인스턴스(이전 저장된 값) 불러와서 그려줌
	if (GI_GameInstance && BGMVolumeSlider && BGMVolumeText)
	{
		BGMVolumeSlider->SetValue(GI_GameInstance->GI_BGMVolumeValue);
		BGMVolumeValue = BGMVolumeSlider->GetValue();
		BGMVolumeText->SetText(FText::AsNumber(FMath::RoundToInt(BGMVolumeValue * 100.0f)));
	}

	TmpBGMVolumeValue = BGMVolumeValue;

	if (GI_GameInstance && FXVolumeSlider && FXVolumeText)
	{
		FXVolumeSlider->SetValue(GI_GameInstance->GI_FXVolumeValue);
		FXVolumeValue = FXVolumeSlider->GetValue();
		FXVolumeText->SetText(FText::AsNumber(FMath::RoundToInt(FXVolumeValue * 100.0f)));
	}

	TmpFXVolumeValue = FXVolumeValue;
}

// BGM 또는 FX 볼륨 변경 부분
void UUI_GameSettingsWidget::OnBGMVolumeChanged(float Value)
{
	if (BGMVolumeSlider && BGMVolumeText)
	{
		BGMVolumeValue = BGMVolumeSlider->GetValue();
		GI_GameInstance->SetBGMVolume(BGMVolumeValue);

		BGMVolumeText->SetText(FText::AsNumber(FMath::RoundToInt(BGMVolumeValue * 100.0f)));
	}
}

void UUI_GameSettingsWidget::OnFXVolumeChanged(float Value)
{
	if (FXVolumeSlider && FXVolumeText)
	{
		// FX 사운드 임의 재생
		USoundBase* SoundFinder = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Button/_Button_button-click_chosic_com_._Button_button-click_chosic_com_"));
		if (SoundFinder)
		{
			UGameplayStatics::PlaySound2D(this, SoundFinder);
		}

		FXVolumeValue = FXVolumeSlider->GetValue();
		GI_GameInstance->SetFXVolume(FXVolumeValue);

		FXVolumeText->SetText(FText::AsNumber(FMath::RoundToInt(FXVolumeValue * 100.0f)));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, FString::SanitizeFloat(FXVolumeValue));
}


void UUI_GameSettingsWidget::OnResolutionComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// 게임 해상도 조절
	FString SelectedResolution = ResolutionCombo->GetSelectedOption();
	if (SelectedResolution == "1920 x 1080 (Windowed)")
	{
		// x : 1920
		// y : 1080
		GI_GameInstance->GI_XResolution = 1920;
		GI_GameInstance->GI_YResolution = 1080;
		GI_GameInstance->GI_bFullScreen = false;
		GI_GameInstance->SetResolution(GI_GameInstance->GI_XResolution, GI_GameInstance->GI_YResolution, GI_GameInstance->GI_bFullScreen);
	}

	else if(SelectedResolution == "1280 x 720 (Windowed)")
	{
		// x : 1280
		// y : 720
		GI_GameInstance->GI_XResolution = 1280;
		GI_GameInstance->GI_YResolution = 720;
		GI_GameInstance->GI_bFullScreen = false;
		GI_GameInstance->SetResolution(GI_GameInstance->GI_XResolution, GI_GameInstance->GI_YResolution, GI_GameInstance->GI_bFullScreen);
	}

	else if(SelectedResolution == "1136 x 640 (Windowed)")
	{
		//x : 1136
		//y : 640
		GI_GameInstance->GI_XResolution = 1136;
		GI_GameInstance->GI_YResolution = 640;
		GI_GameInstance->GI_bFullScreen = false;
		GI_GameInstance->SetResolution(GI_GameInstance->GI_XResolution, GI_GameInstance->GI_YResolution, GI_GameInstance->GI_bFullScreen);
	}

	else if (SelectedResolution == "1920 x 1080 (FullScreen)")
	{
		// x : 1920
		// y : 1080
		GI_GameInstance->GI_XResolution = 1920;
		GI_GameInstance->GI_YResolution = 1080;
		GI_GameInstance->GI_bFullScreen = true;
		GI_GameInstance->SetResolution(GI_GameInstance->GI_XResolution, GI_GameInstance->GI_YResolution, GI_GameInstance->GI_bFullScreen);
	}

	else if (SelectedResolution == "1280 x 720 (FullScreen)")
	{
		// x : 1280
		// y : 720
		GI_GameInstance->GI_XResolution = 1280;
		GI_GameInstance->GI_YResolution = 720;
		GI_GameInstance->GI_bFullScreen = true;
		GI_GameInstance->SetResolution(GI_GameInstance->GI_XResolution, GI_GameInstance->GI_YResolution, GI_GameInstance->GI_bFullScreen);
	}

	else if (SelectedResolution == "1136 x 640 (FullScreen)")
	{
		//x : 1136
		//y : 640
		GI_GameInstance->GI_XResolution = 1136;
		GI_GameInstance->GI_YResolution = 640;
		GI_GameInstance->GI_bFullScreen = true;
		GI_GameInstance->SetResolution(GI_GameInstance->GI_XResolution, GI_GameInstance->GI_YResolution, GI_GameInstance->GI_bFullScreen);
	}
}


void UUI_GameSettingsWidget::OnQualityComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// 게임 퀄리티 조절
	if (SelectedItem == "Low")
	{
		QualityValue = 0;
	}

	else if (SelectedItem == "Medium")
	{
		QualityValue = 1;
	}

	else if (SelectedItem == "High")
	{
		QualityValue = 2;
	}

	else
	{
		QualityValue = 3;
	}

	GI_GameInstance->SetQuality(QualityValue);
}


// 볼륨 변경점 적용 및 인스턴스 호출
void UUI_GameSettingsWidget::OnApplyClicked()
{
	GI_GameInstance->GI_BGMVolumeValue = BGMVolumeValue;
	GI_GameInstance->GI_FXVolumeValue = FXVolumeValue;
	GI_GameInstance->GI_bTutorial = TutorialSkip->IsChecked();
	GI_GameInstance->GI_bIntro = IntroSkip->IsChecked();
	GI_GameInstance->SaveData();

	AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
	if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
	{
		GM->ChangeMenuWidget(GM->UI_TitleWidget);
	}
}


// 설정 취소
void UUI_GameSettingsWidget::OnCancelClicked()
{
	GI_GameInstance->GI_BGMVolumeValue = TmpBGMVolumeValue;
	GI_GameInstance->GI_FXVolumeValue = TmpFXVolumeValue;
	
	GI_GameInstance->SetBGMVolume(TmpBGMVolumeValue);
	GI_GameInstance->SetFXVolume(TmpFXVolumeValue);

	AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
	if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
	{
		GM->ChangeMenuWidget(GM->UI_TitleWidget);
	}
}
