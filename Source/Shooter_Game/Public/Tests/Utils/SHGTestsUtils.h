// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#pragma once

#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"
#include "Engine/Blueprint.h"
#include "Tests/Utils/InputRecordingTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "AutomationBlueprintFunctionLibrary.h"

namespace Tests
{
#define NO_VALUE -1.0f

#define ENUM_LOOP_START(EnumType, EnumElemName)                                    \
    for (int32 index = 0; index < StaticEnum<EnumType>()->NumEnums() - 1; ++index) \
    {                                                                              \
        const auto EnumElemName = static_cast<EnumType>(StaticEnum<EnumType>()->GetValueByIndex(index));

#define ENUM_LOOP_END }

class LevelScope
{
public:
    LevelScope(const FString& LevelName) { AutomationOpenMap(LevelName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

void SpecCloseLevel(const UWorld* World);

UWorld* GetCurrentWorld();

void CallFunctionByNameWithParameters(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

int32 GetActionBindingIndexByName(const UInputComponent* InputComponent, const FName& ActionName, EInputEvent InputEvent);

int32 GetAxisBindingIndexByName(const UInputComponent* InputComponent, const FName& AxisName);

void DoInputAction(const UInputComponent* InputComponent, const FName& ActionName, EInputEvent InputEvent, const FKey& Key);

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

class FSimulateMovmentLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovmentLatentCommand(const UWorld* InWorld, const UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindings);
    virtual ~FSimulateMovmentLatentCommand() override;

    virtual bool Update() override;

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> Bindings;
    int32 Index = 0;
    float WorldStartTime = NO_VALUE;
    float OldFPS = NO_VALUE;
};

class FTakeScreenshotLatentCommand : public IAutomationLatentCommand
{
public:
    FTakeScreenshotLatentCommand(const FString& InScreenshotName, const FAutomationScreenshotOptions& InOptions);

    virtual ~FTakeScreenshotLatentCommand() override;

protected:
    const FString ScreenshotName;
    const FAutomationScreenshotOptions Options;
    bool bScreenshotTaken{false};
    bool bScreenshotTakenAndCompared{false};

    virtual void OnScreenshotTakenAndCompared();
};

class FTakeGameScreenshotLatentCommand : public FTakeScreenshotLatentCommand
{
public:
    FTakeGameScreenshotLatentCommand(const FString& InScreenshotName, const FAutomationScreenshotOptions& InOptions);

    virtual bool Update() override;
};

class FTakeUIScreenshotLatentCommand : public FTakeScreenshotLatentCommand
{
public:
    FTakeUIScreenshotLatentCommand(const FString& InScreenshotName, const FAutomationScreenshotOptions& InOptions);

    virtual bool Update() override;

private:
    bool bVisualizeBufferSetupDone{false};

    virtual void OnScreenshotTakenAndCompared() override;
    void SetBufferVisualization(const FName& VisualizeBuffer);
};

template <typename EnumType, typename FunctionType>
void ForEach(FunctionType&& Function)
{
    const auto Enum = StaticEnum<EnumType>();
    for (int32 i = 0; i != Enum->NumEnums() - 1; ++i)
    {
        Function(static_cast<EnumType>(Enum->GetValueByIndex(i)));
    }
}

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

template <class T>
T* FindWidgetByClass()
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Tests::GetCurrentWorld(), Widgets, T::StaticClass(), false);
    return !Widgets.IsEmpty() ? Cast<T>(Widgets[0]) : nullptr;
}

template <class T>
T* FindWidgetByName(UUserWidget* UserWidget, const FName& WidgetName)
{
    if (!UserWidget)
        return nullptr;

    UWidget* FoundWidget = nullptr;
    UserWidget->WidgetTree->ForEachWidget(
        [&](UWidget* Widget)
        {
            if (Widget->GetFName().IsEqual(WidgetName))
            {
                FoundWidget = Widget;
            }
        });

    return Cast<T>(FoundWidget);
}

template <typename PropertyClass, typename ObjectClass>
PropertyClass GetPropertyValueByName(ObjectClass* Object, const FString& PropertyName)
{
    if (Object)
    {
        for (TFieldIterator<FProperty> PropIt(Object->StaticClass()); PropIt; ++PropIt)
        {
            const FProperty* Property = *PropIt;
            if (Property && Property->GetName().Equals(PropertyName))
            {
                return *Property->ContainerPtrToValuePtr<PropertyClass>(Object);
            }
        }
    }

    return PropertyClass();
}

template <typename T1, typename T2>
struct TestPayload
{
    T1 TestValue;
    T2 ExpectedValue;
};
}    // namespace Tests

#endif    // WITH_AUTOMATION_TESTS
