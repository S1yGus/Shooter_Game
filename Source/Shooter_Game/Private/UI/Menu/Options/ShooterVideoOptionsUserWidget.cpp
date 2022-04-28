// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterVideoOptionsUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Menu/Options/ShooterOptionsWarningUserWidget.h"

void UShooterVideoOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UpdateSettings();
    SetupSettingsComponents();
}

void UShooterVideoOptionsUserWidget::SetupSettingsComponents()
{
    if (ScreenModeComboBox)
    {
        ScreenModeComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedScreenMode);
    }

    if (ScreenResolutionComboBox)
    {
        ScreenResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedScreenResolution);
    }

    if (VSyncComboBox)
    {
        VSyncComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedVSync);
    }

    if (ResolutionScaleSlider)
    {
        ResolutionScaleSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnValueChangedResolutionScale);
    }

    if (ViewDistanceComboBox)
    {
        ViewDistanceComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedViewDistance);
    }

    if (AntiAliasingComboBox)
    {
        AntiAliasingComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedAntiAliasing);
    }

    if (PostProcessingComboBox)
    {
        PostProcessingComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedPostProcessing);
    }

    if (ShadowComboBox)
    {
        ShadowComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedShadow);
    }

    if (IlluminationComboBox)
    {
        IlluminationComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedIllumination);
    }

    if (ReflectionsComboBox)
    {
        ReflectionsComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedReflections);
    }

    if (TextureComboBox)
    {
        TextureComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedTexture);
    }

    if (EffectComboBox)
    {
        EffectComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedEffect);
    }

    if (FoliageComboBox)
    {
        FoliageComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedFoliage);
    }

    if (ShadingComboBox)
    {
        ShadingComboBox->OnSelectionChanged.AddDynamic(this, &UShooterVideoOptionsUserWidget::OnSelectionChangedShading);
    }
}

void UShooterVideoOptionsUserWidget::UpdateSettings()
{
    UpdateScreenModeComboBox();
    UpdateScreenResolutionComboBox();
    UpdateVSyncComboBox();
    UpdateResolutionScaleSlider();
    UpdateViewDistanceComboBox();
    UpdateAntiAliasingComboBox();
    UpdatePostProcessingComboBox();
    UpdateShadowComboBox();
    UpdateIlluminationComboBox();
    UpdateReflectionsComboBox();
    UpdateTextureComboBox();
    UpdateEffectComboBox();
    UpdateFoliageComboBox();
    UpdateShadingComboBox();
}

void UShooterVideoOptionsUserWidget::UpdateScreenModeComboBox()
{
    if (!ScreenModeComboBox)
        return;

    const auto ScreenMode = GEngine->GetGameUserSettings()->GetFullscreenMode();
    ScreenModeComboBox->SetSelectedIndex(static_cast<int32>(ScreenMode));
}

void UShooterVideoOptionsUserWidget::UpdateScreenResolutionComboBox()
{
    if (!ScreenResolutionComboBox)
        return;

    const auto ScreenMode = GEngine->GetGameUserSettings()->GetFullscreenMode();

    TArray<FIntPoint> Resolutions;
    switch (ScreenMode)
    {
        case EWindowMode::Fullscreen:
        case EWindowMode::WindowedFullscreen:
            UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
            break;
        case EWindowMode::Windowed:
            UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions);
            break;
    }

    ScreenResolutionComboBox->ClearOptions();
    ScreenResolutionComboBox->SetIsEnabled(!(GEngine->GetGameUserSettings()->GetFullscreenMode() == EWindowMode::WindowedFullscreen));

    for (const auto Resolution : Resolutions)
    {
        const FString Option = FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y);
        ScreenResolutionComboBox->AddOption(Option);
    }

    const auto CurrentResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
    const FString Option = FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y);
    ScreenResolutionComboBox->SetSelectedOption(Option);
}

void UShooterVideoOptionsUserWidget::UpdateVSyncComboBox()
{
    if (!VSyncComboBox)
        return;

    const auto VSync = GEngine->GetGameUserSettings()->IsVSyncEnabled();
    VSyncComboBox->SetSelectedIndex(static_cast<int32>(VSync));
}

void UShooterVideoOptionsUserWidget::UpdateResolutionScaleSlider()
{
    if (!ResolutionScaleSlider)
        return;

    const auto ResolutionScale = GEngine->GetGameUserSettings()->GetResolutionScaleNormalized();
    ResolutionScaleSlider->SetValue(ResolutionScale);
}

void UShooterVideoOptionsUserWidget::UpdateViewDistanceComboBox()
{
    if (!ViewDistanceComboBox)
        return;

    const auto ViewDistanceQuality = GEngine->GetGameUserSettings()->GetViewDistanceQuality();
    ViewDistanceComboBox->SetSelectedIndex(ViewDistanceQuality);
}

void UShooterVideoOptionsUserWidget::UpdateAntiAliasingComboBox()
{
    if (!AntiAliasingComboBox)
        return;

    const auto AntiAliasingQuality = GEngine->GetGameUserSettings()->GetAntiAliasingQuality();
    AntiAliasingComboBox->SetSelectedIndex(AntiAliasingQuality);
}

void UShooterVideoOptionsUserWidget::UpdatePostProcessingComboBox()
{
    if (!PostProcessingComboBox)
        return;

    const auto PostProcessingQuality = GEngine->GetGameUserSettings()->GetPostProcessingQuality();
    PostProcessingComboBox->SetSelectedIndex(PostProcessingQuality);
}

void UShooterVideoOptionsUserWidget::UpdateShadowComboBox()
{
    if (!ShadowComboBox)
        return;

    const auto ShadowsQuality = GEngine->GetGameUserSettings()->GetShadowQuality();
    ShadowComboBox->SetSelectedIndex(ShadowsQuality);
}

void UShooterVideoOptionsUserWidget::UpdateIlluminationComboBox()
{
    if (!IlluminationComboBox)
        return;

    const auto IlluminationQuality = GEngine->GetGameUserSettings()->GetGlobalIlluminationQuality();
    IlluminationComboBox->SetSelectedIndex(IlluminationQuality);
}

void UShooterVideoOptionsUserWidget::UpdateReflectionsComboBox()
{
    if (!ReflectionsComboBox)
        return;

    const auto ReflectionQuality = GEngine->GetGameUserSettings()->GetReflectionQuality();
    ReflectionsComboBox->SetSelectedIndex(ReflectionQuality);
}

void UShooterVideoOptionsUserWidget::UpdateTextureComboBox()
{
    if (!TextureComboBox)
        return;

    const auto TexturesQuality = GEngine->GetGameUserSettings()->GetTextureQuality();
    TextureComboBox->SetSelectedIndex(TexturesQuality);
}

void UShooterVideoOptionsUserWidget::UpdateEffectComboBox()
{
    if (!EffectComboBox)
        return;

    const auto EffectsQuality = GEngine->GetGameUserSettings()->GetVisualEffectQuality();
    EffectComboBox->SetSelectedIndex(EffectsQuality);
}

void UShooterVideoOptionsUserWidget::UpdateFoliageComboBox()
{
    if (!FoliageComboBox)
        return;

    const auto FoliageQuality = GEngine->GetGameUserSettings()->GetFoliageQuality();
    FoliageComboBox->SetSelectedIndex(FoliageQuality);
}

void UShooterVideoOptionsUserWidget::UpdateShadingComboBox()
{
    if (!ShadingComboBox)
        return;

    const auto ShadingQuality = GEngine->GetGameUserSettings()->GetShadingQuality();
    ShadingComboBox->SetSelectedIndex(ShadingQuality);
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedScreenMode(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto ScreenMode = ScreenModeComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetFullscreenMode(static_cast<EWindowMode::Type>(ScreenMode));
    GEngine->GetGameUserSettings()->ApplyResolutionSettings(false);
    UpdateScreenResolutionComboBox();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedScreenResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    FString LeftS, RightS;
    if (SelectedItem.Split(" x ", &LeftS, &RightS))
    {
        FIntPoint NewResolution = FIntPoint(FCString::Atoi(*LeftS), FCString::Atoi(*RightS));
        GEngine->GetGameUserSettings()->SetScreenResolution(NewResolution);
        GEngine->GetGameUserSettings()->ApplyResolutionSettings(false);
    }

    const auto OptionsWarningWidget = CreateWidget<UShooterOptionsWarningUserWidget>(this, OptionsWarningUserWidget);
    if (OptionsWarningWidget)
    {
        OptionsWarningWidget->AddToViewport();
        OptionsWarningWidget->OnCancelResolutionSettings.AddUObject(this, &UShooterVideoOptionsUserWidget::UpdateScreenResolutionComboBox);
    }
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedVSync(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto VSync = VSyncComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetVSyncEnabled(static_cast<bool>(VSync));
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnValueChangedResolutionScale()
{
    const auto ResolutionScale = ResolutionScaleSlider->GetNormalizedValue();
    GEngine->GetGameUserSettings()->SetResolutionScaleNormalized(ResolutionScale);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedViewDistance(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto ViewDistanceQuality = ViewDistanceComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetViewDistanceQuality(ViewDistanceQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedAntiAliasing(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto AntiAliasingQuality = AntiAliasingComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetAntiAliasingQuality(AntiAliasingQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedPostProcessing(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto PostProcessingQuality = PostProcessingComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetPostProcessingQuality(PostProcessingQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedShadow(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto ShadowQuality = ShadowComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetShadowQuality(ShadowQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedIllumination(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto IlluminationQuality = IlluminationComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(IlluminationQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedReflections(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto ReflectionsQuality = ReflectionsComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetReflectionQuality(ReflectionsQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedTexture(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto TextureQuality = TextureComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetTextureQuality(TextureQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedEffect(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto EffectQuality = EffectComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetVisualEffectQuality(EffectQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedFoliage(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto FoliageQuality = FoliageComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetFoliageQuality(FoliageQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterVideoOptionsUserWidget::OnSelectionChangedShading(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto ShadingQuality = ShadingComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetShadingQuality(ShadingQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}
