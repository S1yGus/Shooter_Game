// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/SHGSettingWidgetInterface.h"
#include "SHGButtonSettingUserWidget.generated.h"

class USHGButtonUserWidget;
class USHGActionSetting;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGButtonSettingUserWidget : public UUserWidget, public ISHGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(USHGActionSetting* NewSetting);
    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* ActionButton;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    USHGActionSetting* Setting;

    void OnClickedButton();
};
