// Shooter_Game, All rights reserved.

#include "UI/Menu/ShooterMenuButtonUserWidget.h"
#include "UI/Menu/ShooterButtonUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"

void UShooterMenuButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!MainMenuButton)
        return;

    MainMenuButton->OnClickedButton.AddDynamic(this, &UShooterMenuButtonUserWidget::OnMainMenu);
}

void UShooterMenuButtonUserWidget::OnMainMenu()
{
    const auto GameInstnce = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstnce)
        return;

    UGameplayStatics::OpenLevel(this, GameInstnce->GetMenuLevelName());
}
