#pragma once

#include "SHGUICoreTypes.generated.h"

UENUM(BlueprintType)
enum class EHintType : uint8
{
    Startup,
    Max
};

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FName LevelName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FText LevelDisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* LevelThumbnails;
};

USTRUCT(BlueprintType)
struct FHintData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FText Title;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor TitleColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FText HintText;
};

DECLARE_MULTICAST_DELEGATE(FOnClickedButtonSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickedOptionsButtonSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedLevelSignature, const FLevelData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowPopUpHintSignature, const FHintData&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamageSignature, float, const FVector&, UPhysicalMaterial*)
