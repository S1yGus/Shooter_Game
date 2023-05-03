// Shooter_Game, All rights reserved.

#include "AI/SHGAIController.h"
#include "AI/SHGAICharacter.h"
#include "Components/SHGAIPerceptionComponent.h"
#include "Components/SHGRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Perception/AISense_Prediction.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/CrowdFollowingComponent.h"

ASHGAIController::ASHGAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
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
                [this](const UBlackboardComponent& BlackboardComponent, FBlackboard::FKey KeyID)
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

                    // If AI character see a target in the process of picking a pickup, then this is maybe not it's main target and FocusOnActor variable shouldn't be
                    // overwrite.
                    if (!BlackboardComponent.GetValueAsObject(TargetPickupBlackboardKeyName))
                    {
                        FocusOnActor = NewFocusOnActor;
                    }

                    return EBlackboardNotificationResult::ContinueObserving;
                });
            BlackboardComponent->RegisterObserver(BlackboardComponent->GetKeyID(EnemyBlackboardKeyName), this, OnEnemyKeyChanged);

            OnTargetPickupKeyChanged.BindLambda(
                [this](const UBlackboardComponent& BlackboardComponent, FBlackboard::FKey KeyID)
                {
                    if (BlackboardComponent.GetValue<UBlackboardKeyType_Object>(KeyID))
                    {
                        FocusOnPickup();
                    }
                    else
                    {
                        RestoreFocus();    // Focus on main target again after picing a pickup.
                    }

                    return EBlackboardNotificationResult::ContinueObserving;
                });
            BlackboardComponent->RegisterObserver(BlackboardComponent->GetKeyID(TargetPickupBlackboardKeyName), this, OnTargetPickupKeyChanged);
        }
    }
}

void ASHGAIController::OnUnPossess()
{
    Super::OnUnPossess();

    OnEnemyKeyChanged.Unbind();
    OnTargetPickupKeyChanged.Unbind();

    if (const auto BlackboardComponent = GetBlackboardComponent())
    {
        BlackboardComponent->UnregisterObserver(BlackboardComponent->GetKeyID(EnemyBlackboardKeyName), OnEnemyKeyChanged.GetHandle());
        BlackboardComponent->UnregisterObserver(BlackboardComponent->GetKeyID(TargetPickupBlackboardKeyName), OnTargetPickupKeyChanged.GetHandle());
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

void ASHGAIController::FocusOnPickup()
{
    // Clear the focus to turn in the direction of the pickup, but AI character can still focus on an enemy if it can see him after clear focus.
    ClearFocus(EAIFocusPriority::Gameplay);

    // Clear enemy blackboard key to stop firing.
    if (const auto BlackboardComponent = GetBlackboardComponent())
    {
        BlackboardComponent->ClearValue(EnemyBlackboardKeyName);
    }

    if (const auto AICharacter = GetPawn<ASHGAICharacter>(); AICharacter && UKismetMathLibrary::RandomBoolWithWeight(SprintProbability))
    {
        AICharacter->StartSprint();
    }
}

void ASHGAIController::RestoreFocus()
{
    if (const auto BlackboardComponent = GetBlackboardComponent())
    {
        BlackboardComponent->SetValueAsObject(EnemyBlackboardKeyName, FocusOnActor.Get());
    }

    if (const auto AICharacter = GetPawn<ASHGAICharacter>())
    {
        AICharacter->StopSprint();
    }
}

void ASHGAIController::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto BlackboardComponent = GetBlackboardComponent();
    if (!BlackboardComponent || BlackboardComponent->GetValueAsObject(EnemyBlackboardKeyName) || !InstigatedBy || InstigatedBy == this)
        return;

    BlackboardComponent->SetValueAsObject(EnemyBlackboardKeyName, InstigatedBy->GetPawn());
}
