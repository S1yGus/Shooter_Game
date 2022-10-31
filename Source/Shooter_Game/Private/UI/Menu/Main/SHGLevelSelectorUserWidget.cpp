// Shooter_Game, All rights reserved.

#include "UI/Menu/Main/SHGLevelSelectorUserWidget.h"
#include "UI/Menu/Main/SHGLevelItemUserWidget.h"
#include "Components/HorizontalBox.h"
#include "SHGGameInstance.h"

void USHGLevelSelectorUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(LevelItemsBox);

    FillLevelItemsBox();
}

void USHGLevelSelectorUserWidget::OnSelectedLevel(const FLevelData& Data)
{
    const auto GameInstance = GetWorld()->GetGameInstance<USHGGameInstance>();
    if (!GameInstance)
        return;

    GameInstance->SetNewGameLevelData(Data);

    for (const auto& LevelItemWidget : LevelItemWidgets)
    {
        LevelItemWidget->SetSelected(LevelItemWidget->GetLevelData().LevelName == Data.LevelName);
    }
}

void USHGLevelSelectorUserWidget::FillLevelItemsBox()
{
    LevelItemsBox->ClearChildren();

    const auto GameInstance = GetWorld()->GetGameInstance<USHGGameInstance>();
    if (!GameInstance)
        return;

    checkf(GameInstance->GetGameLevelsData().Num() != 0, TEXT("GetGameLevelsData must contains items!"));

    for (const auto& LevelData : GameInstance->GetGameLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<USHGLevelItemUserWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget)
            continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnSelectedLevel.AddUObject(this, &ThisClass::OnSelectedLevel);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    OnSelectedLevel(GameInstance->GetGameLevelsData()[0]);
}
