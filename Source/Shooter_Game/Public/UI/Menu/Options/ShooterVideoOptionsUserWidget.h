// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterVideoOptionsUserWidget.generated.h"

class UComboBoxString;
class USlider;
class UShooterOptionsWarningUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterVideoOptionsUserWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ScreenModeComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ScreenResolutionComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* VSyncComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ViewDistanceComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* AntiAliasingComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* PostProcessingComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ShadowComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* TextureComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* EffectComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* FoliageComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UComboBoxString* ShadingComboBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    USlider* ResolutionScaleSlider = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterOptionsWarningUserWidget> OptionsWarningUserWidget;

    virtual void NativeOnInitialized() override;

private:
    void SetupSettingsComponents();

    void UpdateSettings();
    void UpdateScreenModeComboBox();
    void UpdateScreenResolutionComboBox();
    void UpdateVSyncComboBox();
    void UpdateResolutionScaleSlider();
    void UpdateViewDistanceComboBox();
    void UpdateAntiAliasingComboBox();
    void UpdatePostProcessingComboBox();
    void UpdateShadowComboBox();
    void UpdateTextureComboBox();
    void UpdateEffectComboBox();
    void UpdateFoliageComboBox();
    void UpdateShadingComboBox();

    UFUNCTION()
    void OnSelectionChangedScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedScreenResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedVSync(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnValueChangedResolutionScale();

    UFUNCTION()
    void OnSelectionChangedViewDistance(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedAntiAliasing(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedPostProcessing(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedShadow(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedTexture(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedEffect(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedFoliage(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnSelectionChangedShading(FString SelectedItem, ESelectInfo::Type SelectionType);
};
