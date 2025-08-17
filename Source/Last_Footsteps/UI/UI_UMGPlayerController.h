// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI_UMGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LAST_FOOTSTEPS_API AUI_UMGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
