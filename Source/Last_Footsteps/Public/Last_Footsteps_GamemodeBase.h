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


	// ���� �� ��ġ ����
public:
	const int MAX_HP = 100;

	UPROPERTY(VisibleAnywhere)
	int HP = 100;
	
	UPROPERTY(VisibleAnywhere)
	float Time = 0.f;

	float GM_Score = 0;
	int GM_SelectedPosition = 1;

	// UI ǥ��
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_TitleWidget; // ������ ���۵� �� ǥ�õ� ����

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_GameSettingsWidget; // ���� ���� ����	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PlayerHUDWidget; // ���� ���� ����	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PauseWidget; // �Ͻ����� ����

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PlayerDied; // �й� ����

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_PlayerVictory; // �¸� ����

	UPROPERTY()
	UUserWidget* CurrentWidget; // ���� ǥ�õǰ� �ִ� ���� ����

	UPROPERTY()
	UUserWidget* HUDWidget; // HUD ���� ����

	UPROPERTY()
	UUserWidget* PauseWidget; // HUD ���� ����


	// ���� ���� ǥ��
public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameState")
	bool GameLoaded = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameState")
	bool GameStated = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameState")
	bool GamePaused = false;

public:
	// ��� ��
	void PlayerDie();

	// �¸� ��
	void PlayerVictory();

	// HUD
	void HUDWidgetFunc(bool OnOff);

	// Pause
	void PauseWidgetFunc(bool OnOff);
};