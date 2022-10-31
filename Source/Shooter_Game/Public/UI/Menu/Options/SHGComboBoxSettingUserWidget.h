// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/SHGSettingWidgetInterface.h"
#include "SHGComboBoxSettingUserWidget.generated.h"

class UComboBoxString;
class USHGIntSetting;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGComboBoxSettingUserWidget : public UUserWidget, public ISHGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(USHGIntSetting* NewSetting);
    void Update();

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* SettingComboBox;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    USHGIntSetting* Setting;

    UFUNCTION()
    void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
