// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SHGBaseFXComponent.h"
#include "SHGPlayerFXComponent.generated.h"

class UCameraShakeBase;
class USHGLocomotionCameraShake;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake|Locomotion")
    bool bLocomotionCameraShakeEnabled = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake|Locomotion", Meta = (EditCondition = "bLocomotionCameraShakeEnabled"))
    TSubclassOf<USHGLocomotionCameraShake> LocomotionCameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake|Locomotion", Meta = (EditCondition = "bLocomotionCameraShakeEnabled"))
    FVector2D PlayerSpeedRange{0.0f, 1400.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|CameraShake|Locomotion", Meta = (EditCondition = "bLocomotionCameraShakeEnabled"))
    float AmplitudeMultiplierOnZoom = 0.1f;

    virtual void BeginPlay() override;

private:
    FTimerHandle LocomotionCameraShakeTimerHandle;
    USHGLocomotionCameraShake* LocomotionCameraShake;
    bool bZoom = false;

    inline APlayerCameraManager* GetPlayerCameraManager() const;
    inline float GetPlayerVelocityNormalized() const;

    void InitLocomotionCameraShake();
    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
    UFUNCTION()
    void OnPossess(APawn* Pawn, AController* OldController, AController* NewController);
    void OnUpdateLocomotionCameraShake();
};
