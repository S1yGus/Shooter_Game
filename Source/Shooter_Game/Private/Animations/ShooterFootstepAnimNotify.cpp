// Shooter_Game, All rights reserved.

#include "Animations/ShooterFootstepAnimNotify.h"
#include "Player/ShooterBaseCharacter.h"
#include "Components/ShooterBaseVFXComponent.h"

void UShooterFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    const auto Character = Cast<AShooterBaseCharacter>(MeshComp->GetOwner());
    if (!Character)
        return;

    const auto VFXComponent = Character->FindComponentByClass<UShooterBaseVFXComponent>();
    if (!VFXComponent)
        return;

    VFXComponent->MakeFootstepVFX(FootstepNotifyData);
}
