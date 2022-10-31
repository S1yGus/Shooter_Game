// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGImpactIndicatorActor.generated.h"

class UWidgetComponent;
class UPhysicalMaterial;

UCLASS()
class SHOOTER_GAME_API ASHGImpactIndicatorActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGImpactIndicatorActor();

    void SetDamageAmount(float Damage) { DamageAmount = Damage; }
    void SetImpactColor(UPhysicalMaterial* PhysicalMaterial);

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* ImpactWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Impact")
    float LifeSpan = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Impact")
    TMap<UPhysicalMaterial*, FLinearColor> ImpactColorsMap;

    virtual void BeginPlay() override;

private:
    float DamageAmount = 0.0f;
    FLinearColor ImpactColor{FLinearColor::Red};
};
