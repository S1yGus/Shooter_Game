// Shooter_Game, All rights reserved.

#include "AI/Services/SHGChangeWeaponService.h"
#include "AIController.h"
#include "Components/SHGAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/SHGBaseWeaponActor.h"

USHGChangeWeaponService::USHGChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USHGChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (!UKismetMathLibrary::RandomBoolWithWeight(Probability))
        return;

    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController || !BlackboardComponent)
        return;

    const auto Enemy = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName));
    if (!Enemy)
        return;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return;

    const auto AIWeaponComponent = AIPawn->FindComponentByClass<USHGAIWeaponComponent>();
    if (!AIWeaponComponent)
        return;

    ASHGBaseWeaponActor* BestWeapon = nullptr;
    const auto DistanceBetween = FindDistanceBetween(AIPawn, Enemy);
    for (const auto& [WeaponType, Weapon] : AIWeaponComponent->GetWeaponsMap())
    {
        if (!Weapon->IsAmmoEmpty())
        {
            if (DistanceBetween < Weapon->GetMaxAttackDistance() && DistanceBetween > Weapon->GetMinAttackDistance())
            {
                if (!BestWeapon || BestWeapon->GetPriority() < Weapon->GetPriority())
                {
                    BestWeapon = Weapon;
                }
            }
        }
    }

    if (BestWeapon)
    {
        AIWeaponComponent->EquipWeapon(BestWeapon->GetWeaponData().WeaponType);
    }
    else
    {
        AIWeaponComponent->NextWeapon();
    }
}

float USHGChangeWeaponService::FindDistanceBetween(AActor* FirstActor, AActor* SecondActor)
{
    return (FirstActor->GetActorLocation() - SecondActor->GetActorLocation()).Size();
}
