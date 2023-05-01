// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Tests/Utils/InputRecordingTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonUtils, All, All)

class JsonUtils
{
public:
    template <typename T>
    static bool WriteUStructToFile(const FString& FileName, const T& UStruct)
    {
        FString JsonString;
        if (!FJsonObjectConverter::UStructToJsonObjectString(UStruct, JsonString))
        {
            UE_LOG(LogJsonUtils, Error, TEXT("Json conversion error!"))
            return false;
        }

        if (!FFileHelper::SaveStringToFile(JsonString, *FileName))
        {
            UE_LOG(LogJsonUtils, Error, TEXT("File saving error!"))
            return false;
        }

        return true;
    }

    template <typename T>
    static bool ReadUStructFromFile(const FString& FileName, T& UStruct)
    {
        FString JsonString;
        if (!FFileHelper::LoadFileToString(JsonString, *FileName))
        {
            UE_LOG(LogJsonUtils, Error, TEXT("File loading error!"))
            return false;
        }

        return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UStruct);
    }
};
