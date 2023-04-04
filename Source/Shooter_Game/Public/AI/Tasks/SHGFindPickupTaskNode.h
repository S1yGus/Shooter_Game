// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SHGFindPickupTaskNode.generated.h"

class UBehaviorTree;
class UEnvQuery;

UCLASS()
class SHOOTER_GAME_API USHGFindPickupTaskNode : public UBTTask_BlackboardBase
{
    GENERATED_UCLASS_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "EQS")
    TObjectPtr<UEnvQuery> QueryTemplate;

    UPROPERTY(EditAnywhere, Category = "EQS")
    TEnumAsByte<EEnvQueryRunMode::Type> RunMode;

    UPROPERTY(EditAnywhere, Category = "EQS", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    FVector2D ProbabilityRangeWithoutEnemy{0.0f, 0.0f};

    UPROPERTY(EditAnywhere, Category = "EQS", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    FVector2D ProbabilityRangeWithEnemy{0.0f, 0.0f};

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector EnemyBlackboardKey;

    UPROPERTY(EditAnywhere, Category = "EQS")
    FName EQSSearchRadiusParamName{"ActorsOfClass.SearchRadius"};

private:
    FQueryFinishedSignature QueryFinishedDelegate;
    FEQSParametrizedQueryExecutionRequest EQSRequest;

    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
    virtual void PostLoad() override;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual FString GetStaticDescription() const override;
    virtual uint16 GetInstanceMemorySize() const override;
    inline float GetSightSenseRadius(AActor* PerceptionComponentOwner) const;

    void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);
};
