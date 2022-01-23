// Shooter_Game, All rights reserved.

#include "AI/Services/ShooterChangeWeaponService.h"
#include "AIController.h"
#include "Components/ShooterAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UShooterChangeWeaponService::UShooterChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void UShooterChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (!UKismetMathLibrary::RandomBoolWithWeight(RandomWeight))
        return;

    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController || !Blackboard)
        return;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return;

    const auto AIWeaponComponent = AIPawn->FindComponentByClass<UShooterAIWeaponComponent>();
    if (!AIWeaponComponent)
        return;

    const auto Enemy = Cast<AActor>(Blackboard->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName));
    if (!Enemy)
        return;

    float DistanceBetween = FindDistanceBetween(AIPawn, Enemy);
    if (DistanceBetween <= CloseCombatDistance)
    {
        // close combat
        if (ChooseWeaponInRange(AIWeaponComponent, 0, static_cast<int32>(LastCloseCombatType)))
            return;
    }

    // range combat
    if (ChooseWeaponInRange(AIWeaponComponent, static_cast<int32>(LastCloseCombatType) + 1, static_cast<int32>(EWeaponType::Max) - 1))
        return;

    UE_LOG(LogTemp, Error, TEXT("Error_1"))
    AIWeaponComponent->NextWeapon();
}

bool UShooterChangeWeaponService::ChooseWeaponInRange(UShooterAIWeaponComponent* AIWeaponComponent, int32 FirstIndex, int32 LastIndex)
{
    for (int32 WeaponIndex = FirstIndex; WeaponIndex < LastIndex + 1; ++WeaponIndex)
    {
        if (AIWeaponComponent->CheckWeaponAmmo(static_cast<EWeaponType>(WeaponIndex)))
        {
            if (AIWeaponComponent->CheckCurrentWeapon(static_cast<EWeaponType>(WeaponIndex)))
            {
                return true;
            }
            else
            {
                AIWeaponComponent->EquipWeapon(static_cast<EWeaponType>(WeaponIndex));
                return true;
            }
        }
    }

    return false;
}

float UShooterChangeWeaponService::FindDistanceBetween(AActor* FirstActor, AActor* SecondActor)
{
    return (FirstActor->GetActorLocation() - SecondActor->GetActorLocation()).Size();
}
