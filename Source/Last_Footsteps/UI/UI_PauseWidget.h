// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UUI_PauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* PauseWidgetAnimation;

	void PlayStartAnimation();

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MenuButton;

	// �ݹ� �Լ�
	UFUNCTION()
	void OnContinueClicked();

	// �ݹ� �Լ�
	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnMenuClicked();
};
