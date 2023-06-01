// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/Utils/SHGTestsUtils.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/GameUserSettings.h"
#include "BufferVisualizationData.h"

void Tests::SpecCloseLevel(const UWorld* World)
{
    if (APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr)
    {
        PC->ConsoleCommand(TEXT("Exit"), true);
    }
}

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

int32 Tests::GetActionBindingIndexByName(const UInputComponent* InputComponent, const FName& ActionName, EInputEvent InputEvent)
{
    if (InputComponent)
    {
        for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
        {
            const auto Action = InputComponent->GetActionBinding(i);
            if (Action.GetActionName() == ActionName && Action.KeyEvent == InputEvent)
            {
                return i;
            }
        }
    }

    return INDEX_NONE;
}

int32 Tests::GetAxisBindingIndexByName(const UInputComponent* InputComponent, const FName& AxisName)
{
    if (InputComponent)
    {
        return InputComponent->AxisBindings.IndexOfByPredicate(
            [&](const FInputAxisBinding& AxisBinding)
            {
                return AxisBinding.AxisName == AxisName;
            });
    }

    return INDEX_NONE;
}

void Tests::DoInputAction(const UInputComponent* InputComponent, const FName& ActionName, EInputEvent InputEvent, const FKey& Key)
{
    if (InputComponent)
    {
        const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, ActionName, InputEvent);
        if (ActionIndex != INDEX_NONE)
        {
            InputComponent->GetActionBinding(ActionIndex).ActionDelegate.Execute(Key);
        }
    }
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

Tests::FSimulateMovmentLatentCommand::FSimulateMovmentLatentCommand(const UWorld* InWorld, const UInputComponent* InInputComponent,
                                                                    const TArray<FBindingsData>& InBindings)
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
            const auto AxisIndex = GetAxisBindingIndexByName(InputComponent, AxisData.Name);
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
                    const auto ActionIndex = GetActionBindingIndexByName(InputComponent, CurrentAction.Name, CurrentAction.KeyEvent);
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

Tests::FTakeScreenshotLatentCommand::FTakeScreenshotLatentCommand(const FString& InScreenshotName, const FAutomationScreenshotOptions& InOptions)
    : ScreenshotName{InScreenshotName}, Options{InOptions}
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
}

Tests::FTakeScreenshotLatentCommand::~FTakeScreenshotLatentCommand()
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
}

void Tests::FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
    bScreenshotTakenAndCompared = true;
}

Tests::FTakeGameScreenshotLatentCommand::FTakeGameScreenshotLatentCommand(const FString& InScreenshotName, const FAutomationScreenshotOptions& InOptions)
    : FTakeScreenshotLatentCommand(InScreenshotName, InOptions)
{
}

bool Tests::FTakeGameScreenshotLatentCommand::Update()
{
    if (FPlatformTime::Seconds() - StartTime < Options.Delay)
        return false;

    if (!bScreenshotTaken)
    {
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(GetCurrentWorld(), ScreenshotName, {}, Options);
        bScreenshotTaken = true;
    }

    return bScreenshotTakenAndCompared;
}

Tests::FTakeUIScreenshotLatentCommand::FTakeUIScreenshotLatentCommand(const FString& InScreenshotName, const FAutomationScreenshotOptions& InOptions)
    : FTakeScreenshotLatentCommand(InScreenshotName, InOptions)
{
}

bool Tests::FTakeUIScreenshotLatentCommand::Update()
{
    if (FPlatformTime::Seconds() - StartTime < Options.Delay)
        return false;

    if (!bVisualizeBufferSetupDone)
    {
        SetBufferVisualization(Options.VisualizeBuffer);
        bVisualizeBufferSetupDone = true;
        return false;
    }

    if (!bScreenshotTaken)
    {
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(GetCurrentWorld(), ScreenshotName, Options);
        bScreenshotTaken = true;
    }

    return bScreenshotTakenAndCompared;
}

void Tests::FTakeUIScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
    FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared();
    SetBufferVisualization(NAME_None);
}

void Tests::FTakeUIScreenshotLatentCommand::SetBufferVisualization(const FName& VisualizeBuffer)
{
    if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
    {
        static IConsoleVariable* ICVar = IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
        if (ICVar)
        {
            if (ViewportClient->GetEngineShowFlags())
            {
                ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(VisualizeBuffer == NAME_None ? false : true);
                ViewportClient->GetEngineShowFlags()->SetTonemapper(VisualizeBuffer == NAME_None ? true : false);
                ICVar->Set(VisualizeBuffer == NAME_None ? TEXT("") : *VisualizeBuffer.ToString());
            }
        }
    }
}

#endif    // WITH_AUTOMATION_TESTS
