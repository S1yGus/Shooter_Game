#pragma once

#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"
#include "Engine/Blueprint.h"

namespace Tests
{
class LevelScope
{
public:
    LevelScope(const FString& LevelName) { AutomationOpenMap(LevelName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

template <class T>
T* SpawnBlueprint(UWorld* World, const FString& BlueprintName, const FTransform& Transform = FTransform::Identity)
{
    const auto Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintName);
    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

template <class T>
T* SpawnBlueprintDeferred(UWorld* World, const FString& BlueprintName, const FTransform& Transform = FTransform::Identity)
{
    const auto Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintName);
    return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

UWorld* GetCurrentWorld();

void CallFunctionByNameWithParameters(UObject* Object, const FString& FuncName, const TArray<FString>& Params);
}    // namespace Tests
