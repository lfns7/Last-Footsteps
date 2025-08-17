// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerHUD.generated.h"

using namespace std;

class UCanvasPanel;
class UTextBlock;
class UProgressBar;
class UImage;
class UButton;
class ALast_Footsteps_GamemodeBase;
class AENM_Movement;
class ACHR_Movement;
class UGI_GameInstance;

/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UUI_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	UGI_GameInstance* GI_GameInstance;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 공격 경고 UI 노출 함수
	void ShowWarning(FName WarningName);

	// Warning UI 보정용 함수
	//void UpdateWarningPositions(int32 CurrentPlayerLane);
	ALast_Footsteps_GamemodeBase* GM;

	AENM_Movement* BOSS;
	ACHR_Movement* PlayerCharacter;

private:
	// Time
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentSecondTimeText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentMinuteTimeText;

	UPROPERTY(VisibleAnywhere)
	int Minutes;

private:
	// Score
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentScoreText;

	float TempCurrentScore;
	
	void UI_FixScore(float CurrentScore);

private:
	// Player HP

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HP_Foreground;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerHPText;

	const float MAX_HP = 100;

	UPROPERTY(VisibleAnywhere)
	float CurrentHP;

	void UI_FixHPBar(float CurrentHP);

private:
	// Boss HP

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* BossHP_Foreground;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BossHPText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BossDistanceText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* HitEffectImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SuperShoesImage;

	const float BOSS_MAX_HP = 1000;

	UPROPERTY(VisibleAnywhere)
	float BossCurrentHP;

	UPROPERTY(VisibleAnywhere)
	float PlayerToBossDist;

	void UI_FixBossHPBar(float CurrentHP);

private:
	// Icon
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ShieldCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PowerUpCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* RopeCanvas;

private:
	// Pause
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PauseButton;

	UFUNCTION()
	void UI_PauseButtonClicked();

public:
	// Warning Sign
	
	float HUDCurrentLane = 1;
	float WarningTime = 0.0f;
	//vector <int> WarningArea; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TutorialImages")
	TArray<UImage*> TargetImages;
	

	// UMG에서 바인딩된 이미지들
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Throw_Left_Bottom;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Throw_Left_Middle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Throw_Center_Bottom;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Throw_Center_Middle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Throw_Right_Bottom;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Throw_Right_Middle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Melee_Bottom;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Warning_Melee_Middle;


	// Warning Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Throw_Right_Middle_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Throw_Right_Bottom_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Throw_Left_Middle_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Throw_Left_Bottom_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Throw_Center_Middle_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Throw_Center_Bottom_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Melee_Middle_FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Warning_Melee_Bottom_FadeAnim;
};
