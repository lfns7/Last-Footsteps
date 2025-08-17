// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DATA_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API UDATA_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UDATA_SaveGame();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 SaveIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int BestScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	float BestRecord;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	float AudioBGMVolumeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	float AudioFXVolmueValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 XResolution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 YResolution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	bool FullScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 QualityValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	bool bTutorial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	bool bIntro;

};
