// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "SHGAIPerceptionComponent.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    AActor* GetNearestActor();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName InspectionStateBlackboardKeyName{"bInspectionState"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName LocationToInspectBlackboardKeyName{"LocationToInspect"};

private:
    UFUNCTION()
    void HandleInspectionState(const FActorPerceptionUpdateInfo& UpdateInfo);
};
