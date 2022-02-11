// Shooter_Game, All rights reserved.

#include "AI/ShooterAIController.h"
#include "AI/ShooterAICharacter.h"
#include "Components/ShooterAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Prediction.h"
#include "Components/ShooterRespawnComponent.h"

AShooterAIController::AShooterAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UShooterAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*AIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>("RespawnComponent");

    bWantsPlayerState = true;
    bAllowStrafe = true;
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto FocusOnActor = GetFocusOnActor();
    SetFocus(FocusOnActor);

    // update Prediction Sense
    UAISense_Prediction::RequestControllerPredictionEvent(this, FocusOnActor, 0.2f);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AICharacter = Cast<AShooterAICharacter>(InPawn);
    if (!AICharacter)
        return;

    RunBehaviorTree(AICharacter->GetBehaviorTree());

    AICharacter->OnTakeAnyDamage.AddDynamic(this, &AShooterAIController::OnTakeDamage);
}

void AShooterAIController::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (GetFocusOnActor() || !InstigatedBy)
        return;

    if (!GetBlackboardComponent())
        return;
    
    GetBlackboardComponent()->SetValueAsObject(EnemyBlackboardKeyName, InstigatedBy->GetPawn());
}

AActor* AShooterAIController::GetFocusOnActor()
{
    if (!GetBlackboardComponent())
        return nullptr;

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(EnemyBlackboardKeyName));
}
