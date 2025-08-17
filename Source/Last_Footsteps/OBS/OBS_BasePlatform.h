#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OBS_BasePlatform.generated.h"

class UBoxComponent;

UCLASS()
class LAST_FOOTSTEPS_API AOBS_BasePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AOBS_BasePlatform();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Triggers")
	UBoxComponent* Trigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Triggers")
	UBoxComponent* ObsSpawnTarget_1;

public:
	UBoxComponent* GetTrigger();
	UBoxComponent* GetSpawnTarget_1();
};