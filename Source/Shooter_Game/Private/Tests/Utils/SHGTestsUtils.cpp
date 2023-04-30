// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/Utils/SHGTestsUtils.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/GameUserSettings.h"

UWorld* Tests::GetCurrentWorld()
{
    if (!GEngine)
        return nullptr;

    for (const auto& Context : GEngine->GetWorldContexts())
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::GamePreview) && Context.World() != nullptr)
        {
            return Context.World();
        }
    }

    return nullptr;
}

void Tests::CallFunctionByNameWithParameters(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    FString Command{FuncName};
    for (const auto& Param : Params)
    {
        Command.Append(" ").Append(Param);
    }

    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

int32 Tests::GetActionBindingIndexByName(const UInputComponent* InputComponent, const FString& ActionName, EInputEvent InputEvent)
{
    if (InputComponent)
    {
        for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
        {
            const auto Action = InputComponent->GetActionBinding(i);
            if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
            {
                return i;
            }
        }
    }

    return INDEX_NONE;
}

int32 Tests::GetAxisBindingIndexByName(const UInputComponent* InputComponent, const FString& AxisName)
{
    if (InputComponent)
    {
        return InputComponent->AxisBindings.IndexOfByPredicate(
            [&](const FInputAxisBinding& AxisBinding)
            {
                return AxisBinding.AxisName.ToString().Equals(AxisName);
            });
    }

    return INDEX_NONE;
}

Tests::FUntilLatentCommand::FUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
    : Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
{
}

bool Tests::FUntilLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();

        return true;
    }

    Callback();

    return false;
}

Tests::FSimulateMovmentLatentCommand::FSimulateMovmentLatentCommand(const UWorld* InWorld, const UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindings)
    : World(InWorld), InputComponent(InInputComponent), Bindings(InBindings)
{
}

Tests::FSimulateMovmentLatentCommand::~FSimulateMovmentLatentCommand()
{
    if (GEngine && GEngine->GetGameUserSettings())
    {
        GEngine->GetGameUserSettings()->SetFrameRateLimit(OldFPS);
        GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
    }
}

bool Tests::FSimulateMovmentLatentCommand::Update()
{
    if (!World || !InputComponent || !GEngine || !GEngine->GetGameUserSettings())
        return true;

    if (WorldStartTime == NO_VALUE)
    {
        WorldStartTime = World->TimeSeconds;
    }

    if (OldFPS == NO_VALUE)
    {
        OldFPS = GEngine->GetGameUserSettings()->GetFrameRateLimit();
    }

    if (Bindings[Index].WorldTime <= World->TimeSeconds - WorldStartTime)
    {
        GEngine->GetGameUserSettings()->SetFrameRateLimit(Bindings[Index].FPS);
        GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();

        for (const auto& AxisData : Bindings[Index].AxisValues)
        {
            const auto AxisIndex = GetAxisBindingIndexByName(InputComponent, AxisData.Name.ToString());
            if (AxisIndex != INDEX_NONE)
            {
                InputComponent->AxisBindings[AxisIndex].AxisDelegate.Execute(AxisData.Value);
            }
        }

        if (Index > 0)
        {
            for (int32 i = 0; i != Bindings[Index].ActionValues.Num(); ++i)
            {
                const auto CurrentAction = Bindings[Index].ActionValues[i];
                const auto PrevAction = Bindings[Index - 1].ActionValues[i];
                if (CurrentAction.State && !PrevAction.State)
                {
                    const auto ActionIndex = GetActionBindingIndexByName(InputComponent, CurrentAction.Name.ToString(), CurrentAction.KeyEvent);
                    if (ActionIndex != INDEX_NONE)
                    {
                        InputComponent->GetActionBinding(ActionIndex).ActionDelegate.Execute(CurrentAction.Key);
                    }
                }
            }
        }

        if (++Index == Bindings.Num())
        {
            return true;
        }
    }

    return false;
}

#endif    // WITH_AUTOMATION_TESTS
