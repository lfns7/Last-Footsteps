// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerDied.generated.h"

class UButton;
class UTextBlock;
class URichTextBlock;
class UGI_GameInstance;
class ALast_Footsteps_GamemodeBase;

UCLASS()
class LAST_FOOTSTEPS_API UUI_PlayerDied : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UGI_GameInstance* GI_GameInstance;
	ALast_Footsteps_GamemodeBase* GM;
	
	// ����
	float Seconds = 0;
	int Minutes = 0;
	int Score = 0;

	// �ְ�
	float BestSeconds = 0;
	int BestMinutes = 0;
	int BestScore = 0;

	void PlayLoseSound();

public:
	// �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* DiedAnimation;

	void PlayStartAnimation();


public:
	// ��ư
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MenuButton;


	// �ݹ� �Լ�
	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnMenuClicked();


public:
	// �ؽ�Ʈ
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ResultText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ResultText_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NewRecord;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	URichTextBlock* NewRecordRich;
};
