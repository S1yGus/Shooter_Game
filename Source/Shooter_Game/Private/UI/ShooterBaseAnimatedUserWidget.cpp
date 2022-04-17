// Shooter_Game, All rights reserved.

#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UShooterBaseAnimatedUserWidget::ShowStartupAnimation()
{

    PlayAnimation(StartupAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), StartupSound);
}

void UShooterBaseAnimatedUserWidget::ShowFadeoutAnimation()
{

    PlayAnimation(FadeoutAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(), FadeoutSound);
}
