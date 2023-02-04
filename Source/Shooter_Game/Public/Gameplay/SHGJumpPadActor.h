// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGJumpPadActor.generated.h"

class UBoxComponent;
class UArrowComponent;

UCLASS()
class SHOOTER_GAME_API ASHGJumpPadActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGJumpPadActor();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UArrowComponent* ArrowComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float ImpulseMultiplier = 1.0f;

    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult);
};
