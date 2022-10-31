// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGOptionsUserWidget.generated.h"

class UVerticalBox;
class USHGComboBoxSettingUserWidget;
class USHGSliderSettingUserWidget;
class USHGButtonSettingUserWidget;
class USHGCheckBoxSettingUserWidget;
class USHGSetting;

UCLASS()
class SHOOTER_GAME_API USHGOptionsUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(const TArray<USHGSetting*>& SettingsArray);
    void UpdateOptions();

protected:
    UPROPERTY(Meta = (BindWidget))
    UVerticalBox* SettingsVerticalBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGComboBoxSettingUserWidget> ComboBoxSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGSliderSettingUserWidget> SliderSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGButtonSettingUserWidget> ButtonSettingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGCheckBoxSettingUserWidget> CheckBoxSettingWidgetClass;

    virtual void NativeOnInitialized() override;
};
