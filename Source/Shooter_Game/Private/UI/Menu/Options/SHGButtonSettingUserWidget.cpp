// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGButtonSettingUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "Components/TextBlock.h"
#include "Settings/SHGActionSetting.h"

void USHGButtonSettingUserWidget::Init(USHGActionSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());
    ActionButton->SetText(Setting->GetActionName());

    Update();
}

void USHGButtonSettingUserWidget::Update()
{
    if (!Setting)
        return;

    ActionButton->SetIsEnabled(Setting->IsActionAvailable());
}

void USHGButtonSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(ActionButton);

    ActionButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedButton);
}

void USHGButtonSettingUserWidget::OnClickedButton()
{
    if (!Setting)
        return;

    Setting->DoAction();
    Update();
}
