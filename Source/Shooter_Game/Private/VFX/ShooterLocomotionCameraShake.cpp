// Shooter_Game, All rights reserved.


#include "VFX/ShooterLocomotionCameraShake.h"
#include "Kismet/KismetMathLibrary.h"

float UShooterLocomotionCameraShake::GetPlayerVelocityNormalized() const
{
    const auto OwnerPawn = GetCameraManager()->GetOwningPlayerController()->GetPawn<APawn>();
    if (!OwnerPawn)
        return 0.0f;

    return UKismetMathLibrary::NormalizeToRange(OwnerPawn->GetVelocity().Size(), 0.0f, 1400.0f);
}