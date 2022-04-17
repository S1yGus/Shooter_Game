// Shooter_Game, All rights reserved.

#include "UI/Menu/Main/ShooterMenuUserWidget.h"
#include "UI/Menu/ShooterButtonUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "ShooterGameModeBase.h"

void UShooterMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartNewGameButton)
    {
        StartNewGameButton->OnClickedButton.AddDynamic(this, &UShooterMenuUserWidget::OnStartNewGame);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClickedButton.AddDynamic(this, &UShooterMenuUserWidget::OnOptions);
    }

    if (QuitButton)
    {
        QuitButton->OnClickedButton.AddDynamic(this, &UShooterMenuUserWidget::OnQuit);
    }
}

void UShooterMenuUserWidget::OnStartNewGame()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    if (GameInstance->GetNewGameLevelData().LevelName.IsNone())
        return;

    ShowFadeoutAnimation();
}

void UShooterMenuUserWidget::OnOptions()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>();
    if (!GameMode)
        return;

    GameMode->InOptions();
}

void UShooterMenuUserWidget::OnQuit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UShooterMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
        if (!GameInstance)
            return;

        UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetNewGameLevelData().LevelName);
    }
}
