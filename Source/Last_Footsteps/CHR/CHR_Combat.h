#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CHR_Combat.generated.h"

class UNiagaraComponent;

UCLASS()

class LAST_FOOTSTEPS_API ACHR_Combat : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACHR_Combat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//무기 공격력부여*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HitPower = 0.0f;
	float EnemySlowValue = 0.f;

	bool bIsAttack = false;


public:
	//오브젝트 활성여부*
	UPROPERTY()
	bool bActive = false;


public:
	//충돌감지용 구체콜라이더*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	//루트컴포넌트용 Mesh*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mStaticMesh;

	//*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName CurrentWeaponID = "";

	//캐릭터와 충돌 시 처리*
	UFUNCTION()
	void Get_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RopeVisible(bool Visible);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraComponent* WeaponEffect;
};