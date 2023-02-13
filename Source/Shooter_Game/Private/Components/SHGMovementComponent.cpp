// Shooter_Game, All rights reserved.

#include "Components/SHGMovementComponent.h"
#include "Player/SHGBaseCharacter.h"

float USHGMovementComponent::GetMaxSpeed() const
{
    if (const auto OwnerCharacter = GetOwner<ASHGBaseCharacter>())
    {
        if (OwnerCharacter->IsSprinting())
        {
            return Super::GetMaxSpeed() * SprintSpeedMultiplier;
        }
        else if (OwnerCharacter->IsZooming())
        {
            return Super::GetMaxSpeed() * ZoomSpeedMultiplier;
        }
    }

    return Super::GetMaxSpeed();
}

void USHGMovementComponent::InForceField(bool bCondition)
{
    if (!DefaultGroundFriction)
    {
        DefaultGroundFriction = GroundFriction;
    }

    GroundFriction = bCondition ? GroundFrictionInForceField : DefaultGroundFriction;
}
