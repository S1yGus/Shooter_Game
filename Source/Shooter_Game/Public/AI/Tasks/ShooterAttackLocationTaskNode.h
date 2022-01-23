// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ShooterAttackLocationTaskNode.generated.h"

struct FEnvQueryResult;
class UBlackboardComponent;
class UEnvQuery;

UCLASS()
class SHOOTER_GAME_API UShooterAttackLocationTaskNode : public UBTTaskNode
{
    GENERATED_BODY()

    UPROPERTY()
    UBlackboardComponent* Blackboard;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    void HandleQueryResult(TSharedPtr<FEnvQueryResult> Result);

public:
    UShooterAttackLocationTaskNode();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UEnvQuery* EQS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector NewLocationBlackboardKey;
};
