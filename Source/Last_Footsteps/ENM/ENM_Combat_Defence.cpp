// Fill out your copyright notice in the Description page of Project Settings.

#include "ENM_Combat_Defence.h"
#include "Math/UnrealMathUtility.h"

UENM_Combat_Defence::UENM_Combat_Defence()
{
    bIsDefending = false;
}

void UENM_Combat_Defence::TryActivateDefence()
{
    int32 Chance = FMath::RandRange(1, 100);
    if (Chance <= 30) {
        bIsDefending = true;
    }
}
