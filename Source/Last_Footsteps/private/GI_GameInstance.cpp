// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_GameInstance.h"
#include "DATA_SaveGame.h"
#include "Sound/Sound_MasterVolume.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

void UGI_GameInstance::Init()
{
	Super::Init();


	USoundClass* FXVolumeFinder = LoadObject<USoundClass>(nullptr, TEXT("/Game/Blueprints/Sounds/BP_Sound_FXVolume.BP_Sound_FXVolume"));
	if (FXVolumeFinder)
	{
		FXSoundClass = FXVolumeFinder;
	}

	USoundClass* BGMVolumeFinder = LoadObject<USoundClass>(nullptr, TEXT("/Game/Blueprints/Sounds/BP_Sound_BGMVolume.BP_Sound_BGMVolume"));
	if (BGMVolumeFinder)
	{
		BGMSoundClass = BGMVolumeFinder;
	}

	GI_SaveSlotName = TEXT("SaveData");
	GI_SaveIndex = 0;

	LoadGame();
	LoadData();
}

void UGI_GameInstance::SaveData()
{
	UDATA_SaveGame* SaveGameInstance = Cast<UDATA_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDATA_SaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		// ���� ������ ���
		// ������ ������ �̸� �� �ε���
		SaveGameInstance->SaveSlotName = GI_SaveSlotName;
		SaveGameInstance->SaveIndex = GI_SaveIndex;

		// ������ ����, ���, ���� ��
		SaveGameInstance->BestScore = GI_BestScore;
		SaveGameInstance->BestRecord = GI_BestRecord;
		SaveGameInstance->AudioBGMVolumeValue = GI_BGMVolumeValue;
		SaveGameInstance->AudioFXVolmueValue = GI_FXVolumeValue;

		//SaveGameInstance->XResolution = GI_XResolution;
		//SaveGameInstance->YResolution = GI_YResolution;
		SaveGameInstance->FullScreen = GI_bFullScreen;
		SaveGameInstance->bTutorial = GI_bTutorial;
		SaveGameInstance->bIntro = GI_bIntro;

		SaveGameInstance->QualityValue = GI_QualityValue;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr"));
	}

	// ���� �� �ε����� ����
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->SaveIndex);
}


void UGI_GameInstance::LoadData()
{
	UDATA_SaveGame* LoadGameInstance = Cast<UDATA_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDATA_SaveGame::StaticClass()));

	if (LoadGameInstance)
	{
		// �ҷ��� ������ ���
		// ����� ������ �̸��� �ε����� ���� �ε���
		LoadGameInstance->SaveSlotName = GI_SaveSlotName;
		LoadGameInstance->SaveIndex = GI_SaveIndex;
		LoadGameInstance = Cast<UDATA_SaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->SaveIndex));

		// �ҷ��� ����, ���, ���� ��
		GI_BestScore = LoadGameInstance->BestScore;
		GI_BestRecord = LoadGameInstance->BestRecord;
		GI_BGMVolumeValue = LoadGameInstance->AudioBGMVolumeValue;
		GI_FXVolumeValue = LoadGameInstance->AudioFXVolmueValue;

		//GI_XResolution = LoadGameInstance->XResolution;
		//GI_YResolution = LoadGameInstance->YResolution;
		GI_bFullScreen = LoadGameInstance->FullScreen;
		GI_QualityValue = LoadGameInstance->QualityValue;

		GI_bTutorial = LoadGameInstance->bTutorial;
		GI_bIntro = LoadGameInstance->bIntro;

		SetBGMVolume(GI_BGMVolumeValue);
		SetFXVolume(GI_FXVolumeValue);
	}
}

void UGI_GameInstance::LoadGame()
{
	// �ҷ��� ������ �̸��� �ε����� �����ϸ� �ε�
	if (UGameplayStatics::DoesSaveGameExist(GI_SaveSlotName, GI_SaveIndex))
	{
		SaveGameData = Cast<UDATA_SaveGame>(UGameplayStatics::LoadGameFromSlot(GI_SaveSlotName, GI_SaveIndex));
	}
	// ���ٸ� �� ���� ��� ����
	else
	{
		SaveData();
		//SaveGameData = Cast<UDATA_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDATA_SaveGame::StaticClass()));
	}
}


void UGI_GameInstance::SetMasterVolume(float NewVolume)
{
	if (MasterSoundClass)
	{
		// 0.0 ~ 1.0 ���̷� ����
		MasterSoundClass->Properties.Volume = FMath::Clamp(NewVolume, 0.0f, 1.0f);
	}
}

void UGI_GameInstance::SetBGMVolume(float Volume)
{
	if (BGMSoundClass)
	{
		BGMSoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
	}
}

void UGI_GameInstance::SetFXVolume(float Volume)
{
	if (FXSoundClass)
	{
		FXSoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
	}
}


void UGI_GameInstance::SetResolution(int32 X, int32 Y, bool Screen)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	// �ػ� ���� ����
	Settings->SetScreenResolution(FIntPoint(X, Y));

	// ��ü ȭ�� ����
	if (Screen)
	{
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	else
	{
		Settings->SetFullscreenMode(EWindowMode::Windowed);
	}
	Settings->ApplySettings(false);
	SaveData();
}


void UGI_GameInstance::SetQuality(int32 QualityValue)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	// ��ü ����Ƽ ����
	Settings->SetOverallScalabilityLevel(QualityValue);


	GI_QualityValue = QualityValue;
	Settings->ApplySettings(false);
	SaveData();
}


