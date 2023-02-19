// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGFXCoreTypes.h"
#include "SHGExplodingObjectActor.generated.h"

class USHGHealthComponent;
class UNiagaraSystem;
class USoundCue;

UCLASS()
class SHOOTER_GAME_API ASHGExplodingObjectActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGExplodingObjectActor();

    UFUNCTION(BlueprintImplementableEvent)
    void OnExplosion();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D Damage{0.0f, 0.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", Meta = (ClampMin = "0"))
    float DamageRadius = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    bool bDoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECC_Visibility;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
    float ImpulsePower = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FImpactFXData DefaultFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TMap<UPhysicalMaterial*, FImpactFXData> FXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FExplosionCameraShakeData ExplosionCameraShakeData;

    virtual void BeginPlay() override;

private:
    void MakeImpactFX();

    void OnDeath(AController* KillerController, AController* VictimController);
};
