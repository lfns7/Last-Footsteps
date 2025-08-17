// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_GameSettingsWidget.generated.h"

class URadialSlider;
class UButton;
class UTextBlock;
class UGI_GameInstance;
class UComboBoxString;
class UCheckBox;
/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UUI_GameSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGI_GameInstance* GI_GameInstance;

protected:
	virtual void NativeConstruct() override;
	
public:
	// 버튼


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ApplyButton;

	UFUNCTION()
	void OnApplyClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CancelButton;

	UFUNCTION()
	void OnCancelClicked();


public:
	// 볼륨

	UFUNCTION()
	void OnBGMVolumeChanged(float Value);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	URadialSlider* BGMVolumeSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BGMVolumeText;
	
	float BGMVolumeValue = 0.5f;
	float TmpBGMVolumeValue = 0.5f;

	UFUNCTION()
	void OnFXVolumeChanged(float Value);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	URadialSlider* FXVolumeSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* FXVolumeText;

	float FXVolumeValue = 0.5f;
	float TmpFXVolumeValue = 0.5f;

public:
	// 해상도 설정

	UFUNCTION()
	void OnResolutionComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* ResolutionCombo;

	UFUNCTION()
	void OnQualityComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* QualityCombo;

	int32 QualityValue = 1;

public:
	// 스킵 박스

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* TutorialSkip;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* IntroSkip;
};
