#pragma once

#include "Player/ShooterPlayerState.h"

class ShooterUtils
{
public:
    template <class T>
    static T* GetShooterPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    static bool AreEnemy(AController* Controller1, AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2)
            return false;

        const auto ShooterPlayerState1 = Cast<AShooterPlayerState>(Controller1->PlayerState);
        const auto ShooterPlayerState2 = Cast<AShooterPlayerState>(Controller2->PlayerState);

        return ShooterPlayerState1 && ShooterPlayerState2 && ShooterPlayerState1->GetTeamID() != ShooterPlayerState2->GetTeamID();
    }
};