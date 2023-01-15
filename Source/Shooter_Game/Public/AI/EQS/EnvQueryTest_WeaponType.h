// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_WeaponType.generated.h"

class ASHGBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API UEnvQueryTest_WeaponType : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryTest_WeaponType(const FObjectInitializer& ObjectInitializer);

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<ASHGBaseWeaponActor> NeededWeaponClass;
};
