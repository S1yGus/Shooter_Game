// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterVFXComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterVFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterVFXComponent();

    void MakeCameraShake(TSubclassOf<UMatineeCameraShake> CameraShakeClass, float Scale = 1.0f);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UMatineeCameraShake> GetDamageCameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UMatineeCameraShake> LocomotionCameraShakeClass;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    virtual void BeginPlay() override;
};
