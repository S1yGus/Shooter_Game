// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGComboBoxSettingUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Settings/SHGIntSetting.h"

void USHGComboBoxSettingUserWidget::Init(USHGIntSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());

    Update();
}

void USHGComboBoxSettingUserWidget::Update()
{
    if (!Setting)
        return;

    SettingComboBox->ClearOptions();
    for (const auto& Option : Setting->GetOptions())
    {
        SettingComboBox->AddOption(Option.ToString());
    }

    const auto CurrentValue = Setting->GetCurrentValue();
    if (CurrentValue != INDEX_NONE)
    {
        SettingComboBox->SetSelectedIndex(CurrentValue);
    }

    SettingComboBox->SetIsEnabled(CurrentValue != INDEX_NONE);
}

void USHGComboBoxSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(SettingComboBox);

    SettingComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChanged);
}

void USHGComboBoxSettingUserWidget::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !Setting)
        return;

    Setting->SetValue(SettingComboBox->FindOptionIndex(SelectedItem));
}
