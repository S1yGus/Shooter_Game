// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SHGNextPatrolLocationTaskNode.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGNextPatrolLocationTaskNode : public UBTTaskNode
{
    GENERATED_BODY()

public:
    USHGNextPatrolLocationTaskNode();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float PatrolRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector NewLocationBlackboardKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool bSelfCenter = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (EditCondition = "!bSelfCenter"))
    FBlackboardKeySelector TargetActorBlackboardKey;

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
