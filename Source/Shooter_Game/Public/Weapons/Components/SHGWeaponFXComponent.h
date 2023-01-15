// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGFXCoreTypes.h"
#include "SHGWeaponFXComponent.generated.h"

class USoundCue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGWeaponFXComponent();

    virtual void BeginPlay() override;

    void MakeFireFX(USceneComponent* SceneComponent, const FName& SocketName, bool bAlternative);

    void PlayNoAmmoSound(USceneComponent* SceneComponent, const FName& SocketName);
    void PlaySwitchModeSound(USceneComponent* SceneComponent, const FName& SocketName);
    void PlaySwitchFlashlightSound(USceneComponent* SceneComponent, const FName& SocketName);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FWeaponFXData MainWeaponFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FWeaponFXData AlternativeWeaponFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* NoAmmoSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* SwitchModeSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* SwitchFlashlight;

private:
    void MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale = 1.0f);
};
