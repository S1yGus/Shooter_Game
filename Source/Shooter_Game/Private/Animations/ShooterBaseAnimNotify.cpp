// Shooter_Game, All rights reserved.


#include "Animations/ShooterBaseAnimNotify.h"

void UShooterBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    OnNotified.Broadcast(MeshComp);
}