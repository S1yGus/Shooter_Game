// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SHGSettingsCoreTypes.h"
#include "SHGGameUserSettings.generated.h"

class USHGSetting;
class USHGIntSetting;
class USHGFloatSetting;
class USHGActionSetting;
class USHGBoolSetting;
class USHGSettingsSave;

UCLASS()
class SHOOTER_GAME_API USHGGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    FOnResolutionChangedSignature OnResolutionChanged;
    FOnAspectRatioChangedSignature OnAspectRatioChanged;
    FOnSensitivityChangedSignature OnSensitivityChanged;
    FOnHintsStatusChangedSignature OnHintsStatusChanged;

    USHGGameUserSettings();

    static USHGGameUserSettings* Get();

    inline const TArray<FSettingsData>& GetSettings() const { return Settings; }
    const FAspectRatioData& GetAspectRatio() const;
    const FSensitivitySettings& GetSensitivitySettings() const;
    const TMap<EHintType, bool>& GetHintsStatus() const;
    void SetHintsStatus(const TMap<EHintType, bool>& NewHintsMap);
    void SetLastConfirmedResolutionSettings();

    void InitSoundVolume();

    virtual void LoadSettings(bool bForceReload = false) override;
    virtual void SaveSettings() override;

private:
    UPROPERTY()
    TArray<FSettingsData> Settings;
    UPROPERTY()
    USHGIntSetting* ResolutionSetting;

    UPROPERTY()
    USHGSettingsSave* SettingsSave;

    void InitVideoSettings();
    void InitSoundSettings();
    void InitControlSettings();
    void InitGameSettings();

    TArray<FText> GetScreenResolutions() const;
    void UpdateResolutionSetting();
    void SetLowestResolution();
    void SetSoundClassVolume(const FString& SoundClassName, float NewVolume);

    void CheckSettingsSave();

    inline USHGIntSetting* CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<USHGSetting*>& AddTo);
    inline USHGFloatSetting* CreateFloatSetting(const FText& Name, TArray<USHGSetting*>& AddTo);
    inline USHGActionSetting* CreateActionSetting(const FText& Name, const FText& ActionName, TArray<USHGSetting*>& AddTo);
    inline USHGBoolSetting* CreateBoolSetting(const FText& Name, TArray<USHGSetting*>& AddTo);

    template <class T>
    T* CreateSetting(const FText& Name, TArray<USHGSetting*>& AddTo)
    {
        T* Setting = NewObject<T>();
        checkf(Setting->IsA(USHGSetting::StaticClass()), TEXT("T must be based on USHGSetting"));
        Setting->SetName(Name);
        AddTo.Add(Setting);

        return Setting;
    }
};