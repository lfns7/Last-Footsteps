// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "CHR_Movement.generated.h"


class UCableComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UGI_GameInstance;
class UCombat;
class UUI_PlayerHUD;
class UBoxComponent;
struct FInputActionInstance;


UCLASS()
class LAST_FOOTSTEPS_API ACHR_Movement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACHR_Movement();

	FTimerHandle TimerHandle;

	float SlowTime = 0.33f;
	bool bStopTime = false;


public:
	// ����
	FName WeaponID = "";
	bool bGetWeapon = false;

	//�տ�����ִ� ������Ʈ ����*
	UPROPERTY()
	class ACHR_Combat* CurrentWeapon;


	// ī�޶� ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultCameraLocation = FVector(235.0f, 0, 40.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetCameraLocation = FVector(235.0f, 0, 40.0f);

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;


	UFUNCTION()
	void SwitchCamera(int index);

	// ī�޶� ����ũ
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> DamagedCameraShakeClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �÷��̾ ��ġ�ϰ� �ִ� ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lane", meta = (AllowPrivateAccess = "true"))
	int CurrentLane = 1;

	const float LaneOffset = 200.0f;
	const int NumLanes = 3;
	const float LaneChangeSpeed = 10.0f;

	FVector TargetLaneLocation;

	float CurrentTime;
	void changeLane();


	// ī�޶� UPROPERTY
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* SpringArmCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* MainCamera;


public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imc_TPS;

	FVector direction;



	// InputAction UPROPERTY
protected:
	//ĳ���� �ڵ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float RunSpeed = 25000.0f;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_JUMP;
	void InputJump(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_ROPE;
	void Rope();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SLIDE;
	void Slide();

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_MOVELEFT;
	void MoveLeft();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_MOVERIGHT;
	void MoveRight();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LOOKUP;
	void LookUp(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_THROWWEAPON;
	void CallThrowWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_MobileSwipe;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ITEM;

	FVector2D TouchPressedLocation;
	bool isSwipe = false;

	UPROPERTY(EditAnywhere, Category = EndlessRunCoreSettings, meta = (DisplayName = "Minimum Swipe Distance"))
	float MinSwipeDistance = 15;

	void TouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchMoved(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_CALLMENU;
	void CallMenu();

public:
	// �̵� ó�� �κ�
	// �̵��� ���� ��ġ �� �ӽ� ��ġ ����
	FVector TempLocation;
	FVector DestLocation;

	void LaneChangeLerp();
	void FindCurrentPlatform();


	// ���� Ʈ���̼����� ���� �÷��̾ �ִ� ��ġ�� �˱� ���� ����
	FVector CurrentPlatformInfo;
	FTimerHandle DelayTimerHandle;

	// �÷��̾��� ���� ������ ���� bool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsMoved = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsTurning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsSliding;

	void SlideAfter();

public:
	// ���� ó��
	void CanRopeRange();
	FVector RopeHitLocation;
	
	bool bCanRopeRange = false;
	bool bIsRoped = false;
	bool bRopeMaxHeight = false;

	// ���̺� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cable")
	UCableComponent* Cable;

	// ��ǥ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cable")
	AActor* TargetActor;


	
public:
	//*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName CurrentWeaponID = "";

	UPROPERTY(EditAnywhere, Category = "AttackItem")
	TSubclassOf<AActor> AttackItemClass;

	// ������ ���� �Լ�
	void SpawnAttackItem();


public:
	// �ǰ� �κ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damaged")
	UNiagaraSystem* DamagedNiagara;

	bool bLeftMoveBlocked = false;
	bool bRightMoveBlocked = false;
	bool bDamaged = false;
	bool bHeartBeat = false;

	void CallDamagedEffect();
	void CharacterDamaged();
	void HeartBeat();

	float invincibility = 0.f;

	FTimerHandle HeartTimer;


	
// ������ ����
public:
	bool bPowerUp = false;
	bool bShield = false;

	bool bPowerUpEffect = false;
	bool bShieldEffect = false;

	bool bSuperShoes = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UNiagaraSystem* PowerUpNiagara;
	void CallPowerUpEffect();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UNiagaraSystem* ShieldNiagara;
	void CallShieldEffect();


public:
	// HUD ĳ�̿� ������
	UPROPERTY()
	UUI_PlayerHUD* CachedHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
	UBoxComponent* Sensor;

	UPROPERTY()
	bool bIsRearCameraOn = false;

	UFUNCTION()
	void OnRearSensorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
