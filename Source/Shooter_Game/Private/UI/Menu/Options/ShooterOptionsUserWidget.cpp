// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterOptionsUserWidget.h"
#include "UI/Menu/ShooterButtonUserWidget.h"
#include "ShooterBaseHUD.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/VerticalBox.h"
#include "ShooterGameInstance.h"
#include "UI/Menu/Options/ShooterOptionsButtonUserWidget.h"

void UShooterOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SaveButton)
    {
        SaveButton->OnClickedButton.AddDynamic(this, &UShooterOptionsUserWidget::OnSaveSettings);
    }

    if (BackButton)
    {
        BackButton->OnClickedButton.AddDynamic(this, &UShooterOptionsUserWidget::OnBackToRootMenu);
    }

    FillOptionsButtonsBox();
}

void UShooterOptionsUserWidget::OnSaveSettings()
{
    GEngine->GetGameUserSettings()->SaveSettings();

    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (GameInstance)
    {
        GameInstance->SaveSettingsSave();
    }
}

void UShooterOptionsUserWidget::OnBackToRootMenu()
{
    if (!GetOwningPlayer())
        return;

    const auto HUD = GetOwningPlayer()->GetHUD<AShooterBaseHUD>();
    if (!HUD)
        return;

    HUD->BackToRootMenu();
}

void UShooterOptionsUserWidget::OnSelectedOptionsCategory(const FOptionsButtonData& OptionsButtonData)
{
    for (const auto OptionsCategoryButton : OptionsCategoryButtons)
    {
        const bool IsSelected = OptionsCategoryButton->GetOptionsButtonData().WidgetID == OptionsButtonData.WidgetID;

        OptionsCategoryButton->SetSelected(IsSelected);

        if (IsSelected)
        {
            OptionsWidgetSwitcher->SetActiveWidgetIndex(OptionsButtonData.WidgetID);
        }
    }
}

void UShooterOptionsUserWidget::FillOptionsButtonsBox()
{
    if (!OptionsButtonsBox)
        return;

    OptionsButtonsBox->ClearChildren();

    checkf(OptionsCategoryButtonsData.Num() != 0, TEXT("OptionsCategoryButtonsData must contains items!"));

    for (const auto OptionsCategoryButtonData : OptionsCategoryButtonsData)
    {
        const auto OptionsCategoryButton = CreateWidget<UShooterOptionsButtonUserWidget>(GetWorld(), OptionsCategoryButtonClass);
        if (!OptionsCategoryButton)
            continue;

        OptionsCategoryButton->SetOptionsButtonData(OptionsCategoryButtonData);
        OptionsCategoryButton->OnClickedOptionsButton.AddUObject(this, &UShooterOptionsUserWidget::OnSelectedOptionsCategory);

        OptionsCategoryButtons.Add(OptionsCategoryButton);
        OptionsButtonsBox->AddChild(OptionsCategoryButton);
    }

    OnSelectedOptionsCategory(OptionsCategoryButtons[0]->GetOptionsButtonData());
}
