// Shooter_Game, All rights reserved.

#include "AI/SHGAIController.h"
#include "AI/SHGAICharacter.h"
#include "Components/SHGAIPerceptionComponent.h"
#include "Components/SHGRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Perception/AISense_Prediction.h"

ASHGAIController::ASHGAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<USHGAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*AIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<USHGRespawnComponent>("RespawnComponent");

    bWantsPlayerState = true;
    bAllowStrafe = true;
}

void ASHGAIController::BeginPlay()
{
    Super::BeginPlay();

    check(AIPerceptionComponent);
    check(RespawnComponent);
}

void ASHGAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (const auto AICharacter = Cast<ASHGAICharacter>(InPawn))
    {
        RunBehaviorTree(AICharacter->GetBehaviorTree());

        AICharacter->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeDamage);

        if (const auto BlackboardComponent = GetBlackboardComponent())
        {
            OnEnemyKeyChanged.BindLambda(
                [&](const UBlackboardComponent& BlackboardComponent, FBlackboard::FKey KeyID)
                {
                    const auto NewFocusOnActor = Cast<AActor>(BlackboardComponent.GetValue<UBlackboardKeyType_Object>(KeyID));
                    if (!NewFocusOnActor)    // If lost sight of the focus on actor, then request prediction.
                    {
                        RequestPrediction();
                    }
                    else
                    {
                        InterruptInspectionState();
                    }

                    SetFocus(NewFocusOnActor);
                    FocusOnActor = NewFocusOnActor;

                    return EBlackboardNotificationResult::ContinueObserving;
                });

            BlackboardComponent->RegisterObserver(BlackboardComponent->GetKeyID(EnemyBlackboardKeyName), this, OnEnemyKeyChanged);
        }
    }
}

void ASHGAIController::InterruptInspectionState()
{
    if (const auto BlackboardComponent = GetBlackboardComponent(); BlackboardComponent && BlackboardComponent->GetValueAsBool(InspectionStateBlackboardKeyName))
    {
        BlackboardComponent->ClearValue(InspectionStateBlackboardKeyName);
    }
}

void ASHGAIController::RequestPrediction()
{
    if (FocusOnActor.IsValid())
    {
        UAISense_Prediction::RequestControllerPredictionEvent(this, Cast<AActor>(FocusOnActor.Get()), PredictionTime);
    }
}

void ASHGAIController::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto BlackboardComponent = GetBlackboardComponent();
    if (!BlackboardComponent || BlackboardComponent->GetValueAsObject(EnemyBlackboardKeyName) || !InstigatedBy)
        return;

    BlackboardComponent->SetValueAsObject(EnemyBlackboardKeyName, InstigatedBy->GetPawn());
}
