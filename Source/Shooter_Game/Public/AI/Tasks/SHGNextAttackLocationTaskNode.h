// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SHGNextAttackLocationTaskNode.generated.h"

class UEnvQuery;
struct FEnvQueryResult;

UCLASS()
class SHOOTER_GAME_API USHGNextAttackLocationTaskNode : public UBTTaskNode
{
    GENERATED_BODY()

public:
    USHGNextAttackLocationTaskNode();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
    UEnvQuery* EQS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
    FName InnerRadiusParamName{"Donut.InnerRadius"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
    FName OuterRadiusParamName{"Donut.OuterRadius"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector NewLocationBlackboardKey;

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    void HandleQueryResult(TSharedPtr<FEnvQueryResult> Result);
};
