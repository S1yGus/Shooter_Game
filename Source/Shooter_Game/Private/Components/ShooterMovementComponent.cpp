// Shooter_Game, All rights reserved.

#include "Components/ShooterMovementComponent.h"
#include "Player/ShooterBaseCharacter.h"

float USHGMovementComponent::GetMaxSpeed() const
{
    const auto OwnerCharacter = GetOwner<AShooterBaseCharacter>();
    if (!OwnerCharacter)
        return Super::GetMaxSpeed();

    return OwnerCharacter->IsSprinting() ? Super::GetMaxSpeed() * SprintSpeedMultiplier : Super::GetMaxSpeed();
}
