// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGRecoilComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGRecoilComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGRecoilComponent();

    void MakeRecoil();
    void StopRecoilRecoverTimer();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MinPitchRecoilMagnitude = 2.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MaxPitchRecoilMagnitude = 2.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MinYawRecoilMagnitude = -0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MaxYawRecoilMagnitude = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil", Meta = (Units = "s"))
    float RecoilTime = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float RecoilRecoverScale = 4.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil", Meta = (Units = "cm"))
    float RecoilRecoverPitchAdditionalOffset = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil", Meta = (Units = "cm"))
    float RecoilRecoverYawAdditionalOffset = 2.0f;

private:
    FTimerHandle RecoilTimerHandle;
    FTimerHandle RecoilRecoveryTimerHandle;
    FRotator InitialControllerInputRotation;
    float CurrentRecoilTime = 0.0f;
    float CurrentPitchRecoil = 0.0f;
    float CurrentYawRecoil = 0.0f;
    float CurrentRecoveryPitchRecoil = 0.0f;
    float CurrentRecoveryYawRecoil = 0.0f;

    inline APawn* GetOwnerPawn();
    bool CalculateRecoil();

    void OnRecoilUpdate();
    void OnRecoilRecoveryUpdate();
};
