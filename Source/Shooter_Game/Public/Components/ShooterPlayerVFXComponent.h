// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SHGBaseFXComponent.h"
#include "ShooterPlayerVFXComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGPlayerVFXComponent : public USHGBaseFXComponent
{
    GENERATED_BODY()

public:
    USHGPlayerVFXComponent();

    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> GetDamageCameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> LocomotionCameraShakeClass;

    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
