// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterImpactIndicatorActor.generated.h"

class UWidgetComponent;
class UPhysicalMaterial;

UCLASS()
class SHOOTER_GAME_API AShooterImpactIndicatorActor : public AActor
{
    GENERATED_BODY()

public:
    AShooterImpactIndicatorActor();

    void SetDamageAmount(float Damage) { DamageAmount = Damage; }
    void SetImpactColor(UPhysicalMaterial* PhysicalMaterial);

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* ImpactWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Impact")
    float LifeSpawn = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Impact")
    TMap<UPhysicalMaterial*, FLinearColor> ImpactColorsMap;

    virtual void BeginPlay() override;

private:
    float DamageAmount = 0.0f;
    FLinearColor ImpactColor = FLinearColor::Red;
};
