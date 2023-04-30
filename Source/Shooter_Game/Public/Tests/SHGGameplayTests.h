// Shooter_Game, All rights reserved.

#pragma once

#include <SHGGameplayTests.generated.h>

USTRUCT()
struct FRecMovementTestData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString LevelPath;

    UPROPERTY()
    int32 PickupsAmount;

    UPROPERTY()
    FString RecInputFileName;
};
