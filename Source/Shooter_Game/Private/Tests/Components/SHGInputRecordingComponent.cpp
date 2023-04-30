// Shooter_Game, All rights reserved.

#include "Tests/Components/SHGInputRecordingComponent.h"
#include "Tests/Utils/JsonUtils.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

USHGInputRecordingComponent::USHGInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USHGInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    InputData.Bindings.Add(GenerateBindingsData());
}

void USHGInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    const auto Pawn = GetOwner<APawn>();
    check(Pawn);

    const auto PC = Pawn->GetController<APlayerController>();
    check(PC);

    PlayerInput = PC->PlayerInput;
    check(PlayerInput);

    check(Pawn->InputComponent);
    check(GEngine);
    check(GEngine->GetGameUserSettings());

    InputData.InitialTransform = GetOwner()->GetTransform();

    OldFPS = GEngine->GetGameUserSettings()->GetFrameRateLimit();
    GEngine->GetGameUserSettings()->SetFrameRateLimit(RecordingFPS);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void USHGInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    JsonUtils::WriteInputData(GenerateFileName(), InputData);

    GEngine->GetGameUserSettings()->SetFrameRateLimit(OldFPS);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

FBindingsData USHGInputRecordingComponent::GenerateBindingsData() const
{
    FBindingsData BindingsData{.WorldTime{static_cast<float>(GetWorld()->TimeSeconds)}, .FPS{1.0f / GetWorld()->GetDeltaSeconds()}};
    for (const auto& AxisBinding : GetOwner()->InputComponent->AxisBindings)
    {
        BindingsData.AxisValues.Emplace(AxisBinding.AxisName, AxisBinding.AxisValue);
    }

    for (int32 i = 0; i != GetOwner()->InputComponent->GetNumActionBindings(); ++i)
    {
        const auto Action = GetOwner()->InputComponent->GetActionBinding(i);
        const auto ActionKeys = PlayerInput->GetKeysForAction(Action.GetActionName());
        if (!ActionKeys.IsEmpty())
        {
            const bool Pressed = PlayerInput->IsPressed(ActionKeys[0].Key);
            const bool State = (Pressed && Action.KeyEvent == EInputEvent::IE_Pressed) ||    //
                               (!Pressed && Action.KeyEvent == EInputEvent::IE_Released);
            BindingsData.ActionValues.Emplace(Action.GetActionName(), Action.KeyEvent, ActionKeys[0].Key, State);
        }
    }

    return BindingsData;
}

FString USHGInputRecordingComponent::GenerateFileName() const
{
    FString SavedDir = FPaths::ProjectSavedDir();
    return SavedDir.Append("/Tests/")
        .Append(FileName.ToString())
        .Append("_")
        .Append(UGameplayStatics::GetCurrentLevelName(this))
        .Append("_")
        .Append(FDateTime::Now().ToString())
        .Append(".json");
}
