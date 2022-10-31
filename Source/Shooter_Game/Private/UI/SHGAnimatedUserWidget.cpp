// Shooter_Game, All rights reserved.

#include "UI/SHGAnimatedUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void USHGAnimatedUserWidget::ShowStartupAnimation()
{
    if (IsAnimationPlaying(StartupAnimation))
        return;

    PlayAnimation(StartupAnimation);
    UGameplayStatics::PlaySound2D(this, StartupSound);
}

void USHGAnimatedUserWidget::ShowFadeoutAnimation()
{
    if (IsAnimationPlaying(FadeoutAnimation))
        return;

    PlayAnimation(FadeoutAnimation);
    UGameplayStatics::PlaySound2D(this, FadeoutSound);
}

void USHGAnimatedUserWidget::ChangeGameState(EGameState NewGameState)
{
    GameStateToSet = NewGameState;
    ShowFadeoutAnimation();
}
