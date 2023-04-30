// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tests/Utils/InputRecordingTypes.h"
#include "SHGInputRecordingComponent.generated.h"

class UPlayerInput;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGInputRecordingComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputRecording")
    FName FileName{"TestInput"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputRecording")
    float RecordingFPS{30.0f};

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FInputData InputData;
    const UPlayerInput* PlayerInput;
    float OldFPS;

    FBindingsData GenerateBindingsData() const;
    FString GenerateFileName() const;
};
