// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundClass.h"
#include "Sound_MasterVolume.generated.h"

/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API USound_MasterVolume : public USoundClass
{
	GENERATED_BODY()
	
public:
    USound_MasterVolume();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetMasterVolume(float NewVolume);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetBGMVolume(float NewVolume);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetSFXVolume(float NewVolume);


private:
    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundClass* BGMSoundClass;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundClass* SFXSoundClass;
};