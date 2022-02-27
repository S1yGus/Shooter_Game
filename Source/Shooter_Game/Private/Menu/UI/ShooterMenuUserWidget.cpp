// Shooter_Game, All rights reserved.

#include "Menu/UI/ShooterMenuUserWidget.h"
#include "UI/ShooterButtonUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

void UShooterMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartNewGameButton)
    {
        StartNewGameButton->OnClickedButton.AddDynamic(this, &UShooterMenuUserWidget::OnStartNewGame);
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

    PlayAnimation(LoadingAnimation);

    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UShooterMenuUserWidget::OnQuit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UShooterMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == LoadingAnimation)
    {
        const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
        if (!GameInstance)
            return;

        UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetNewGameLevelData().LevelName);
    }
}
