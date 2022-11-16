#pragma once

#include "SHGBaseCoreTypes.h"
#include "SHGUICoreTypes.generated.h"

UENUM(BlueprintType)
enum class EHintType : uint8
{
    Startup,
    Max
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

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairIcon;
};

DECLARE_MULTICAST_DELEGATE(FOnClickedButtonSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickedOptionsButtonSignature, int32);    // int32 WidgetID
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedLevelSignature, const FLevelData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowPopUpHintSignature, const FHintData&);
