// Shooter_Game, All rights reserved.

#include "Components/SHGMovementComponent.h"
#include "Player/ShooterBaseCharacter.h"

float USHGMovementComponent::GetMaxSpeed() const
{
    const auto OwnerCharacter = GetOwner<AShooterBaseCharacter>();
    if (!OwnerCharacter)
        return Super::GetMaxSpeed();

    return OwnerCharacter->IsSprinting() ? Super::GetMaxSpeed() * SprintSpeedMultiplier : Super::GetMaxSpeed();
}

void USHGMovementComponent::InForceField(bool bCondition)
{
    if (!DefaultGroundFriction)
    {
        DefaultGroundFriction = GroundFriction;
    }

    GroundFriction = bCondition ? GroundFrictionInForceField : DefaultGroundFriction;
}
