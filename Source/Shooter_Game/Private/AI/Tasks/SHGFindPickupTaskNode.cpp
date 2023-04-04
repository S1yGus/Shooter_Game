// Shooter_Game, All rights reserved.

#include "AI/Tasks/SHGFindPickupTaskNode.h"
#include "VisualLogger/VisualLogger.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

static struct FBTEnvQueryTaskMemory
{
    int32 RequestID;
};

USHGFindPickupTaskNode::USHGFindPickupTaskNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NodeName = "Finding Pickup EQS Query";

    QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &ThisClass::OnQueryFinished);
}

void USHGFindPickupTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    EQSRequest.InitForOwnerAndBlackboard(*this, GetBlackboardAsset());
}

void USHGFindPickupTaskNode::PostLoad()
{
    Super::PostLoad();

    if (!EQSRequest.QueryTemplate)
    {
        EQSRequest.QueryTemplate = QueryTemplate;
        EQSRequest.RunMode = RunMode;
    }
}

EBTNodeResult::Type USHGFindPickupTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (const auto AIController = OwnerComp.GetAIOwner())
    {
        if (const auto AIPawn = AIController->GetPawn(); AIPawn && EQSRequest.IsValid())
        {
            const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
            FBTEnvQueryTaskMemory* MyMemory = CastInstanceNodeMemory<FBTEnvQueryTaskMemory>(NodeMemory);

            FAIDynamicParam AIDynamicParam;
            AIDynamicParam.ParamName = EQSSearchRadiusParamName;
            AIDynamicParam.ParamType = EAIParamType::Float;
            AIDynamicParam.Value = GetSightSenseRadius(AIController);
            EQSRequest.QueryConfig.Add(AIDynamicParam);
            if (MyMemory->RequestID = EQSRequest.Execute(*AIPawn, BlackboardComponent, QueryFinishedDelegate); MyMemory->RequestID >= 0)
            {
                WaitForMessage(OwnerComp, UBrainComponent::AIMessage_QueryFinished, MyMemory->RequestID);
                return EBTNodeResult::InProgress;
            }
        }
    }

    return EBTNodeResult::Failed;
}

EBTNodeResult::Type USHGFindPickupTaskNode::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(OwnerComp.GetWorld()); QueryManager)
    {
        FBTEnvQueryTaskMemory* MyMemory = (FBTEnvQueryTaskMemory*)(NodeMemory);
        QueryManager->AbortQuery(MyMemory->RequestID);
    }

    return EBTNodeResult::Aborted;
}

FString USHGFindPickupTaskNode::GetStaticDescription() const
{
    return FString::Printf(TEXT("%s: %s\nResult Blackboard key: %s"),    //
                           *Super::GetStaticDescription(),               //
                           *GetNameSafe(EQSRequest.QueryTemplate),       //
                           *BlackboardKey.SelectedKeyName.ToString());
}

uint16 USHGFindPickupTaskNode::GetInstanceMemorySize() const
{
    return sizeof(FBTEnvQueryTaskMemory);
}

float USHGFindPickupTaskNode::GetSightSenseRadius(AActor* PerceptionComponentOwner) const
{
    const auto AIPerceptionComponent = PerceptionComponentOwner->FindComponentByClass<UAIPerceptionComponent>();
    if (!AIPerceptionComponent)
        return 0.0f;

    const auto SightConfig = Cast<UAISenseConfig_Sight>(AIPerceptionComponent->GetSenseConfig(UAISense::GetSenseID<UAISenseConfig_Sight>()));
    if (!SightConfig)
        return 0.0f;

    return SightConfig->SightRadius;
}

void USHGFindPickupTaskNode::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
    if (Result->IsAborted())
        return;

    const auto AIPawn = Cast<APawn>(Result->Owner.Get());
    if (!AIPawn)
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to get AI Pawn!"));
        return;
    }

    const auto AIController = AIPawn->GetController<AAIController>();
    if (!AIController)
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to get AI Controller!"));
        return;
    }

    UBehaviorTreeComponent* BehaviorTreeComponent = AIController ? AIController->FindComponentByClass<UBehaviorTreeComponent>() : nullptr;
    if (!BehaviorTreeComponent)
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to find behavior tree to notify about finished query from %s!"), *GetNameSafe(AIController));
        return;
    }

    const auto BlackboardComponent = BehaviorTreeComponent->GetBlackboardComponent();
    if (!BlackboardComponent)
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to get Blackboard Component!"));
        return;
    }

    bool bSuccess = Result.IsValid() && Result->IsSuccsessful() && !Result->Items.IsEmpty();
    if (bSuccess)
    {
        const auto bHasEnemy = static_cast<bool>(BlackboardComponent->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName));
        const auto Probability = FMath::GetMappedRangeValueClamped(FVector2D{0.0f, GetSightSenseRadius(AIController)},                      //
                                                                   bHasEnemy ? ProbabilityRangeWithEnemy : ProbabilityRangeWithoutEnemy,    //
                                                                   FVector::Distance(Result->GetItemAsActor(0)->GetActorLocation(), AIPawn->GetActorLocation()));

        if (UKismetMathLibrary::RandomBoolWithWeight(Probability))
        {
            UEnvQueryItemType* ItemTypeCDO = Result->ItemType->GetDefaultObject<UEnvQueryItemType>();
            bSuccess = ItemTypeCDO->StoreInBlackboard(BlackboardKey, BlackboardComponent, Result->RawData.GetData() + Result->Items[0].DataOffset);
            if (!bSuccess)
            {
                UE_VLOG(AIPawn, LogBehaviorTree, Warning, TEXT("Failed to store query result! item:%s key:%s"),
                        *UEnvQueryTypes::GetShortTypeName(Result->ItemType).ToString(), *UBehaviorTreeTypes::GetShortTypeName(BlackboardKey.SelectedKeyType));
            }
        }
    }

    FAIMessage::Send(BehaviorTreeComponent, FAIMessage(UBrainComponent::AIMessage_QueryFinished, this, Result->QueryID, bSuccess));
}
