// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterControlOptionsUserWidget.generated.h"

class USlider;
class UCheckBox;
class UShooterSettingsSave;

UCLASS()
class SHOOTER_GAME_API UShooterControlOptionsUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UCheckBox* InvertMouseYCheckBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* MouseYSensSlider = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* MouseXSensSlider = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* MouseAimedYSensSlider = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* MouseAimedXSensSlider = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FVector2D MouseSensRange = FVector2D(20.0f, 200.0f);

    virtual void NativeOnInitialized() override;

private:
    void SetupSettingsComponents();
    UShooterSettingsSave* GetSettingsSave();

    void UpdateSettings();
    void UpdateInvertMouseYCheckBox();
    void UpdateMouseYSensSlider();
    void UpdateMouseXSensSlider();
    void UpdateMouseAimedYSensSlider();
    void UpdateMouseAimedXSensSlider();

    UFUNCTION()
    void OnCheckStateChangedInvertMouseY(bool bIsChecked);

    UFUNCTION()
    void OnMouseCaptureEndMouseYSens();

    UFUNCTION()
    void OnMouseCaptureEndMouseXSens();

    UFUNCTION()
    void OnMouseCaptureEndMouseAimedYSens();

    UFUNCTION()
    void OnMouseCaptureEndMouseAimedXSens();
};
