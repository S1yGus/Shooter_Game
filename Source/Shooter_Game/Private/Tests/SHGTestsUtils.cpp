// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGTestsUtils.h"
#include "Misc/OutputDeviceNull.h"

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

int32 Tests::GetActionBindingIndexByName(UInputComponent* InputComponent, const FString& ActionName, EInputEvent InputEvent)
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

int32 Tests::GetAxisBindingIndexByName(UInputComponent* InputComponent, const FString& AxisName)
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

#endif    // WITH_AUTOMATION_TESTS

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
