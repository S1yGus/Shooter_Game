// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "SHGAIController.generated.h"

class USHGAIPerceptionComponent;
class USHGRespawnComponent;

UCLASS()
class SHOOTER_GAME_API ASHGAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASHGAIController();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGRespawnComponent* RespawnComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName EnemyBlackboardKeyName{"Enemy"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName TargetPickupBlackboardKeyName{"TargetPickup"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName InspectionStateBlackboardKeyName{"bInspectionState"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", Meta = (Units = "s"))
    float PredictionTime = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SprintProbability = 0.75f;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

private:
    FOnBlackboardChangeNotification OnEnemyKeyChanged;
    FOnBlackboardChangeNotification OnTargetPickupKeyChanged;

    FWeakObjectPtr FocusOnActor;

    void InterruptInspectionState();
    void RequestPrediction();
    void FocusOnPickup();
    void RestoreFocus();

    UFUNCTION()
    void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
