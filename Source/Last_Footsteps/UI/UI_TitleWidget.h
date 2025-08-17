// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_TitleWidget.generated.h"

/**
 * 
 */
class UGI_GameInstance;

UCLASS()
class LAST_FOOTSTEPS_API UUI_TitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UGI_GameInstance* GI_GameInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* StartAnimation;

	void PlayStartAnimation();

	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameSettingButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameQuitButton;

	// 콜백 함수
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnQuitClicked();	
};
