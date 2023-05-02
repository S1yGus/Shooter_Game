// Shooter_Game, All rights reserved.

#include "Components/SHGAIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Touch.h"
#include "Components/SHGHealthComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SHGUtils.h"

void USHGAIPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();

    OnTargetPerceptionInfoUpdated.AddDynamic(this, &ThisClass::HandleInspectionState);
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

void USHGAIPerceptionComponent::HandleInspectionState(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    const auto AIController = GetOwner<AAIController>();
    if (!AIController)
        return;

    const auto BlackboardComponent = AIController->GetBlackboardComponent();
    if (!BlackboardComponent)
        return;

    const auto SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, UpdateInfo.Stimulus);
    if ((SenseClass == UAISense_Hearing::StaticClass() && UpdateInfo.Target.Get() != AIController->GetPawn())    // If heard something,
        || SenseClass == UAISense_Prediction::StaticClass()                                                      // or lost sight of the enemy,
        || SenseClass == UAISense_Touch::StaticClass())                                                          // or felt the touch.
    {

        BlackboardComponent->ClearValue(InspectionStateBlackboardKeyName);    // Clear the inspection state key to interrupt the current inspection.
        BlackboardComponent->SetValueAsBool(InspectionStateBlackboardKeyName, true);
        BlackboardComponent->SetValueAsVector(LocationToInspectBlackboardKeyName, UpdateInfo.Stimulus.StimulusLocation);    // Set a new location to inspect.
    }
}
