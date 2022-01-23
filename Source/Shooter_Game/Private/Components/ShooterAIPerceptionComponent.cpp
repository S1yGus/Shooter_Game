// Shooter_Game, All rights reserved.

#include "Components/ShooterAIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Prediction.h"
#include "Components/ShooterHealthComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UShooterAIPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();

    OnTargetPerceptionInfoUpdated.AddDynamic(this, &UShooterAIPerceptionComponent::CheckForEnemyLost);
}

AActor* UShooterAIPerceptionComponent::GetNearestActor()
{
    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if (PerceivedActors.Num() == 0)
        return nullptr;

    const auto AIController = Cast<AAIController>(GetOwner());
    if (!AIController)
        return nullptr;

    const auto Pawn = AIController->GetPawn();
    if (!Pawn)
        return nullptr;

    float NearestDistance = MAX_FLT;
    AActor* NearestActor = nullptr;
    for (const auto PerceivedActor : PerceivedActors)
    {
        const auto HealthComponent = PerceivedActor->FindComponentByClass<UShooterHealthComponent>();
        if (!HealthComponent || HealthComponent->IsDead())
            return nullptr;

        float CurrentDistance = (PerceivedActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
        if (CurrentDistance < NearestDistance)
        {
            NearestDistance = CurrentDistance;
            NearestActor = PerceivedActor;
        }
    }

    return NearestActor;
}

void UShooterAIPerceptionComponent::CheckForEnemyLost(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    const auto StimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), UpdateInfo.Stimulus);
    if (UpdateInfo.Stimulus.SensingSucceeded || StimulusClass != UAISense_Prediction::StaticClass())
        return;

    const auto AIController = Cast<AAIController>(GetOwner());
    const auto Blackboard = AIController->GetBlackboardComponent();

    const auto NearestActor = GetNearestActor();
    if (NearestActor)
    {
        Blackboard->SetValueAsObject(EnemyKeyName, NearestActor);
        return;
    }
    else
    {
        Blackboard->ClearValue(EnemyKeyName);
    }

    if (!AIController || !Blackboard)
        return;

    if (!UpdateInfo.Target.IsValid())
        return;

    Blackboard->SetValueAsObject(LostEnemyKeyName, UpdateInfo.Target.Get());
    Blackboard->SetValueAsVector(NewLocationKeyName, UpdateInfo.Stimulus.StimulusLocation);
}
