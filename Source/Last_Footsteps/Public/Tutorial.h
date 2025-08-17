// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tutorial.generated.h"

class ACHR_Movement;
class APlayerController;
class ALast_Footsteps_GamemodeBase;
class UGI_GameInstance;
class UUI_Tutorial;

UCLASS()
class LAST_FOOTSTEPS_API ATutorial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorial();

public:
	UGI_GameInstance* GI_GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bHelloTutorial = false;
	bool bStateTutorial = false;
	bool bBossTutorial = false;
	bool bRecordTutorial = false;
	bool bMoveTutorial = false;
	bool bJumpTutorial = false;
	bool bSlideTutorial = false;
	bool bRopeTutorial = false;
	bool bItemTutorial = false;
	bool bWarningTutorial = false;
	bool bSettingTutorial = false;
	bool bEndTutorial = false;


	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UI_Tutorial; // Æ©Åä¸®¾ó À§Á¬

	UPROPERTY()
	UUI_Tutorial* TutorialWidget;


	void FreezeWorld(bool bFreeze);
	void GameStart();

	ACHR_Movement* PlayerCharacter;
	APlayerController* PlayerController;
	ALast_Footsteps_GamemodeBase* GM;
};
