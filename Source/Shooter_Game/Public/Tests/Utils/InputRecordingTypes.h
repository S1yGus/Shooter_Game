// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputRecordingTypes.generated.h"

USTRUCT()
struct FAxisData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    float Value;
};

USTRUCT()
struct FActionData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    TEnumAsByte<EInputEvent> KeyEvent;

    UPROPERTY()
    FKey Key;

    UPROPERTY()
    bool State;
};

USTRUCT()
struct FBindingsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    float WorldTime;

    UPROPERTY()
    float FPS;

    UPROPERTY()
    TArray<FAxisData> AxisValues;

    UPROPERTY()
    TArray<FActionData> ActionValues;
};

USTRUCT()
struct FInputData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FTransform InitialTransform;

    UPROPERTY()
    TArray<FBindingsData> Bindings;
};
