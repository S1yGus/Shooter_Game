// Shooter_Game, All rights reserved.

#include "AI/SHGAIController.h"
#include "AI/SHGAICharacter.h"
#include "Components/ShooterAIPerceptionComponent.h"
#include "Components/SHGRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Prediction.h"

ASHGAIController::ASHGAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<USHGAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*AIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<USHGRespawnComponent>("RespawnComponent");

    bWantsPlayerState = true;
    bAllowStrafe = true;
}

void ASHGAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto FocusOnActor = GetFocusOnActor();
    SetFocus(FocusOnActor);

    UAISense_Prediction::RequestControllerPredictionEvent(this, FocusOnActor, PredictionTime);
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
        AICharacter->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeDamage);

        RunBehaviorTree(AICharacter->GetBehaviorTree());
    }
}

void ASHGAIController::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto BlackboardComponent = GetBlackboardComponent();
    if (!BlackboardComponent || GetFocusOnActor() || !InstigatedBy)
        return;

    BlackboardComponent->SetValueAsObject(EnemyBlackboardKeyName, InstigatedBy->GetPawn());
}

AActor* ASHGAIController::GetFocusOnActor()
{
    if (const auto BlackboardComponent = GetBlackboardComponent())
    {
        return Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyBlackboardKeyName));
    }

    return nullptr;
}
