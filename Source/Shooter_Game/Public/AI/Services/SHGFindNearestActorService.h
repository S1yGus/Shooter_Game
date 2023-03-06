// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SHGFindNearestActorService.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGFindNearestActorService : public UBTService
{
    GENERATED_BODY()

public:
    USHGFindNearestActorService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyBlackboardKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
