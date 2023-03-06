// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SHGClearBlackboarbKeyTaskNode.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGClearBlackboarbKeyTaskNode : public UBTTaskNode
{
    GENERATED_BODY()

public:
    USHGClearBlackboarbKeyTaskNode();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector BlackboardKey;

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
