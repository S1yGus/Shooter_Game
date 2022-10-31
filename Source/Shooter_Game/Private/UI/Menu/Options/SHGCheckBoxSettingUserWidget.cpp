// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGCheckBoxSettingUserWidget.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Settings/SHGBoolSetting.h"

void USHGCheckBoxSettingUserWidget::Init(USHGBoolSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());

    Update();
}

void USHGCheckBoxSettingUserWidget::Update()
{
    if (!Setting)
        return;

    const auto CurrentValue = Setting->GetCurrentValue();
    if (CurrentValue != INDEX_NONE)
    {
        SettingCheckBox->SetIsChecked(static_cast<bool>(CurrentValue));
    }

    SettingCheckBox->SetIsEnabled(CurrentValue != INDEX_NONE);
}

void USHGCheckBoxSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(SettingCheckBox);

    SettingCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCheckStateChanged);
}

void USHGCheckBoxSettingUserWidget::OnCheckStateChanged(bool bIsChecked)
{
    if (!Setting)
        return;

    Setting->SetValue(bIsChecked);
}
