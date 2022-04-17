// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterOptionsUserWidget.generated.h"

class UShooterButtonUserWidget;
class UShooterOptionsButtonUserWidget;
class UWidgetSwitcher;
class UVerticalBox;

UCLASS()
class SHOOTER_GAME_API UShooterOptionsUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TArray<FOptionsButtonData> OptionsCategoryButtonsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterOptionsButtonUserWidget> OptionsCategoryButtonClass;

    UPROPERTY(Meta = (BindWidget))
    UVerticalBox* OptionsButtonsBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UWidgetSwitcher* OptionsWidgetSwitcher = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* SaveButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* BackButton = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UShooterOptionsButtonUserWidget*> OptionsCategoryButtons;

    UFUNCTION()
    void OnSaveSettings();

    UFUNCTION()
    void OnBackToRootMenu();

    void OnSelectedOptionsCategory(const FOptionsButtonData& OptionsButtonData);
    void FillOptionsButtonsBox();
};
