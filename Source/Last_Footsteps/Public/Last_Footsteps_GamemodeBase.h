// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Last_Footsteps_GamemodeBase.generated.h"


/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API ALast_Footsteps_GamemodeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;


	// 게임 내 수치 관련
public:
	const int MAX_HP = 100;

	UPROPERTY(VisibleAnywhere)
	int HP = 100;
	
	UPROPERTY(VisibleAnywhere)
	float Time = 0.f;

	float GM_Score = 0;
	int GM_SelectedPosition = 1;

	// UI 표시
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_TitleWidget; // 게임이 시작될 때 표시될 위젯

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_GameSettingsWidget; // 게임 설정 위젯	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PlayerHUDWidget; // 게임 중의 위젯	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PauseWidget; // 일시정지 위젯

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PlayerDied; // 패배 위젯

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PlayerVictory; // 승리 위젯

	UPROPERTY()
	UUserWidget* CurrentWidget; // 현재 표시되고 있는 위젯 정보

	UPROPERTY()
	UUserWidget* HUDWidget; // HUD 위젯 정보

	UPROPERTY()
	UUserWidget* PauseWidget; // HUD 위젯 정보


	// 게임 상태 표시
public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameState")
	bool GameLoaded = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameState")
	bool GameStated = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameState")
	bool GamePaused = false;

public:
	// 사망 시
	void PlayerDie();

	// 승리 시
	void PlayerVictory();

	// HUD
	void HUDWidgetFunc(bool OnOff);

	// Pause
	void PauseWidgetFunc(bool OnOff);
};