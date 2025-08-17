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


	//���� ���ݷºο�*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HitPower = 0.0f;
	float EnemySlowValue = 0.f;

	bool bIsAttack = false;


public:
	//������Ʈ Ȱ������*
	UPROPERTY()
	bool bActive = false;


public:
	//�浹������ ��ü�ݶ��̴�*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	//��Ʈ������Ʈ�� Mesh*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mStaticMesh;

	//*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName CurrentWeaponID = "";

	//ĳ���Ϳ� �浹 �� ó��*
	UFUNCTION()
	void Get_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RopeVisible(bool Visible);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraComponent* WeaponEffect;
};