// Shooter_Game, All rights reserved.

#include "UI/Menu/Main/ShooterLevelSelectorUserWidget.h"
#include "UI/Menu/Main/ShooterLevelItemUserWidget.h"
#include "ShooterGameInstance.h"
#include "Components/HorizontalBox.h"

void UShooterLevelSelectorUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    FillLevelItemsBox();
}

void UShooterLevelSelectorUserWidget::OnSelectedLevel(const FLevelData& Data)
{
    const auto GameInstance = Cast<UShooterGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
        return;

    GameInstance->SetNewGameLevelData(Data);

    for (const auto LevelItemWidget : LevelItemWidgets)
    {
        LevelItemWidget->SetSelected(LevelItemWidget->GetLevelData().LevelName == Data.LevelName);
    }
}

void UShooterLevelSelectorUserWidget::FillLevelItemsBox()
{
    if (!LevelItemsBox)
        return;

    LevelItemsBox->ClearChildren();

    const auto GameInstance = Cast<UShooterGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
        return;

    checkf(GameInstance->GetGameLevelsData().Num() != 0, TEXT("GetGameLevelsData must contains items!"));

    for (const auto LevelData : GameInstance->GetGameLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<UShooterLevelItemUserWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget)
            continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnSelectedLevel.AddUObject(this, &UShooterLevelSelectorUserWidget::OnSelectedLevel);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if (!GameInstance->GetNewGameLevelData().LevelName.IsNone())
    {
        OnSelectedLevel(GameInstance->GetNewGameLevelData());
    }
}
