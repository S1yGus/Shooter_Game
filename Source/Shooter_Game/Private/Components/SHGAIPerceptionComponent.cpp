// Shooter_Game, All rights reserved.

#include "Components/SHGAIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Prediction.h"
#include "Components/SHGHealthComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SHGUtils.h"

void USHGAIPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();

    OnTargetPerceptionInfoUpdated.AddDynamic(this, &ThisClass::CheckForEnemyLost);
}

AActor* USHGAIPerceptionComponent::GetNearestActor()
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
    if (PerceiveActors.IsEmpty())
        return nullptr;

    const auto AIController = Cast<AAIController>(GetOwner());
    if (!AIController)
        return nullptr;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return nullptr;

    float NearestDistance = MAX_FLT;
    AActor* NearestActor = nullptr;
    for (const auto PerceiveActor : PerceiveActors)
    {
        const auto PerceivePawn = Cast<APawn>(PerceiveActor);
        if (!PerceivePawn)
            continue;

        const auto PerceivePawnController = PerceivePawn->GetController();
        if (!PerceivePawnController)
            continue;

        const auto HealthComponent = PerceiveActor->FindComponentByClass<USHGHealthComponent>();
        if (!HealthComponent || HealthComponent->IsDead() || !SHGUtils::AreEnemies(AIController, PerceivePawnController))
            continue;

        const auto CurrentDistance = (PerceiveActor->GetActorLocation() - AIPawn->GetActorLocation()).Size();
        if (CurrentDistance < NearestDistance)
        {
            NearestDistance = CurrentDistance;
            NearestActor = PerceiveActor;
        }
    }

    return NearestActor;
}

void USHGAIPerceptionComponent::CheckForEnemyLost(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    const auto StimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, UpdateInfo.Stimulus);
    if (StimulusClass != UAISense_Prediction::StaticClass() || UpdateInfo.Stimulus.SensingSucceeded)
        return;

    const auto AIController = Cast<AAIController>(GetOwner());
    const auto Blackboard = AIController->GetBlackboardComponent();
    if (!AIController || !Blackboard)
        return;

    if (const auto NearestActor = GetNearestActor())    // If we have another nearest actor, then focus on it.
    {
        Blackboard->SetValueAsObject(EnemyKeyName, NearestActor);
    }
    else
    {
        Blackboard->ClearValue(EnemyKeyName);

        if (UpdateInfo.Target.IsValid())
        {
            Blackboard->SetValueAsObject(LostEnemyKeyName, UpdateInfo.Target.Get());
            Blackboard->SetValueAsVector(NewLocationKeyName, UpdateInfo.Stimulus.StimulusLocation);
        }
    }
}
