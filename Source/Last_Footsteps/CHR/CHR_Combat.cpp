

#include "CHR_Combat.h"
#include "CHR_Movement.h"
#include "CHR_Interaction.h"
#include "ENM_Movement.h"
#include "ENM_BossManager.h"
#include "ENM_Combat_Damaged.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h" 
#include "Kismet/GameplayStatics.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ACHR_Combat::ACHR_Combat()
{
	PrimaryActorTick.bCanEverTick = true;

	mStaticMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Obj"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	
	//충돌감지용 콜라이더 크기설정
	CollisionSphere->InitSphereRadius(100.0f);
	RootComponent = CollisionSphere;

	// Collision에 붙이기
	mStaticMesh->SetupAttachment(RootComponent);

	WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponEffect"));
	//Attach the Niagara component to the root component
	WeaponEffect->SetupAttachment(RootComponent);

	// Optionally set the Niagara system (can also be set in Blueprint)
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/Resources/Effects/WeaponEffect.WeaponEffect"));
	if (NiagaraSystemAsset.Succeeded())
	{
		WeaponEffect->SetAsset(NiagaraSystemAsset.Object);
		WeaponEffect->Activate();
	}
}

// Called when the game starts or when spawned
void ACHR_Combat::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACHR_Combat::Get_OnOverlapBegin);
}


// Called every frame
void ACHR_Combat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACHR_Combat::Get_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACHR_Movement * PlayerCharacter = Cast<ACHR_Movement>(OtherActor))
	{
		if (CurrentWeaponID != "" && bActive && bIsAttack)
		{
			//무기 들고있을시 무시
			return;
		}
		
		if (!bActive && PlayerCharacter->CurrentWeaponID == "" && !bIsAttack)
		{
			bActive = true;
			

			//오른손에 착용
			SetActorLocation(FVector::ZeroVector);
			SetActorRotation(FRotator::ZeroRotator);
			AttachToComponent(PlayerCharacter->FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
			
			if (WeaponEffect)
			{
				WeaponEffect->DestroyComponent();
			}


			// 스케일 조정
			SetActorRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

			//무기상태최신화
			FString ClassName = GetClass()->GetName();

			if (ClassName.Contains("Weapon_Dynamite"))
			{
				HitPower = 70.0f;
				EnemySlowValue = 200.f;
				SetActorRelativeLocation(FVector(-48, 18, 95));
				SetActorRelativeRotation(FRotator(45, -45, 40));
				CurrentWeaponID = "Dynamite";
			}
			else if (ClassName.Contains("Weapon_Grenade"))
			{
				HitPower = 50.0f;
				EnemySlowValue = 100.f;
				SetActorRelativeLocation(FVector(-48, 18, 95));
				SetActorRelativeRotation(FRotator(-71, -27, -51));
				CurrentWeaponID = "Grenade";
			}
			else if (ClassName.Contains("Weapon_Molotov"))
			{
				SetActorRelativeLocation(FVector(-48, 18, 95));
				SetActorRelativeRotation(FRotator(45, -23, -33));
				HitPower = 30.0f;
				EnemySlowValue = 40.f;
				CurrentWeaponID = "Molotov";
			}
			else if (ClassName.Contains("Weapon_Stone"))
			{
				HitPower = 10.0f;
				EnemySlowValue = 10.f;
				SetActorRelativeLocation(FVector(-48, 18, 95));
				SetActorRelativeRotation(FRotator(45, -45, 40));
				CurrentWeaponID = "Stone";
			}
			else if (ClassName.Contains("Weapon_Trap"))
			{
				HitPower = 20.0f;
				EnemySlowValue = 500.f;
				SetActorRelativeLocation(FVector(-48, 18, 95));
				SetActorRelativeRotation(FRotator(45, -45, 40));
				CurrentWeaponID = "Trap";
			}

			PlayerCharacter->CurrentWeaponID = CurrentWeaponID;
			PlayerCharacter->CurrentWeapon = this;
		}
	}

	else if (AENM_BossManager* Boss = Cast<AENM_BossManager>(OtherActor))
	{
		if (bIsAttack)
		{
			AENM_Movement* BossCharacter = Cast<AENM_Movement>(OtherActor);
			UENM_Combat_Damaged* DamagedComponent = Boss->FindComponentByClass<UENM_Combat_Damaged>();
			if (DamagedComponent && BossCharacter)
			{
				BossCharacter->CallDamagedSound();
				DamagedComponent->ApplyDamage(HitPower, EnemySlowValue);
				bIsAttack = false;

				if (CurrentWeaponID == "Dynamite" || CurrentWeaponID == "Grenade")
				{
					USoundBase* Explosion = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/Explosion.Explosion"));
					if (Explosion)
					{
						UGameplayStatics::PlaySound2D(this, Explosion);
					}
				}

				else if (CurrentWeaponID == "Molotov")
				{
					USoundBase* Fire = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/Fire.Fire"));
					if (Fire)
					{
						UGameplayStatics::PlaySound2D(this, Fire);
					}
				}

				else if (CurrentWeaponID == "Stone")
				{
					USoundBase* Stone = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/Stone.Stone"));
					if (Stone)
					{
						UGameplayStatics::PlaySound2D(this, Stone);
					}
				}

				else if (CurrentWeaponID == "Trap")
				{
					USoundBase* Trap = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/AttackItem/Trap.Trap"));
					if (Trap)
					{
						UGameplayStatics::PlaySound2D(this, Trap);
					}
				}
			}
			//무기삭제
			CurrentWeaponID = "";
			Destroy();
		}
	}
}

void ACHR_Combat::RopeVisible(bool Visible)
{
	if (Visible)
	{
		mStaticMesh->SetVisibility(true);
	}
	else
	{
		mStaticMesh->SetVisibility(false);
	}
}
