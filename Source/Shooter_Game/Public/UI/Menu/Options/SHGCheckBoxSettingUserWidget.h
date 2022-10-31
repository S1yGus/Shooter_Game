// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/SHGSettingWidgetInterface.h"
#include "SHGCheckBoxSettingUserWidget.generated.h"

class UCheckBox;
class USHGBoolSetting;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGCheckBoxSettingUserWidget : public UUserWidget, public ISHGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(USHGBoolSetting* NewSetting);
    void Update();

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    UCheckBox* SettingCheckBox;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    USHGBoolSetting* Setting;

    UFUNCTION()
    void OnCheckStateChanged(bool bIsChecked);
};
