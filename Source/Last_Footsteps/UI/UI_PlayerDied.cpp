// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_PlayerDied.h"
#include "CHR_Movement.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "GI_GameInstance.h"
#include "kismet/GameplayStatics.h"


void UUI_PlayerDied::NativeConstruct()
{
    // 게임 인스턴스 캐스트
    // 최고 점수 및 생존 시간 등 실시간 반영 위함
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
    GI_GameInstance = Cast<UGI_GameInstance>(GameInstance);

    AGameModeBase* BaseMode = UGameplayStatics::GetGameMode(this);
    GM = Cast<ALast_Footsteps_GamemodeBase>(UGameplayStatics::GetGameMode(BaseMode));


    PlayStartAnimation();
    PlayLoseSound();

    // 재시작
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UUI_PlayerDied::OnRestartClicked);
    }

    // 메인메뉴 호출
    if (MenuButton)
    {
        MenuButton->OnClicked.AddDynamic(this, &UUI_PlayerDied::OnMenuClicked);
    }


    ResultText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ResultText")));
    ResultText_1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("ResultText_1")));
    NewRecord = Cast<UTextBlock>(GetWidgetFromName(TEXT("NewRecord")));
    NewRecordRich = Cast<URichTextBlock>(GetWidgetFromName(TEXT("NewRecordRich")));

    if (ResultText && GM && GI_GameInstance)
    {
        Score = GM->GM_Score;
        bool bCheckTRecord = false;
        bool bCheckSRecord = false;

        if (GI_GameInstance->GI_BestRecord <= GM->Time)
        {
            NewRecord->SetVisibility(ESlateVisibility::Visible);
            GI_GameInstance->GI_BestRecord = GM->Time;
            GI_GameInstance->SaveData();
            bCheckTRecord = true;
        }

        if (GI_GameInstance->GI_BestScore <= Score)
        {
            NewRecord->SetVisibility(ESlateVisibility::Visible);
            GI_GameInstance->GI_BestScore = Score;
            GI_GameInstance->SaveData();
            bCheckSRecord = true;
        }

        Minutes = (int)(GM->Time) / 60;
        Seconds = GM->Time - (60 * Minutes);

        BestMinutes = (int)GI_GameInstance->GI_BestRecord / 60;
        BestSeconds = GI_GameInstance->GI_BestRecord - (60 * BestMinutes);

        BestScore = GI_GameInstance->GI_BestScore;


        FText ResultCurrentRecord = FText::Format(FText::FromString(TEXT("Current Record\n{0}분 {1}초")), FText::AsNumber(Minutes), FText::AsNumber(Seconds, &FNumberFormattingOptions().SetMinimumFractionalDigits(2).SetMaximumFractionalDigits(2)));
        FText ResultCurrentScore = FText::Format(FText::FromString(TEXT(" / {0}점")), FText::AsNumber(Score));
        FText ResultBestRecord;
        FText ResultBestScore;
        
        if (bCheckTRecord)
        {
            ResultBestRecord = FText::Format(FText::FromString(TEXT("Best Record\n<NewRecord>{0}분 {1}초</>")), FText::AsNumber(BestMinutes), FText::AsNumber(BestSeconds, &FNumberFormattingOptions().SetMinimumFractionalDigits(2).SetMaximumFractionalDigits(2)));
        }
        else
        {
            ResultBestRecord = FText::Format(FText::FromString(TEXT("Best Record\n{0}분 {1}초")), FText::AsNumber(BestMinutes), FText::AsNumber(BestSeconds, &FNumberFormattingOptions().SetMinimumFractionalDigits(2).SetMaximumFractionalDigits(2)));
        }

        if (bCheckSRecord)
        {
            ResultBestScore = FText::Format(FText::FromString(TEXT(" / <NewRecord>{0}점</>")), FText::AsNumber(BestScore));
        }
        else
        {
            ResultBestScore = FText::Format(FText::FromString(TEXT(" / {0}점")), FText::AsNumber(BestScore));
        }
        


        FText ResultFText = FText::Format(FText::FromString(TEXT("{0}{1}")), ResultCurrentRecord, ResultCurrentScore);
        FText ResultFText_1 = FText::Format(FText::FromString(TEXT("{0}{1}")), ResultBestRecord, ResultBestScore);
        ResultText->SetText(FText::AsCultureInvariant(ResultFText));
        
        // Deprecated
        // ResultText_1->SetText(FText::AsCultureInvariant(ResultFText_1));
        NewRecordRich->SetText(FText::AsCultureInvariant(ResultFText_1));
    }
    
}

void UUI_PlayerDied::OnRestartClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainMap"));

    if (GM)
    {
        GM->ChangeMenuWidget(GM->UI_PlayerHUDWidget);
    }
}

void UUI_PlayerDied::OnMenuClicked()
{
    UGameplayStatics::OpenLevel(this, FName("TitleMenu"));

    if (GM)
    {
        GM->ChangeMenuWidget(GM->UI_TitleWidget);
    }
}

void UUI_PlayerDied::PlayLoseSound()
{
    USoundBase* SoundFinder = LoadObject<USoundBase>(nullptr, TEXT("/Game/Resources/Sounds/Use/Lose/_Lose_vine-boom-sound-effect_chosic_com_._Lose_vine-boom-sound-effect_chosic_com_"));
    if (SoundFinder)
    {
        UGameplayStatics::PlaySound2D(this, SoundFinder);
    }
}

void UUI_PlayerDied::PlayStartAnimation()
{
    if (DiedAnimation)
    {
        PlayAnimation(DiedAnimation);
    }
}