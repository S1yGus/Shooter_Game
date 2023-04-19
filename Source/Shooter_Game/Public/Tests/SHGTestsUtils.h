// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#pragma once

#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"
#include "Engine/Blueprint.h"

namespace Tests
{
#define ENUM_LOOP_START(EnumType, EnumElemName)                                    \
    for (int32 index = 0; index < StaticEnum<EnumType>()->NumEnums() - 1; ++index) \
    {                                                                              \
        const auto EnumElemName = static_cast<EnumType>(index);

#define ENUM_LOOP_END }

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

template <typename T1, typename T2>
struct TestPayload
{
    T1 TestValue;
    T2 ExpectedValue;
};

UWorld* GetCurrentWorld();

void CallFunctionByNameWithParameters(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

int32 GetActionBindingIndexByName(UInputComponent* InputComponent, const FString& ActionName, EInputEvent InputEvent);

int32 GetAxisBindingIndexByName(UInputComponent* InputComponent, const FString& AxisName);

class FUntilLatentCommand : public IAutomationLatentCommand
{
public:
    FUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout);

    virtual bool Update() override;

private:
    TFunction<void()> Callback;
    TFunction<void()> TimeoutCallback;
    float Timeout;
};
}    // namespace Tests

#endif    // WITH_AUTOMATION_TESTS
