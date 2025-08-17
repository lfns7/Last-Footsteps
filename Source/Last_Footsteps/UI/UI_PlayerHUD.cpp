// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_PlayerHUD.h"
#include "CHR_Movement.h"
#include "ENM_BossManager.h"
#include "ENM_MoveMent.h"
#include "GI_GameInstance.h"
#include "Components/Button.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include <vector>
#include <Math/UnrealMathUtility.h>


void UUI_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentScoreText")));

	CurrentSecondTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentSecondTimeText")));
	CurrentMinuteTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentMinuteTimeText")));

	HP_Foreground = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Foreground")));
	PlayerHPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerHPText")));

	HitEffectImage = Cast<UImage>(GetWidgetFromName(TEXT("HitEffectImage")));
	SuperShoesImage = Cast<UImage>(GetWidgetFromName(TEXT("SuperShoesImage")));


	ShieldCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ShieldCanvas")));

	PauseButton = Cast<UButton>(GetWidgetFromName(TEXT("PauseButton")));
	if (PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UUI_PlayerHUD::UI_PauseButtonClicked);
	}

	GM = Cast<ALast_Footsteps_GamemodeBase>(UGameplayStatics::GetGameMode(this));


	AActor* BS = UGameplayStatics::GetActorOfClass(GetWorld(), AENM_Movement::StaticClass());
	BOSS = Cast<AENM_Movement>(BS);

	ACharacter* PL = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerCharacter = Cast<ACHR_Movement>(PL);


	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	GI_GameInstance = Cast<UGI_GameInstance>(GameInstance);

	if (GI_GameInstance->GI_bTutorial || GI_GameInstance->GI_bTutorialCheck) {
		PauseButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_PlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!GM && !BOSS && !PlayerCharacter)
	{
		return;
	}


	if (PlayerCharacter && BOSS)
	{
		PlayerToBossDist = FVector::Dist(FVector(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, 0), FVector(BOSS->GetActorLocation().X, BOSS->GetActorLocation().Y, 0));
	}

	if (CurrentHP != GM->HP)
	{
		CurrentHP = FMath::Lerp(CurrentHP, GM->HP, 0.07f);
		UI_FixHPBar(CurrentHP);
		

		// 플레이어의 무적 시간이 0초 이상인 경우라면 피격 이미지 표현
		if (PlayerCharacter->invincibility > 0.f && !PlayerCharacter->bSuperShoes)
		{
			// 무적 시간을 값으로 사용하여 최대 1초간 이펙트 표현
			HitEffectImage->SetRenderOpacity(FMath::Clamp(PlayerCharacter->invincibility, 0.0f, 1.0f));
		}
		else if (PlayerCharacter->invincibility > 0.f && PlayerCharacter->bSuperShoes)
		{
			// 무적 시간을 값으로 사용하여 최대 2.0초간 이펙트 표현
			SuperShoesImage->SetRenderOpacity(FMath::Clamp(PlayerCharacter->invincibility, 0.0f, 2.0f));
		}
		else
		{
			SuperShoesImage->SetRenderOpacity(0.0f);
			HitEffectImage->SetRenderOpacity(0.0f);
		}
	}

	// 쉴드 아이템 보유여부
	if (PlayerCharacter->bShield)
	{
		ShieldCanvas->SetRenderOpacity(1.0f);
	}
	else
	{
		ShieldCanvas->SetRenderOpacity(0.0f);
	}

	// 파워업 아이템 보유여부
	if (PlayerCharacter->bPowerUp)
	{
		PowerUpCanvas->SetRenderOpacity(1.0f);
	}
	else
	{
		PowerUpCanvas->SetRenderOpacity(0.0f);
	}

	// 로프 가능여부
	if (PlayerCharacter->bCanRopeRange && !PlayerCharacter->bIsSliding)
	{
		RopeCanvas->SetRenderOpacity(1.0f);
	}
	else
	{
		RopeCanvas->SetRenderOpacity(0.0f);
	}

	if (TempCurrentScore != GM->GM_Score)
	{
		TempCurrentScore = FMath::Lerp(TempCurrentScore, GM->GM_Score, 1.0f);
		
		UI_FixScore(TempCurrentScore);
	}

	// 보스 체력바 연출
	if (BOSS->BossHPTemp != BossCurrentHP)
	{
		BossCurrentHP = FMath::Lerp(BossCurrentHP, BOSS->BossHPTemp, 0.07f);
		UI_FixBossHPBar(BossCurrentHP);
	}

	if (WarningTime <= 1.5f && TargetImages.Num() != 0 && FMath::IsNearlyEqual(BOSS->GetActorRotation().Yaw, PlayerCharacter->GetActorRotation().Yaw, 1.f))
	{
		if (HUDCurrentLane == PlayerCharacter->CurrentLane)
		{
			if (HUDCurrentLane == 0)
			{
				for (int i = 0; i < TargetImages.Num(); i++)
				{
					// 켜질 수 없는 위치 비활성화
					Warning_Throw_Left_Bottom->SetVisibility(ESlateVisibility::Hidden);
					Warning_Throw_Left_Middle->SetVisibility(ESlateVisibility::Hidden);

					if (TargetImages[i] == Warning_Throw_Left_Bottom)
					{
						// 1라인 기준 왼쪽 -> 가운데
						Warning_Throw_Center_Bottom->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Center_Bottom->SetOpacity(WarningTime);
					}
					
					else if (TargetImages[i] == Warning_Throw_Left_Middle)
					{
						Warning_Throw_Center_Middle->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Center_Middle->SetOpacity(WarningTime);
					}

					else if (TargetImages[i] == Warning_Throw_Center_Bottom)
					{
						// 1라인 기준 가운데 -> 오른쪽
						Warning_Throw_Right_Bottom->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Right_Bottom->SetOpacity(WarningTime);
					}

					else if (TargetImages[i] == Warning_Throw_Center_Middle)
					{
						Warning_Throw_Right_Middle->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Right_Middle->SetOpacity(WarningTime);
					}
				}
			}

			// 가운데 라인은 레인에 따라 표현
			else if (HUDCurrentLane == 1)
			{
				for (int i = 0; i < TargetImages.Num(); i++)
				{
					TargetImages[i]->SetVisibility(ESlateVisibility::Visible);
					TargetImages[i]->SetOpacity(WarningTime);
				}
			}

			else
			{
				for (int i = 0; i < TargetImages.Num(); i++)
				{
					// 켜질 수 없는 위치 비활성화
					Warning_Throw_Right_Bottom->SetVisibility(ESlateVisibility::Hidden);
					Warning_Throw_Right_Middle->SetVisibility(ESlateVisibility::Hidden);

					if (TargetImages[i] == Warning_Throw_Right_Bottom)
					{
						// 3라인 기준 오른쪽 -> 가운데
						Warning_Throw_Center_Bottom->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Center_Bottom->SetOpacity(WarningTime);
					}

					else if (TargetImages[i] == Warning_Throw_Right_Middle)
					{
						Warning_Throw_Center_Middle->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Center_Middle->SetOpacity(WarningTime);
					}

					else if (TargetImages[i] == Warning_Throw_Center_Bottom)
					{
						// 3라인 기준 가운데 -> 왼쪽
						Warning_Throw_Left_Bottom->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Left_Bottom->SetOpacity(WarningTime);
					}

					else if (TargetImages[i] == Warning_Throw_Center_Middle)
					{
						Warning_Throw_Left_Middle->SetVisibility(ESlateVisibility::Visible);
						Warning_Throw_Left_Middle->SetOpacity(WarningTime);
					}
				}
			}
		}
		else
		{
			// 라인이 다른 경우 라인 정보 초기화
			HUDCurrentLane = PlayerCharacter->CurrentLane;
			Warning_Throw_Left_Bottom->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Left_Middle->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Center_Bottom->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Center_Middle->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Right_Bottom->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Right_Middle->SetVisibility(ESlateVisibility::Hidden);
		}
		
		WarningTime += 1.0 * InDeltaTime; 
	}
	else
	{
		// 경고 시간이 끝나면 배열 초기화
		if (TargetImages.Num() > 0)
		{
			Warning_Throw_Left_Bottom->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Left_Middle->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Center_Bottom->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Center_Middle->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Right_Bottom->SetVisibility(ESlateVisibility::Hidden);
			Warning_Throw_Right_Middle->SetVisibility(ESlateVisibility::Hidden);
			TargetImages.Empty();
		}
	}


	if (CurrentSecondTimeText && CurrentMinuteTimeText && GM)
	{
		Minutes = (int)(GM->Time) / 60;
		FString SecondsTime;

		// 초 단위 시간
		if(Minutes == 0)
			SecondsTime = FString::Printf(TEXT("%.2f"), GM->Time);
		else
			SecondsTime = FString::Printf(TEXT("%.2f"), GM->Time - (60 * Minutes));

		// 분단위 시간
		FString MinutesTime = FString::Printf(TEXT("%d"), Minutes);


		// 초
		if (GM->Time < 10.f || ((int)GM->Time%60) < 10)
		{
			CurrentSecondTimeText->SetText(FText::FromString("0" + SecondsTime));
		}

		else
		{
			CurrentSecondTimeText->SetText(FText::FromString(SecondsTime));
		}

		
		// 분
		if (Minutes <= 9)
		{
			CurrentMinuteTimeText->SetText(FText::FromString("0"+MinutesTime+" : "));
		}
		else
		{
			CurrentMinuteTimeText->SetText(FText::FromString(MinutesTime + " : "));
		}
	}
}

void UUI_PlayerHUD::UI_FixScore(float CurrentScore)
{
	CurrentScoreText->SetText(FText::AsNumber(CurrentScore));
}

void UUI_PlayerHUD::UI_FixHPBar(float _CurrentHP)
{
	FString HpText = FString::Printf(TEXT("%.0f"), _CurrentHP);
	PlayerHPText->SetText(FText::FromString(HpText));
	HP_Foreground->SetPercent(_CurrentHP /MAX_HP);
}

void UUI_PlayerHUD::UI_FixBossHPBar(float _BossCurrentHP)
{
	FString BossHpText;
	if (_BossCurrentHP == 1000)
	{
		BossHpText = FString::Printf(TEXT("1,000"));
	}

	else
	{
		BossHpText = FString::Printf(TEXT("%.0f"), _BossCurrentHP);
	}
	
	BossHPText->SetText(FText::FromString(BossHpText));
	
	BossHP_Foreground->SetPercent(_BossCurrentHP / BOSS_MAX_HP);

	FString DistanceText = FString::Printf(TEXT("%.2f"), PlayerToBossDist / 100.f);
	BossDistanceText->SetText(FText::FromString(DistanceText + " M"));
}

void UUI_PlayerHUD::UI_PauseButtonClicked()
{
	if ((GM->HP > 0) && (BOSS->BossHP > 0))
	{
		PlayerCharacter->CallMenu();
	}
}

void UUI_PlayerHUD::ShowWarning(FName WarningName)
{
	UImage* TargetImage = nullptr;
	WarningTime = 0.0f;

	// 이름으로 이미지 매칭
	if (WarningName == "Melee_Bottom")
	{
		WarningTime = 1.0f;
		TargetImage = Warning_Melee_Bottom;
	}

	else if (WarningName == "Melee_Middle")
	{
		WarningTime = 1.0f;
		TargetImage = Warning_Melee_Middle;
	}
	
	else if (WarningName == "Throw_Left_Bottom") // 1
	{
		TargetImage = Warning_Throw_Left_Bottom;
		TargetImages.Add(TargetImage);
	}
		
	else if (WarningName == "Throw_Left_Middle") // 2
	{
		TargetImage = Warning_Throw_Left_Middle;
		TargetImages.Add(TargetImage);
	}
		
	else if (WarningName == "Throw_Center_Bottom") // 3
	{
		TargetImage = Warning_Throw_Center_Bottom;
		TargetImages.Add(TargetImage);
	}
		
	else if (WarningName == "Throw_Center_Middle") // 4
	{
		TargetImage = Warning_Throw_Center_Middle;
		TargetImages.Add(TargetImage);
	}
		
	else if (WarningName == "Throw_Right_Bottom") // 5
	{
		TargetImage = Warning_Throw_Right_Bottom;
		TargetImages.Add(TargetImage);
	}
		
	else if (WarningName == "Throw_Right_Middle") // 6
	{
		TargetImage = Warning_Throw_Right_Middle;
		TargetImages.Add(TargetImage);
	}


	// 이미지 유효성 확인
	if (!TargetImage)
		return;


	if (WarningName == "Melee_Bottom" || WarningName == "Melee_Middle")
	{
		TargetImage->SetVisibility(ESlateVisibility::Visible);

		//// Animation 플레이
		//PlayAnimation(Warning_Throw_Right_Middle_FadeAnim, 0.0f, 1);
		//PlayAnimation(Warning_Throw_Right_Bottom_FadeAnim, 0.0f, 1);
		//PlayAnimation(Warning_Throw_Left_Middle_FadeAnim, 0.0f, 1);
		//PlayAnimation(Warning_Throw_Left_Bottom_FadeAnim, 0.0f, 1);
		//PlayAnimation(Warning_Throw_Center_Middle_FadeAnim, 0.0f, 1);
		//PlayAnimation(Warning_Throw_Center_Bottom_FadeAnim, 0.0f, 1);
		PlayAnimation(Warning_Melee_Middle_FadeAnim, 0.0f, 1);
		PlayAnimation(Warning_Melee_Bottom_FadeAnim, 0.0f, 1);


		// 1초 뒤에 자동으로 숨기기
		FTimerHandle TempHandle;
		TWeakObjectPtr<UImage> WeakImage(TargetImage);

		FTimerDelegate Delegate;
		Delegate.BindLambda([WeakImage]()
			{
				if (WeakImage.IsValid())
				{
					WeakImage->SetVisibility(ESlateVisibility::Hidden);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[AutoHide] TargetImage is no longer valid."));
				}
			});

		GetWorld()->GetTimerManager().SetTimer(TempHandle, Delegate, 1.0f, false);
	}
}