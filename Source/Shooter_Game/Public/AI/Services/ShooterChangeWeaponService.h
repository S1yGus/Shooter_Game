// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterCoreTypes.h"
#include "ShooterChangeWeaponService.generated.h"

class UShooterAIWeaponComponent;
class UEnvQuery;

UCLASS()
class SHOOTER_GAME_API UShooterChangeWeaponService : public UBTService
{
    GENERATED_BODY()

public:
    UShooterChangeWeaponService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RandomWeight = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyBlackboardKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float CloseCombatDistance = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EWeaponType LastCloseCombatType = EWeaponType::Shotgun;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    bool ChooseWeaponInRange(UShooterAIWeaponComponent* AIWeaponComponent, int32 FirstIndex, int32 LastIndex);

    float FindDistanceBetween(AActor* FirstActor, AActor* SecondActor);
};
