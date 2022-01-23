// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_AmmoType.generated.h"

class AShooterBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API UEnvQueryTest_AmmoType : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryTest_AmmoType(const FObjectInitializer& ObjectInitializer);

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<AShooterBaseWeaponActor> NeededAmmoClass;
};
