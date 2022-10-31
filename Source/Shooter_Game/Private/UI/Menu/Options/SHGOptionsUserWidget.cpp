// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGOptionsUserWidget.h"
#include "UI/Menu/Options/SHGComboBoxSettingUserWidget.h"
#include "UI/Menu/Options/SHGSliderSettingUserWidget.h"
#include "UI/Menu/Options/SHGButtonSettingUserWidget.h"
#include "UI/Menu/Options/SHGCheckBoxSettingUserWidget.h"
#include "Settings/SHGIntSetting.h"
#include "Settings/SHGFloatSetting.h"
#include "Settings/SHGActionSetting.h"
#include "Settings/SHGBoolSetting.h"
#include "Components/VerticalBox.h"
#include "Interfaces/SHGSettingWidgetInterface.h"

#define CREATE_AND_ADD_SETTING_WIDGET(Class, WidgetClass, Setting)       \
    Class* SettingWidget = CreateWidget<Class>(GetWorld(), WidgetClass); \
    check(SettingWidget);                                                \
    SettingWidget->Init(Setting);                                        \
    SettingsVerticalBox->AddChild(SettingWidget);

void USHGOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingsVerticalBox);
}

void USHGOptionsUserWidget::Init(const TArray<USHGSetting*>& SettingsArray)
{
    SettingsVerticalBox->ClearChildren();

    for (const auto Setting : SettingsArray)
    {
        if (const auto IntSetting = Cast<USHGIntSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(USHGComboBoxSettingUserWidget, ComboBoxSettingWidgetClass, IntSetting);
        }
        else if (const auto FloatSetting = Cast<USHGFloatSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(USHGSliderSettingUserWidget, SliderSettingWidgetClass, FloatSetting);
        }
        else if (const auto ActionSetting = Cast<USHGActionSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(USHGButtonSettingUserWidget, ButtonSettingWidgetClass, ActionSetting);
        }
        else if (const auto BoolSetting = Cast<USHGBoolSetting>(Setting))
        {
            CREATE_AND_ADD_SETTING_WIDGET(USHGCheckBoxSettingUserWidget, CheckBoxSettingWidgetClass, BoolSetting);
        }
    }
}

void USHGOptionsUserWidget::UpdateOptions()
{
    for (const auto Widget : SettingsVerticalBox->GetAllChildren())
    {
        if (const auto SettingWidget = Cast<ISHGSettingWidgetInterface>(Widget))
        {
            SettingWidget->Update();
        }
    }
}
