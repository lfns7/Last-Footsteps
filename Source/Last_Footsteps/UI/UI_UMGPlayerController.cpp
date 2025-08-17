// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_UMGPlayerController.h"

void AUI_UMGPlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetInputMode(FInputModeGameAndUI());
}
