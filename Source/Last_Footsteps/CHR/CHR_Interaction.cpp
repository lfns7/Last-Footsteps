// Fill out your copyright notice in the Description page of Project Settings.


#include "CHR_Interaction.h"
#include "CHR_Movement.h"
#include "GameFramework/Actor.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

UCHR_Interaction::UCHR_Interaction()
{
	PrimaryComponentTick.bCanEverTick = true;

	// BoxComponent 생성
	ItemCatcher = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCatcher"));
	ItemCatcher->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	ItemCatcher->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemCatcher->SetCollisionObjectType(ECC_GameTraceChannel4);
	ItemCatcher->SetCollisionResponseToAllChannels(ECR_Overlap);
	ItemCatcher->SetGenerateOverlapEvents(true);
}

// Called when the game starts
void UCHR_Interaction::BeginPlay()
{
	Super::BeginPlay();

	if (ItemCatcher)
	{
		// CHR_Movement에 ItemCatcher 부착
		AActor* Owner = GetOwner();
		if (Owner && Owner->GetRootComponent())
		{
			ItemCatcher->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		ItemCatcher->OnComponentBeginOverlap.AddDynamic(this, &UCHR_Interaction::GetItem_OnOverlapBegin);
	}
}


void UCHR_Interaction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindLeftObstacle();
	FindRightObstacle();
	FindForwardObstacle();
}

// 왼쪽 방향 장애물 탐지
void UCHR_Interaction::FindLeftObstacle()
{
	// 플레이어 캐릭터 캐스트
	APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(Character);

	if (!PlayerCharacter)
	{
		return;
	}

	FHitResult Hit;
	FVector Start = PlayerCharacter->GetActorLocation();
	FVector End;

	// 정면
	if (FMath::IsNearlyEqual(PlayerCharacter->GetActorRotation().Yaw, 90.f, 1.f))
	{
		End = Start + FVector(250.f, 0.f, 0.f);
	}

	// 좌측
	else if (FMath::IsNearlyEqual(PlayerCharacter->GetActorRotation().Yaw, 0.f, 1.f))
	{
		End = Start + FVector(0.f, -250.f, 0.f);
	}

	// 우측
	else
	{
		End = Start + FVector(0.f, 250.f, 0.f);
	}

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(2.0f)
	);

	// 라인 트레이서가 적중한 경우에 대한 결과 획득
	if (bHit)
	{
		PlayerCharacter->bLeftMoveBlocked = true;
	}
	else
	{
		PlayerCharacter->bLeftMoveBlocked = false;
	}
}

void UCHR_Interaction::FindRightObstacle()
{
	APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(Character);

	if (!PlayerCharacter)
	{
		return;
	}

	FHitResult Hit;
	FVector Start = PlayerCharacter->GetActorLocation();
	FVector End;

	// 정면
	if (FMath::IsNearlyEqual(PlayerCharacter->GetActorRotation().Yaw, 90.f, 1.f))
	{
		End = Start + FVector(-250.f, 0.f, 0.f);
	}

	// 좌측
	else if (FMath::IsNearlyEqual(PlayerCharacter->GetActorRotation().Yaw, 0.f, 1.f))
	{
		End = Start + FVector(0.f, 250.f, 0.f);
	}

	// 우측
	else
	{
		End = Start + FVector(0.f, -250.f, 0.f);
	}

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(2.0f)
	);

	// 라인 트레이서가 적중한 경우에 대한 결과 획득
	if (bHit)
	{
		PlayerCharacter->bRightMoveBlocked = true;
	}
	else
	{
		PlayerCharacter->bRightMoveBlocked = false;
	}
}


void UCHR_Interaction::FindForwardObstacle()
{
	APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(Character);

	if (!PlayerCharacter)
	{
		return;
	}
	FHitResult Hit;
	FVector Start = PlayerCharacter->GetActorLocation() - FVector(0.f, 0.f, -60.f);
	FVector End;

	End = Start + FVector(0.f, 0.f, -100.f);
	if (PlayerCharacter->bIsSliding)
	{
		End = Start + FVector(0.f, 0.f, -80.f);
	}

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(15.f)
	);

		//DrawDebugCapsule(
		//GetWorld(),
		//(Start + End) / 2,
		//(End - Start).Size() / 2,
		//15.f,
		//FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
		//FColor::White,
		//false,
		//1.f
		//	);

	// 라인 트레이서가 적중한 경우에 대한 결과 획득
	if (bHit)
	{
		if (!(PlayerCharacter->bDamaged) && PlayerCharacter->invincibility <= 0)
		{
			PlayerCharacter->bDamaged = true;
		}
			
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Forward Overlaped")));
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
	}
}


void UCHR_Interaction::GetItem_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// GM
	AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
	ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(BaseMode);

	// CHR
	APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ACHR_Movement* PlayerCharacter = Cast<ACHR_Movement>(Character);

	if (OtherActor && OtherActor != GetOwner())
	{
		FString OtherName = OtherActor->GetName();

		if (OtherName.Contains(TEXT("ITEM_Score_Coin")))
		{
			USoundBase* CoinGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/GetItem/mixkit-coins-sound-2003.mixkit-coins-sound-2003"));
			if (CoinGet)
			{
				UGameplayStatics::PlaySound2D(this, CoinGet);
			}
			GM->GM_Score += 100 * ((1 + (int)GM->Time / 10) * 1.1f);
			OtherActor->Destroy();
		}

		else if (OtherName.Contains(TEXT("ITEM_Health_Low")))
		{
			USoundBase* HeartGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/GetItem/Heartbeat-1_chosic_com_.Heartbeat-1_chosic_com_"));
			if (HeartGet)
			{
				UGameplayStatics::PlaySound2D(this, HeartGet);
			}
			GM->HP += (GM->MAX_HP * 0.05);
			if (GM->HP >= 100)
				GM->HP = 100;
			OtherActor->Destroy();
		}

		else if (OtherName.Contains(TEXT("ITEM_Health_Large")))
		{
			USoundBase* HeartGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/GetItem/Heartbeat-1_chosic_com_.Heartbeat-1_chosic_com_"));
			if (HeartGet)
			{
				UGameplayStatics::PlaySound2D(this, HeartGet);
			}
			GM->HP += (GM->MAX_HP * 0.15);
			if (GM->HP >= 100)
				GM->HP = 100;
			OtherActor->Destroy();
		}

		else if (OtherName.Contains(TEXT("ITEM_PowerUp")))
		{
			USoundBase* PowerUpGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/GetItem/mixkit-laser-weapon-shot-1681.mixkit-laser-weapon-shot-1681"));
			if (PowerUpGet)
			{
				UGameplayStatics::PlaySound2D(this, PowerUpGet);
			}
			PlayerCharacter->bPowerUp = true;
			PlayerCharacter->bPowerUpEffect = true;
			OtherActor->Destroy();
		}

		else if (OtherName.Contains(TEXT("ITEM_Shield")))
		{
			USoundBase* ShieldGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Grunts/Player/GetItem/mixkit-sci-fi-metallic-reveal-or-shield-887.mixkit-sci-fi-metallic-reveal-or-shield-887"));
			if (ShieldGet)
			{
				UGameplayStatics::PlaySound2D(this, ShieldGet);
			}
			PlayerCharacter->bShield = true;
			PlayerCharacter->bShieldEffect = true;
			OtherActor->Destroy();
		}

		else if (OtherName.Contains(TEXT("ITEM_SuperShoes")))
		{
			USoundBase* ShoesGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/OtherFX/Wind.Wind"));
			if (ShoesGet)
			{
				UGameplayStatics::PlaySound2D(this, ShoesGet);
			}
			PlayerCharacter->invincibility = 2.0f;
			PlayerCharacter->bSuperShoes = true;
			OtherActor->Destroy();
		}

		else if (OtherName.Contains(TEXT("ITEM_Weapon_Dynamite")))
		{
			if (!PlayerCharacter->bGetWeapon && PlayerCharacter->WeaponID == "")
			{
				USoundBase* ItemGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/GetItem.GetItem"));
				if (ItemGet)
				{
					UGameplayStatics::PlaySound2D(this, ItemGet);
				}
				PlayerCharacter->WeaponID = "Dynamite";
				PlayerCharacter->bGetWeapon = true;
			}
		}

		else if (OtherName.Contains(TEXT("ITEM_Weapon_Grenade")))
		{
			if (!PlayerCharacter->bGetWeapon && PlayerCharacter->WeaponID == "")
			{
				USoundBase* ItemGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/GetItem.GetItem"));
				if (ItemGet)
				{
					UGameplayStatics::PlaySound2D(this, ItemGet);
				}
				PlayerCharacter->WeaponID = "Grenade";
				PlayerCharacter->bGetWeapon = true;
			}
		}

		else if (OtherName.Contains(TEXT("ITEM_Weapon_Molotov")))
		{
			if (!PlayerCharacter->bGetWeapon && PlayerCharacter->WeaponID == "")
			{
				USoundBase* ItemGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/GetItem.GetItem"));
				if (ItemGet)
				{
					UGameplayStatics::PlaySound2D(this, ItemGet);
				}

				PlayerCharacter->WeaponID = "Molotov";
				PlayerCharacter->bGetWeapon = true;
			}
		}

		else if (OtherName.Contains(TEXT("ITEM_Weapon_Stone")))
		{
			if (!PlayerCharacter->bGetWeapon && PlayerCharacter->WeaponID == "")
			{
				USoundBase* ItemGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/GetItem.GetItem"));
				if (ItemGet)
				{
					UGameplayStatics::PlaySound2D(this, ItemGet);
				}
				PlayerCharacter->WeaponID = "Stone";
				PlayerCharacter->bGetWeapon = true;
			}
		}

		else if (OtherName.Contains(TEXT("ITEM_Weapon_Trap")))
		{
			if (!PlayerCharacter->bGetWeapon && PlayerCharacter->WeaponID == "")
			{
				USoundBase* ItemGet = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/GetItem.GetItem"));
				if (ItemGet)
				{
					UGameplayStatics::PlaySound2D(this, ItemGet);
				}
				PlayerCharacter->WeaponID = "Trap";
				PlayerCharacter->bGetWeapon = true;
			}
		}
	}
}