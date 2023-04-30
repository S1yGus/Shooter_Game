// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tests/Utils/InputRecordingTypes.h"

class JsonUtils
{
public:
    static bool WriteInputData(const FString& FileName, const FInputData& InputData);
    static bool ReadInputData(const FString& FileName, FInputData& InputData);
};
