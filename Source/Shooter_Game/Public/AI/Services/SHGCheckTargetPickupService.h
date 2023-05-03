// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SHGCheckTargetPickupService.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGCheckTargetPickupService : public UBTService
{
    GENERATED_BODY()

public:
    USHGCheckTargetPickupService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector TargetPickupBlackboardKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
