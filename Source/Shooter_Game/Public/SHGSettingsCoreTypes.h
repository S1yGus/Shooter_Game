#pragma once

#include "SHGUICoreTypes.h"
#include "SHGSettingsCoreTypes.generated.h"

UENUM(BlueprintType)
enum class ESettingType : uint8
{
    Video,
    Sound,
    Control,
    Game,
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FAspectRatioData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VideoSettings")
    float AspectRatio = 1.7777777f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VideoSettings")
    float FOV = 90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VideoSettings")
    FText DisplayName{FText::FromString("16:9")};
};

USTRUCT(BlueprintType)
struct FCultureData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Culture")
    FString Culture;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Culture")
    FText CultureName;
};

USTRUCT(BlueprintType)
struct FVideoSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VideoSettings")
    FAspectRatioData AspectRatioData;
};

USTRUCT(BlueprintType)
struct FSoundSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundSettings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MasterVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundSettings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float UIVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundSettings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float FXVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundSettings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MusicVolume = 0.5f;
};

USTRUCT(BlueprintType)
struct FGameSettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSettings")
    TMap<EHintType, bool> HintsStatus{{EHintType::Startup, true}};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSettings", Meta = (ClampMin = "1.0", ClampMax = "2.0"))
    float Difficulty = 1.0f;
};

USTRUCT(BlueprintType)
struct FSensitivitySettings
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ControlSettings")
    float MouseXSens = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ControlSettings")
    float MouseYSens = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ControlSettings")
    float MouseAimedXSens = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ControlSettings")
    float MouseAimedYSens = 0.25f;
};

USTRUCT(BlueprintType)
struct FSettingsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    ESettingType Type;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    TArray<class USHGSetting*> Settings;

    bool operator==(const FSettingsData& Data) const { return Type == Data.Type; }
};

using HintsStatus = TMap<EHintType, bool>;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHintsStatusChangedSignature, const HintsStatus&);
DECLARE_MULTICAST_DELEGATE(FOnResolutionChangedSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAspectRatioChangedSignature, const FAspectRatioData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSensitivityChangedSignature, const FSensitivitySettings&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDifficultyChangedSignature, float);
DECLARE_MULTICAST_DELEGATE(FOnPressedEntSignature);
DECLARE_MULTICAST_DELEGATE(FOnPressedEscSignature);
