// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/Sound_MasterVolume.h"
#include "UObject/ConstructorHelpers.h"

USound_MasterVolume::USound_MasterVolume()
{
    // 에디터에서 경로 찾기: MasterSoundClass 애셋 경로 사용
    static ConstructorHelpers::FObjectFinder<USoundClass> MasterVolumeFinder(TEXT("/Script/Engine.SoundClass'/Game/Blueprints/Sounds/BP_Sound_MasterVolume.BP_Sound_MasterVolume'"));
    if (MasterVolumeFinder.Succeeded())
    {
        MasterSoundClass = MasterVolumeFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> FXVolumeFinder(TEXT("/Script/Engine.SoundClass'/Game/Blueprints/Sounds/BP_Sound_FXVolume.BP_Sound_FXVolume'"));
    if (FXVolumeFinder.Succeeded())
    {
        MasterSoundClass = FXVolumeFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> BGMVolumeFinder(TEXT("/Script/Engine.SoundClass'/Game/Blueprints/Sounds/BP_Sound_BGMVolume.BP_Sound_BGMVolume'"));
    if (BGMVolumeFinder.Succeeded())
    {
        MasterSoundClass = BGMVolumeFinder.Object;
    }
}

void USound_MasterVolume::SetMasterVolume(float NewVolume)
{
    if (MasterSoundClass)
    {
        // 0.0 ~ 1.0 사이로 제한
        MasterSoundClass->Properties.Volume = FMath::Clamp(NewVolume, 0.0f, 1.0f);
    }
}

void USound_MasterVolume::SetBGMVolume(float Volume)
{
    if (BGMSoundClass)
    {
        BGMSoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
    }
}

void USound_MasterVolume::SetSFXVolume(float Volume)
{
    if (SFXSoundClass)
    {
        SFXSoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
    }
}