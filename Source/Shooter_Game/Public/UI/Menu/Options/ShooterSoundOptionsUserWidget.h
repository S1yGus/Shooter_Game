// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterSoundOptionsUserWidget.generated.h"

class UComboBoxString;
class USlider;
class UShooterSettingsSave;

UCLASS()
class SHOOTER_GAME_API UShooterSoundOptionsUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* SoundQualityComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* MasterVolumeScaleSlider = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* CharacterVolumeScaleSlider = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* EffectsVolumeScaleSlider = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* AmbientVolumeScaleSlider = nullptr;

    virtual void NativeOnInitialized() override;

private:
    void SetupSettingsComponents();
    UShooterSettingsSave* GetSettingsSave();

    void UpdateSettings();
    void UpdateSoundQualityComboBox();
    void UpdateMasterVolumeSlider();
    void UpdateCharacterVolumeSlider();
    void UpdateEffectsVolumeSlider();
    void UpdateAmbientVolumeSlider();

    UFUNCTION()
    void OnSelectionChangedSoundQuality(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnValueChangedMasterVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndMasterVolume();

    UFUNCTION()
    void OnValueChangedCharacterVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndCharacterVolume();

    UFUNCTION()
    void OnValueChangedEffectsVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndEffectsVolume();

    UFUNCTION()
    void OnValueChangedAmbientVolume(float Value);

    UFUNCTION()
    void OnMouseCaptureEndAmbientVolume();
};
