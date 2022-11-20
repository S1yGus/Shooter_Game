// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SHGBaseFXComponent.h"
#include "SHGPlayerFXComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGPlayerFXComponent : public USHGBaseFXComponent
{
    GENERATED_BODY()

public:
    USHGPlayerFXComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake")
    TSubclassOf<UCameraShakeBase> GetDamageCameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake")
    FVector2D CameraShakeDamageRange{0.0f, 100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake")
    TSubclassOf<UCameraShakeBase> LocomotionCameraShakeClass;

    virtual void BeginPlay() override;

private:
    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
