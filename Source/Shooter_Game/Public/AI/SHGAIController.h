// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SHGAIController.generated.h"

class USHGAIPerceptionComponent;
class USHGRespawnComponent;

UCLASS()
class SHOOTER_GAME_API ASHGAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASHGAIController();

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGRespawnComponent* RespawnComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName EnemyBlackboardKeyName = "Enemy";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float PredictionTime = 0.2f;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

private:
    AActor* GetFocusOnActor();

    UFUNCTION()
    void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
