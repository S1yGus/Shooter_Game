// Shooter_Game, All rights reserved.

#include "UI/ShooterGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "UI/ShooterPlayerHUDWidget.h"

void AShooterGameHUD::BeginPlay()
{
    Super::BeginPlay();

    const auto PlayerHUDWidget = CreateWidget<UShooterPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (!PlayerHUDWidget)
        return;

    PlayerHUDWidget->AddToViewport();
}

void AShooterGameHUD::DrawCrossHair()
{
    FVector2D Center = FVector2D(Canvas->SizeX / 2, Canvas->SizeY / 2);
    int32 HalfLineSize = 10;
    int32 LineThicknes = 3;
    FLinearColor CrossColor = FLinearColor::Yellow;

    DrawLine(Center.X - HalfLineSize, Center.Y, Center.X + HalfLineSize, Center.Y, CrossColor, LineThicknes);
    DrawLine(Center.X, Center.Y - HalfLineSize, Center.X, Center.Y + HalfLineSize, CrossColor, LineThicknes);
}
