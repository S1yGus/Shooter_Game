// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SHGChangeWeaponService.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGChangeWeaponService : public UBTService
{
    GENERATED_BODY()

public:
    USHGChangeWeaponService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Probability = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyBlackboardKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    inline float FindDistanceBetween(AActor* FirstActor, AActor* SecondActor);
};
