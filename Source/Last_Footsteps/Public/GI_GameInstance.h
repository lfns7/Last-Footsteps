// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "GI_GameInstance.generated.h"

class UDATA_SaveGame;


UCLASS()
class LAST_FOOTSTEPS_API UGI_GameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	int GI_BestScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	float GI_BestRecord = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	float GI_BGMVolumeValue = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	float GI_FXVolumeValue = 0.5f;

	UDATA_SaveGame* SaveGameData;
	FString GI_SaveSlotName;
	int32 GI_SaveIndex = 0;

	int32 GI_XResolution = 1280;
	int32 GI_YResolution = 720;
	bool GI_bFullScreen = false;
	
	
	bool GI_bTutorial = false;
	bool GI_bIntro = false;
	bool GI_bTutorialCheck = false;


	// 0 : Low , 1 : Medium , 2 : High , 3 : Epic
	int32 GI_QualityValue = 2;

	virtual void Init() override;

public:
	void SaveData();
	void LoadData();
	void LoadGame();


public:
	// Volume Set
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetMasterVolume(float NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetBGMVolume(float NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetFXVolume(float NewVolume);


private:
	// Sound
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundClass* BGMSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundClass* FXSoundClass;

public:
	// Resolution
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetResolution(int32 X, int32 Y, bool Screen);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetQuality(int32 QualityValue);
};
