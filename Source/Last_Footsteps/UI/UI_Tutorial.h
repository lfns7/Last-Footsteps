// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Tutorial.generated.h"


class UImage;
class UTextBlock;
class UBackgroundBlur;
class UCanvasPanel;
class UGI_GameInstance;
/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UUI_Tutorial : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UGI_GameInstance* GI_GameInstance;
	
public:
	// 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* TutorialImageBox;


	// 전체
	UPROPERTY(meta = (BindWidget))
	class UImage* Black;

	UPROPERTY(meta = (BindWidget))
	class UBackgroundBlur* Background;

	// 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TutorialText;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* MoveImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* JumpImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* SlideImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* RopeImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* WarningImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* SettingImage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UCanvasPanel* PlayerStateCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UCanvasPanel* BossCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UCanvasPanel* RecordCanvas;


public:
	// 튜토리얼 출력 부분
	void PlayerStateTutorial();
	void BossTutorial();
	void RecordTutorial();
	void RecordTutorial2();

	void offImage();

	void MoveTutorial();
	void JumpTutorial();
	void RopeTutorial();
	void SlideTutorial();
	void ItemTutorial();
	void WarningTutorial();
	void SettingTutorial();
	void ByeTutorial();

	void EndTutorial();
	void RemoveAll();



};
