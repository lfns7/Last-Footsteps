// Fill out your copyright notice in the Description page of Project Settings.


#include "CHR_Movement.h"
#include "CHR_Combat.h"
#include "ENM_Movement.h"
#include <random>
#include "Last_Footsteps_GamemodeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "UI_PlayerHUD.h"
#include "ENM_ThrowAttack_Projectile_Name.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CHR_Interaction.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Camera\CameraActor.h"
#include "InputActionValue.h"
#include "GI_GameInstance.h"
#include "InputAction.h"


ACHR_Movement::ACHR_Movement()
{
	PrimaryActorTick.bCanEverTick = true;

	UCHR_Interaction* ItemDetector = CreateDefaultSubobject<UCHR_Interaction>(TEXT("ItemDetector"));

	//���̷�Ż�޽� ������ ȣ��
	//MeshComponent��ġ, ȸ���� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//MeshComponent��ġ,ȸ������
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// �ȵ���̵� �ۿ��� ��׶���� ���� ���
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &ACHR_Movement::CallMenu);

	GetCharacterMovement()->MaxWalkSpeed = 2500.f;
	GetCharacterMovement()->JumpZVelocity = 500.f;


	// SpringArmComponent ����
	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCamera"));
	SpringArmCamera->SetRelativeLocation(DefaultCameraLocation);
	SpringArmCamera->SetupAttachment(RootComponent);
	SpringArmCamera->bUsePawnControlRotation = false;


	// CameraComponent ����
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->FieldOfView = 103.f;
	MainCamera->SetupAttachment(SpringArmCamera);
	MainCamera->bUsePawnControlRotation = false;

	
	// 1��Ī ����
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(SpringArmCamera);
	FirstPersonMesh->bOnlyOwnerSee = true;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetRelativeLocationAndRotation(FVector(105, -110, -30), FRotator(-30, 70, -90));


	// ���̺� ������Ʈ ����
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	Cable->SetupAttachment(RootComponent); // �Ǵ� ���ϴ� ����

	// �⺻ ���̺� ���� (�ɼ�)
	Cable->CableLength = 10.f;
	Cable->NumSegments = 20;
	Cable->CableWidth = 5.f;
	Cable->SolverIterations = 32;
	Cable->bUseSubstepping = true;
	Cable->bEnableStiffness = true;
	Cable->bEnableCollision = false;
	Cable->EndLocation = FVector::ZeroVector;
	Cable->SetVisibility(false);

	Sensor = CreateDefaultSubobject<UBoxComponent>(TEXT("Sensor"));
	Sensor->SetupAttachment(RootComponent);
	Sensor->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	Sensor->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sensor->SetGenerateOverlapEvents(true);
	Sensor->SetCollisionObjectType(ECC_GameTraceChannel5); // "Sensor" ä��
	Sensor->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sensor->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	Sensor->IgnoreActorWhenMoving(this, true);
}


void ACHR_Movement::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_TPS, 0);
		}
	}

	//ī�޶� ȸ���� ����*
	// PlayerController Cast
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		// EnhancedInput
		UEnhancedInputLocalPlayerSubsystem* SubSystem_2 = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		PlayerController->PlayerCameraManager->ViewPitchMax = 70.0f;
		PlayerController->PlayerCameraManager->ViewPitchMin = -10.0f;
		PlayerController->PlayerCameraManager->ViewYawMax = 60.0f;
		PlayerController->PlayerCameraManager->ViewYawMin = -60.0f;
	}

	//���� ������Ÿ�� ���� ������ ���ε�

	if (Sensor)
	{
		Sensor->OnComponentBeginOverlap.AddDynamic(this, &ACHR_Movement::OnRearSensorOverlap);
	}

	//HUD ĳ�� �߰�
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUI_PlayerHUD::StaticClass(), false);

	if (FoundWidgets.Num() > 0)
	{
		CachedHUD = Cast<UUI_PlayerHUD>(FoundWidgets[0]);
	}
}

// Called to bind functionality to input
void ACHR_Movement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(IA_LOOKUP, ETriggerEvent::Triggered, this, &ACHR_Movement::LookUp);
		PlayerInput->BindAction(IA_JUMP, ETriggerEvent::Started, this, &ACHR_Movement::InputJump);
		PlayerInput->BindAction(IA_ROPE, ETriggerEvent::Started, this, &ACHR_Movement::Rope);
		PlayerInput->BindAction(IA_SLIDE, ETriggerEvent::Started, this, &ACHR_Movement::Slide);

		PlayerInput->BindAction(IA_MOVELEFT, ETriggerEvent::Started, this, &ACHR_Movement::MoveLeft);
		PlayerInput->BindAction(IA_MOVERIGHT, ETriggerEvent::Started, this, &ACHR_Movement::MoveRight);

		PlayerInput->BindAction(IA_ITEM, ETriggerEvent::Started, this, &ACHR_Movement::SpawnAttackItem);

		PlayerInput->BindAction(IA_CALLMENU, ETriggerEvent::Completed, this, &ACHR_Movement::CallMenu);
		PlayerInput->BindAction(IA_THROWWEAPON, ETriggerEvent::Completed, this, &ACHR_Movement::CallThrowWeapon);

		PlayerInputComponent->BindTouch(IE_Pressed, this, &ACHR_Movement::TouchPressed);
		PlayerInputComponent->BindTouch(IE_Repeat, this, &ACHR_Movement::TouchMoved);
		PlayerInputComponent->BindTouch(IE_Released, this, &ACHR_Movement::TouchReleased);
	}
}

void ACHR_Movement::InputJump(const FInputActionValue& inputValue)
{
	if (!bIsTurning && !bIsSliding && !bStopTime)
	{
		USoundBase* JumpSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Jump/Jumping_on_Dirt.Jumping_on_Dirt"));
		if (JumpSound)
		{
			UGameplayStatics::PlaySound2D(this, JumpSound);
		}

		Jump();
	}
}

void ACHR_Movement::Rope()
{
	if (TargetActor && bCanRopeRange && !bIsRoped && !bIsSliding && !bSuperShoes)
	{
		isSwipe = true;
		USoundBase* Rope = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/Rope/22__effort_grunt_male.22__effort_grunt_male"));
		if (Rope)
		{
			UGameplayStatics::PlaySound2D(this, Rope);
		}

		bIsRoped = true;
		Cable->SetVisibility(true);
		// ���̺��� ������ Ÿ�� ���Ϳ� ���̱�
		Cable->SetAttachEndTo(TargetActor, TEXT("DefaultSceneRoot"), NAME_None);
		FVector CableEnd = TargetActor->GetActorLocation(); // ���� �� ��ġ
		FVector MyLoc = GetActorLocation();
		FVector ToCable = (CableEnd - MyLoc);

		float Distance = ToCable.Size();
		float Slack = 5.f; // �� ���� ����
		if (Distance > Cable->CableLength + Slack)
		{
			//FVector PullDir = ToCable.GetSafeNormal();
			float PullForce = 1620.f; // ���� ��
			FVector PullDir = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal(); 
			
			LaunchCharacter(PullDir * PullForce, true, true);
		}
	}
}


void ACHR_Movement::Slide()
{
	if (bIsSliding)
		return;

	if (GetCharacterMovement()->Velocity.Length() > 0.f && !(GetCharacterMovement()->IsFalling()))
	{
		isSwipe = true;
		bIsSliding = true;
		GetCapsuleComponent()->SetCapsuleSize(25.f, 50.f);
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ACHR_Movement::SlideAfter, 0.8f, false);
	}
}

void ACHR_Movement::SlideAfter()
{
	GetCapsuleComponent()->SetCapsuleSize(25.f, 80.f);
	GetMesh()->SetWorldLocation(GetActorLocation() - FVector(0, 0, 50.f));
	bIsSliding = false;
}

void ACHR_Movement::CanRopeRange()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End;
	if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 90.f, 1.f))
	{
		End = Start + FVector(0, 1800.f, 1000.f);
	}

	else if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 0.f, 1.f))
	{
		End = Start + FVector(1800.f, 0, 1000.f);
	}

	else
	{
		End = Start + FVector(-1800.f, 0, 1000.f);
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // �ڽ� ����

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(90.f)
	);
	

	// ���� Ʈ���̼��� ������ ��쿡 ���� ��� ȹ��
	if (bHit)
	{
		AActor* HitPoint = Hit.GetActor();
		if (HitPoint)
		{
			TargetActor = HitPoint;
			RopeHitLocation = HitPoint->GetActorLocation();
			bCanRopeRange = true;
			//GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::Magenta, FString::Printf(TEXT("Rope Chance!")));
		}
	}
	else
	{
		Cable->SetAttachEndTo(nullptr, NAME_None, NAME_None);
		bCanRopeRange = false;
		bIsRoped = false;
		Cable->SetVisibility(false);
	}
}


void ACHR_Movement::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}


void ACHR_Movement::MoveLeft()
{	
	// �� ���� ������ �ƴϸ鼭, ȸ���� �ƴϰ�, ȭ�� ���� ����(�޴�)�� �ƴϸ�, ���� ���⿡ ��ֹ��� ���� �ʴٸ�
	if (CurrentLane != 0 && !bIsTurning && !bStopTime && !bLeftMoveBlocked && CurrentTime >= 0.15f)
	{
		CurrentLane = FMath::Clamp(CurrentLane - 1, 0, 2);
		isSwipe = true;
	}
}

void ACHR_Movement::MoveRight()
{
	if (CurrentLane != 2 && !bIsTurning && !bStopTime && !bRightMoveBlocked && CurrentTime >= 0.15f)
	{
		CurrentLane = FMath::Clamp(CurrentLane + 1, 0, 2);
		isSwipe = true;
	}
}

// �Ͻ����� �޴� ȣ��
void ACHR_Movement::CallMenu()
{
	AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
	if (ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode))
	{
		if (bStopTime)
		{
			//GM->ChangeMenuWidget(GM->UI_PlayerHUDWidget);
			GM->HUDWidgetFunc(true);
			GM->PauseWidgetFunc(false);
			bStopTime = false;
		}
	
		else
		{
			GM->HUDWidgetFunc(false);
			GM->PauseWidgetFunc(true);
			bStopTime = true;
		}
	}
}


void ACHR_Movement::CallThrowWeapon()
{
	if (CurrentWeapon)
	{
		//����߻�
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//������ �޽� ������Ʈ ã��
		auto MeshComp = CurrentWeapon->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp)
		{
			//�����ùķ���ƮȰ��ȭ
			MeshComp->SetSimulatePhysics(true);

			//�߷�����
			MeshComp->SetEnableGravity(true);

			//ĳ���� ���� �ڷ� ��� ���� �����ǰ��
			FVector ThrowDirection = -GetActorForwardVector();

			//��������(�ڷγ�����)
			float ThrowStrength = 6000.0f;
			FVector ThrowVelocity = ThrowDirection * ThrowStrength;

			MeshComp->AddImpulseAtLocation(ThrowVelocity, ThrowDirection);
		}

		CurrentWeaponID = "";
		CurrentWeapon->bIsAttack = true;
		CurrentWeapon->bActive = false;
		SetActorEnableCollision(true);
		CurrentWeapon = nullptr;
	}
}

// ��ġ �Է� ��
void ACHR_Movement::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == 0)
	{
		TouchPressedLocation = FVector2D(Location);
	}
}

// ��ġ ���� ��
void ACHR_Movement::TouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	isSwipe = false;
}

// ��ġ �� ������ ��
void ACHR_Movement::TouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	// �Է��� ��ġ�� 1���� ��� (Swipe ����)
	if (FingerIndex == 0)
	{
		// ��ġ ��ġ üũ
		FVector2D TouchDelta = FVector2D(Location) - TouchPressedLocation;

		float AbsX = FMath::Abs(TouchDelta.X);
		float AbsY = FMath::Abs(TouchDelta.Y);

		float ThresholdDistance = TouchDelta.Size();


		// ��ġ �Ÿ� �� swipe ���� �Ǻ�
		if (ThresholdDistance > MinSwipeDistance && !isSwipe) {
			// �¿� �Ǵ� ���� ���� �Ǻ�
			if (AbsX >= AbsY) {
				if (TouchDelta.X > 0) {
					MoveRight();
				}
				else {
					MoveLeft();
				}
			}

			else {
				if (TouchDelta.Y < 0) {
					if (GetCharacterMovement()->IsMovingOnGround())
					{
						isSwipe = true;
						if (!bIsTurning && !bIsSliding && !bStopTime)
						{
							USoundBase* JumpSoundSwipe = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Jump/Jumping_on_Dirt.Jumping_on_Dirt"));
							if (JumpSoundSwipe)
							{
								UGameplayStatics::PlaySound2D(this, JumpSoundSwipe);
							}
							Jump();
						}
					}

					Rope();
				}
				else
				{
					Slide();
				}
			}
		}
	}
}



// ���� ����
void ACHR_Movement::SpawnAttackItem()
{
	if (!AttackItemClass && !bStopTime)
		return;

	//���� ��ġ
	FVector SpawnLocation = GetActorLocation() - GetActorForwardVector() * 100.0f + FVector(0, 0, 50.0f);
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(AttackItemClass, SpawnLocation, SpawnRotation, SpawnParams);
}


void ACHR_Movement::changeLane()
{
	DestLocation = GetActorLocation();

	// �� ���� ������ ���� �������� ��
	// ����
	if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 90.f, 1.f))
	{
		switch (CurrentLane)
		{
		case 0:
			DestLocation.X = CurrentPlatformInfo.X + 250.f;
			break;

		case 1: 
			DestLocation.X = CurrentPlatformInfo.X;
			break;

		case 2:
			DestLocation.X = CurrentPlatformInfo.X - 250.f;
			break;
		}
	}

	// ����
	else if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 0.f, 1.f))
		switch (CurrentLane)
		{
		case 0:
			DestLocation.Y = CurrentPlatformInfo.Y - 250.f;
			break;

		case 1: 
			DestLocation.Y = CurrentPlatformInfo.Y;
			break;

		case 2:
			DestLocation.Y = CurrentPlatformInfo.Y + 250.f;
			break;
		}

	// ����
	else
	{
		switch (CurrentLane)
		{
		case 0:
			DestLocation.Y = CurrentPlatformInfo.Y + 250.f;
			break;

		case 1:
			DestLocation.Y = CurrentPlatformInfo.Y;
			break;

		case 2:
			DestLocation.Y = CurrentPlatformInfo.Y - 250.f;
			break;
		}
	}

	LaneChangeLerp();
}

// Called every frame
void ACHR_Movement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(UGameplayStatics::GetGameMode(this));
	GM->Time += DeltaTime;

	//�ڵ��̵�ó��
	FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, RunSpeed);

	
	// �帥 �ð� ȹ��
	CurrentTime += DeltaTime;

	FindCurrentPlatform();
	changeLane();
	LaneChangeLerp();

	// ���� �տ� ���
	if (CurrentWeapon && !bIsRoped)
	{
		FirstPersonMesh->SetVisibility(true);
		if (CurrentWeapon)
		{
			CurrentWeapon->RopeVisible(true);
		}
	}
	else
	{
		// ����
		FirstPersonMesh->SetVisibility(false);
		if (CurrentWeapon)
		{
			CurrentWeapon->RopeVisible(false);
		}
	}

	if (GM->HP <= 30)
	{
		if (!bHeartBeat)
		{
			USoundBase* Beat = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/OtherFX/Heartbeat.Heartbeat"));
			if (Beat)
			{
				UGameplayStatics::PlaySound2D(this, Beat);
				bHeartBeat = true;
				GetWorldTimerManager().SetTimer(HeartTimer, this, &ACHR_Movement::HeartBeat, 10.f, true, 0.75f);
			}
		}
	}
	else
	{
		if (bHeartBeat)
		{
			GetWorldTimerManager().ClearTimer(HeartTimer);
			bHeartBeat = false;
		}
	}

	// ���� ȹ�� ����Ʈ
	if (bShield)
	{
		if (bShieldEffect)
		{
			bShieldEffect = false;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACHR_Movement::CallShieldEffect, 1.0f, false, 0.0f);
		}
	}
	
	// �Ŀ��� ȹ�� ����Ʈ
	if (bPowerUp)
	{
		if (bPowerUpEffect)
		{
			bPowerUpEffect = false;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACHR_Movement::CallPowerUpEffect, 1.0f, false, 0.0f);
		}
	}
	
	// ���� ������ Z�� �Ÿ�
	if (GetActorLocation().Z < 425)
		CanRopeRange();


	// ī�޶� ����κ�
	if (bIsSliding && !bIsRoped)
	{
		TargetCameraLocation = FVector(235.0f, 0, 20.0f);
		SwitchCamera(2);
	}
	else if (bIsRoped && !bIsSliding)
	{
		TargetCameraLocation = FVector(-125.0f, 0, 120.0f);
		SwitchCamera(3);
	}
	else if (bSuperShoes)
	{
		TargetCameraLocation = FVector(400.0f, 0, 40.0f);
		SwitchCamera(4);
	}
	else
	{
		TargetCameraLocation = FMath::VInterpTo(TargetCameraLocation, DefaultCameraLocation, GetWorld()->DeltaTimeSeconds, 3.f);
		SwitchCamera(1);
	}


	// ���� / �Ͻ����� �� ���ο�
	if (SlowTime < 1.0f)
	{
		SlowTime += DeltaTime * 0.4f;
		GetWorld()->GetWorldSettings()->SetTimeDilation(SlowTime);
	}

	else
	{
		SlowTime = 1.0f;
	}

	// �޴� ȣ�� �� 
	if (bStopTime)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.f);
	}

	else if (!bStopTime && SlowTime >= 1.0f)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	}


	invincibility -= DeltaTime;

	// �ǰ� ó�� �κ�
	if (bDamaged && invincibility <= 0 && !bSuperShoes)
	{
		CharacterDamaged();
	}

	else if(!bDamaged && invincibility <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2500.f;
		bSuperShoes = false;
	}

	else if (bSuperShoes && invincibility > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 3000.f;
	}
}

// ���� ���� �� ��ġ ����
void ACHR_Movement::LaneChangeLerp()
{
	// ���� �̵��� ���� ����
	if (FMath::IsNearlyEqual(GetActorRotation().Yaw, 90.f, 1.f))
	{
		TempLocation = FVector(FMath::FInterpTo(GetActorLocation().X, DestLocation.X, GetWorld()->DeltaTimeSeconds, 7.5f), GetActorLocation().Y, GetActorLocation().Z);
		SetActorRelativeLocation(TempLocation);
	}

	// ���� �Ǵ� �������� ���� ����
	else if ((FMath::IsNearlyEqual(GetActorRotation().Yaw, 180.f, 1.f) || FMath::IsNearlyEqual(GetActorRotation().Yaw, 0.f, 1.f)))
	{
		TempLocation = FVector(GetActorLocation().X, FMath::FInterpTo(GetActorLocation().Y, DestLocation.Y, GetWorld()->DeltaTimeSeconds, 7.5f), GetActorLocation().Z);
		SetActorRelativeLocation(TempLocation);
	}
}

// ���� Ʈ���̽��� ����ؼ� ���� �÷��̾ ��� �÷����� ��ġ�� �ִ��� �� ȸ������ ���� ������ ���ϴ� �Լ�
void ACHR_Movement::FindCurrentPlatform()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 600.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // �ڽ� ����

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	// ���� Ʈ���̼��� ������ ��쿡 ���� ��� ȹ��
	if (bHit && Hit.GetActor())
	{
		AActor* FloorActor = Hit.GetActor();
		CurrentPlatformInfo = FloorActor->GetActorLocation();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *CurrentPlatformInfo.ToString()));
}


// ����� �Ծ��� ���� ����
void ACHR_Movement::CharacterDamaged()
{
	ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(UGameplayStatics::GetGameMode(this));

	if (bSuperShoes)
	{
		return;
	}

	//���� Ȱ��ȭ ���¶�� ������ ��ȿȭ
	if (bShield)
	{
		USoundBase* ShieldDamage = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/OtherFX/Shield.Shield"));
		if (ShieldDamage)
		{
			UGameplayStatics::PlaySound2D(this, ShieldDamage);
		}

		bShield = false;
		bDamaged = false;
		invincibility = 1.5f;
		//��ȿȭ�ϱ�
		return;
	}

	invincibility = 1.5f;
	bDamaged = false;
	CallDamagedEffect();

	// �ð������� �������� �������� �����Ƿ� SlowTime�� ����
	GetCharacterMovement()->MaxWalkSpeed = 2250.f;
	SlowTime = 0.66f;

	// ���Ӹ�忡�� �����ϴ� HP ����
	GM->HP -= 10;

	// ��� ó��(�й�)
	if (GM->HP <= 0)
	{
		GM->PlayerDie();
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 4);
	auto RandResult = dist(mt); 

	switch (RandResult)
	{
		case 0 :
		{
			USoundBase* Damaged_1 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/Damaged/_PlayerDamaged_DamageGrunts_1._PlayerDamaged_DamageGrunts_1"));
			if (Damaged_1)
			{
				UGameplayStatics::PlaySound2D(this, Damaged_1);
			}
			break;
		}

		case 1 :
		{
			USoundBase* Damaged_2 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/Damaged/_PlayerDamaged_DamageGrunts_2._PlayerDamaged_DamageGrunts_2"));
			if (Damaged_2)
			{
				UGameplayStatics::PlaySound2D(this, Damaged_2);
			}
			break;
		}

		case 2 :
		{
			USoundBase* Damaged_3 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/Damaged/_PlayerDamaged_DamageGrunts_3._PlayerDamaged_DamageGrunts_3"));
			if (Damaged_3)
			{
				UGameplayStatics::PlaySound2D(this, Damaged_3);
			}
			break;
		}

		case 3 :
		{
			USoundBase* Damaged_4 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/Damaged/_PlayerDamaged_DamageGrunts_4._PlayerDamaged_DamageGrunts_4"));
			if (Damaged_4)
			{
				UGameplayStatics::PlaySound2D(this, Damaged_4);
			}
			break;
		}

		case 4 :
		{
			USoundBase* Damaged_5 = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/Damaged/_PlayerDamaged_DamageGrunts_5._PlayerDamaged_DamageGrunts_5"));
			if (Damaged_5)
			{
				UGameplayStatics::PlaySound2D(this, Damaged_5);
			}
			break;
		}
	}
}


void ACHR_Movement::HeartBeat()
{
	bHeartBeat = false;
}


void ACHR_Movement::CallPowerUpEffect()
{
	if (!PowerUpNiagara)
		return;

	FVector NiagaraVector = FVector::ZeroVector;
	FRotator NiagaraRotator = FRotator::ZeroRotator;
	if (!bIsSliding)
	{
		NiagaraVector = FVector(0.f, 0.f, -45.f);
		NiagaraRotator = FRotator(0.f, -90.f, -90.f);
	}

	UNiagaraComponent* PowerUpNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		PowerUpNiagara,
		GetMesh(),
		FName("pelvis"),
		NiagaraVector,
		NiagaraRotator,
		EAttachLocation::SnapToTarget,
		true
	);
}

void ACHR_Movement::CallShieldEffect()
{
	if (!ShieldNiagara)
		return;

	FVector NiagaraVector = FVector::ZeroVector;
	FRotator NiagaraRotator = FRotator::ZeroRotator;
	if (!bIsSliding)
	{
		NiagaraVector = FVector(0.f, 0.f, -45.f);
		NiagaraRotator = FRotator(0.f, -90.f, -90.f);
	}

	UNiagaraComponent* ShieldNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		ShieldNiagara,
		GetMesh(),
		FName("pelvis"),
		NiagaraVector,
		NiagaraRotator,
		EAttachLocation::SnapToTarget,	 
		true									 
	);
}

void ACHR_Movement::CallDamagedEffect()
{
	if (!DamagedNiagara)
		return;

	if (DamagedCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DamagedCameraShakeClass);
	}

	UNiagaraComponent* DamagedComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		DamagedNiagara,				     // ����� ���̾ư��� �ý���
		GetMesh(),							 // ���� ��� (Skeletal Mesh ��)
		FName("spine_04"),					 // ���� �� �̸� 
		FVector(0.f, 0.f, 0.f),			 // ������ ��ġ
		FRotator(0.f, 0.f, 0.f),			 // ������ ȸ��
		EAttachLocation::SnapToTarget,	 // �θ� ��ġ�� �������� ��Ȯ�� ����
		true									 // �ڵ� �ı� ���� (false�� �������� �� �� ����)
	);
}


// ī�޶� ��ȯ
void ACHR_Movement::SwitchCamera(int index)
{
	switch (index)
	{
	// default
	case 1:
		SpringArmCamera->SetRelativeLocation(DefaultCameraLocation);
		break;

	// slide
	case 2:
		SpringArmCamera->SetRelativeLocation(FMath::VInterpTo(DefaultCameraLocation, TargetCameraLocation, GetWorld()->DeltaTimeSeconds, 75.f));
		break;

	// rope
	case 3:
		SpringArmCamera->SetRelativeLocation(FMath::VInterpTo(DefaultCameraLocation, TargetCameraLocation, GetWorld()->DeltaTimeSeconds, 550.f));
		break;

	// SuperShoes
	case 4:
		SpringArmCamera->SetRelativeLocation(FMath::VInterpTo(DefaultCameraLocation, TargetCameraLocation, GetWorld()->DeltaTimeSeconds, 550.f));
		break;
	}
}

//�Ĺ� ī�޶� �۵� �� Warning UI ȣ��
void ACHR_Movement::OnRearSensorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("[2] Sensor Overlap with: %s"), *OtherActor->GetName());
	if (!OtherActor || OtherActor == this)
		return;

	if (AENM_ThrowAttack_Projectile_Name* Projectile = Cast<AENM_ThrowAttack_Projectile_Name>(OtherActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[2.1] Projectile WarningName at Overlap: %s"), *Projectile->WarningName.ToString());
		//�ߺ� ó��
		if (Projectile->bRearSensorDetected)
		{
			return;
		}

		// WarningName ������ ������ ��ٸ�
		if (!Projectile->bIsInitialized)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Projectile not initialized yet: %s / Actor Name: %s"), *Projectile->WarningName.ToString(), *OtherActor->GetName());
			return;
		}
		
		Projectile->bRearSensorDetected = true;
		//UE_LOG(LogTemp, Warning, TEXT("Projectile WarningName: %s"), *Projectile->WarningName.ToString());

		if (CachedHUD)
		{
			CachedHUD->ShowWarning(Projectile->WarningName);
			//UE_LOG(LogTemp, Warning, TEXT("[4] ShowWarning() Called with: %s / OtherActor Name: %s"), *Projectile->WarningName.ToString(), *OtherActor->GetName());
		}
	}
}