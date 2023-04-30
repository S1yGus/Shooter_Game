// Shooter_Game, All rights reserved.

#include "Tests/Utils/JsonUtils.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonUtils, All, All)

bool JsonUtils::WriteInputData(const FString& FileName, const FInputData& InputData)
{
    TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(InputData);
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("Json conversion error!"))
        return false;
    }

    FString JsonString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("Json serialization error!"))
        return false;
    }

    if (!JsonWriter->Close())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("Json writer closing error!"))
        return false;
    }

    if (!FFileHelper::SaveStringToFile(JsonString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("File saving error!"))
        return false;
    }

    return true;
}

bool JsonUtils::ReadInputData(const FString& FileName, FInputData& InputData)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("File loading error!"))
        return false;
    }

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("Json deserialization error!"))
        return false;
    }

    return FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InputData);
}
