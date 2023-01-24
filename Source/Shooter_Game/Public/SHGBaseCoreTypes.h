#pragma once

#include "SHGBaseCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitingToStart,
    MainMenu,
    Options,
    OptionsWarning,
    PopUpHint,
    Game,
    Spectating,
    Pause,
    BackToRootMenu,
    GameOver,
    Quit,
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FText LevelDisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelThumbnails;
};

USTRUCT(BlueprintType)
struct FArenaData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "64"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "4"))
    int32 TeamsNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FLinearColor DefaultTeamColor{FLinearColor::Gray};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TArray<FLinearColor> TeamsColors{FLinearColor::Red, FLinearColor::Blue, FLinearColor::Yellow, FLinearColor::Green};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "99"))
    int32 RaundsNum = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1"))
    int32 RaundTime = 300;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1"))
    int32 RespawnTime = 5;
};

// Health
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AController*, AController*);    // AController* KillerController, AController* VictimController
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);          // float HealthAmount, float HealthPercent

// Stamina
DECLARE_MULTICAST_DELEGATE(FOnOutOfStaminaSignature);
DECLARE_MULTICAST_DELEGATE(FOnNotEnoughStaminaSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature, float, float);    // float StaminaAmount, float StaminaPercent

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerStateChangedSignature, int32, int32);    // int32 KillsNum, int32 DeathsNum
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNewRaundSignature, int32, int32);              // int32 CurrentRaund, int32 RaundsNum
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTakeLocalDamageSignature, float, const FVector&,
                                       UPhysicalMaterial*)    // float DamageAmount, const FVector& HitLocation, UPhysicalMaterial* PhysicalMaterial
