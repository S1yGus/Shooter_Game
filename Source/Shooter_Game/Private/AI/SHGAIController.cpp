// Shooter_Game, All rights reserved.

#include "AI/SHGAIController.h"
#include "AI/SHGAICharacter.h"
#include "Components/SHGAIPerceptionComponent.h"
#include "Components/SHGRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Perception/AISense_Prediction.h"

constexpr static float SensePredictionUpdateTimerRate = 0.1f;

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

    FTimerHandle SensePredictionUpdateTimerHandle;
    GetWorldTimerManager().SetTimer(SensePredictionUpdateTimerHandle, this, &ThisClass::OnSensePredictionUpdate, SensePredictionUpdateTimerRate, true);
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
                    FocusOnActor = Cast<AActor>(BlackboardComponent.GetValue<UBlackboardKeyType_Object>(KeyID));
                    SetFocus(FocusOnActor);

                    return EBlackboardNotificationResult::ContinueObserving;
                });

            BlackboardComponent->RegisterObserver(BlackboardComponent->GetKeyID(EnemyBlackboardKeyName), this, OnEnemyKeyChanged);
        }
    }
}

void ASHGAIController::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto BlackboardComponent = GetBlackboardComponent();
    if (!BlackboardComponent || BlackboardComponent->GetValueAsObject(EnemyBlackboardKeyName) || !InstigatedBy)
        return;

    BlackboardComponent->SetValueAsObject(EnemyBlackboardKeyName, InstigatedBy->GetPawn());
}

void ASHGAIController::OnSensePredictionUpdate()
{
    UAISense_Prediction::RequestControllerPredictionEvent(this, FocusOnActor, PredictionTime);
}
