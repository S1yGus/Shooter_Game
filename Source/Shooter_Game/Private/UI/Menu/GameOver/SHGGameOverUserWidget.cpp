// Shooter_Game, All rights reserved.

#include "UI/Menu/GameOver/SHGGameOverUserWidget.h"
#include "UI/Menu/GameOver/SHGPlayerInfoRowUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "Components/VerticalBox.h"
#include "Player/ShooterPlayerState.h"
#include "SHGGameModeBase.h"
#include "SHGGameInstance.h"
#include "EngineUtils.h"
#include "SHGUtils.h"

void USHGGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(RestartButton);
    check(MenuButton);
    check(QuitButton);
    check(PlayersInfoBox);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    RestartButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedRestartButton);
    MenuButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedMenuButton);
    QuitButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedQuitButton);
}

void USHGGameOverUserWidget::OnClickedRestartButton()
{
    ChangeGameState(EGameState::Game);
}

void USHGGameOverUserWidget::OnClickedMenuButton()
{
    ChangeGameState(EGameState::MainMenu);
}

void USHGGameOverUserWidget::OnClickedQuitButton()
{
    ChangeGameState(EGameState::Quit);
}

void USHGGameOverUserWidget::OnGameStateChanged(EGameState GameState)
{
    if (GameState == EGameState::GameOver)
    {
        UpdatePlayersInfo();
    }
}

void USHGGameOverUserWidget::UpdatePlayersInfo()
{
    PlayersInfoBox->ClearChildren();

    TArray<AController*> Controllers;
    for (auto* Controller : TActorRange<AController>(GetWorld()))
    {
        if (!Controller || !Cast<AShooterPlayerState>(Controller->PlayerState))
            continue;

        Controllers.Add(Controller);
    }

    Controllers.Sort(
        [](const AController& Controller1, const AController& Controller2)
        {
            const auto PlayerState1 = Cast<AShooterPlayerState>(Controller1.PlayerState);
            const auto PlayerState2 = Cast<AShooterPlayerState>(Controller2.PlayerState);
            return PlayerState1->GetKillsNum() > PlayerState2->GetKillsNum();
        });

    for (auto* Controller : Controllers)
    {
        const auto PlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);

        const auto PlayerInfoWidget = CreateWidget<USHGPlayerInfoRowUserWidget>(GetWorld(), PlayersInfoRowWidgetClass);
        if (!PlayerInfoWidget)
            continue;

        PlayerInfoWidget->SetName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerInfoWidget->SetKills(SHGUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerInfoWidget->SetDeaths(SHGUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerInfoWidget->SetTeam(SHGUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerInfoWidget->SetTeamColor(PlayerState->GetTeamColor());
        PlayerInfoWidget->SetPlayerHighlighting(Controller->IsPlayerController());

        PlayersInfoBox->AddChild(PlayerInfoWidget);
    }
}

void USHGGameOverUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameInstance = GetWorld()->GetGameInstance<USHGGameInstance>();
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
    if (!GameInstance || !GameMode)
        return;

    switch (GameStateToSet)
    {
        case EGameState::Game:
            GameInstance->RestartLevel();
            break;
        case EGameState::MainMenu:
            GameInstance->OpenMainMenu();
            break;
        case EGameState::Quit:
            GameInstance->QuitGame(GetOwningPlayer());
            break;
        default:
            GameMode->SetGameState(GameStateToSet);
            break;
    }
}
