// Shooter_Game, All rights reserved.

#include "Menu/UI/ShooterMenuHUD.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"

void AShooterMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
            MenuWidget->ShowStartupAnimation();
        }
    }
}
