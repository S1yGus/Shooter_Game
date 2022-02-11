// Shooter_Game, All rights reserved.

#include "UI/ShooterGameOverUserWidget.h"
#include "ShooterGameModeBase.h"
#include "Components/VerticalBox.h"
#include "EngineUtils.h"
#include "Player/ShooterPlayerState.h"
#include "UI/ShooterPlayerInfoRowUserWidget.h"
#include "ShooterUtils.h"
#include "UI/ShooterButtonUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UShooterGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->OnGameStateChanged.AddUObject(this, &UShooterGameOverUserWidget::OnGameStateChanged);
    }

    if (RestartLevelButton)
    {
        RestartLevelButton->OnClickedButton.AddDynamic(this, &UShooterGameOverUserWidget::OnRestartLevel);
    }
}

void UShooterGameOverUserWidget::OnRestartLevel()
{
    FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
    UGameplayStatics::OpenLevel(this, CurrentLevelName);
}

void UShooterGameOverUserWidget::OnGameStateChanged(EGameState GameState)
{
    if (GameState == EGameState::GameOver)
    {
        UpdatePlayersInfo();
    }
}

void UShooterGameOverUserWidget::UpdatePlayersInfo()
{
    if (!PlayersInfoBox)
        return;

    PlayersInfoBox->ClearChildren();

    TArray<AController*> Controllers;
    for (const auto Controller : TActorRange<AController>(GetWorld()))
    {
        if (!Controller || !Cast<AShooterPlayerState>(Controller->PlayerState))
            continue;

        Controllers.Add(Controller);
    }

    Controllers.Sort([](const AController& Controller1, const AController& Controller2) {
        const auto PlayerState1 = Cast<AShooterPlayerState>(Controller1.PlayerState);    //
        const auto PlayerState2 = Cast<AShooterPlayerState>(Controller2.PlayerState);    //
        return PlayerState1->GetKillsNum() > PlayerState2->GetKillsNum();
    });

    for (const auto Controller : Controllers)
    {
        const auto PlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);

        const auto PlayerInfoWidget = CreateWidget<UShooterPlayerInfoRowUserWidget>(GetWorld(), PlayersInfoRowWidgetClass);
        if (!PlayerInfoWidget)
            continue;

        PlayerInfoWidget->SetNameTextBlock(FText::FromString(PlayerState->GetPlayerName()));
        PlayerInfoWidget->SetKillsTextBlock(ShooterUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerInfoWidget->SetDeathsTextBlock(ShooterUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerInfoWidget->SetTeamTextBlock(ShooterUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerInfoWidget->SetTeamColorImage(PlayerState->GetTeamColor());
        PlayerInfoWidget->SetPlayerHighlighting(Controller->IsPlayerController());

        PlayersInfoBox->AddChild(PlayerInfoWidget);
    }
}
