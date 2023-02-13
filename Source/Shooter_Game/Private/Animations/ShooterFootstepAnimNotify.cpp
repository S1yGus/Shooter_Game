// Shooter_Game, All rights reserved.

#include "Animations/ShooterFootstepAnimNotify.h"
#include "Player/SHGBaseCharacter.h"
#include "Components/SHGBaseFXComponent.h"

void UShooterFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    const auto Character = Cast<ASHGBaseCharacter>(MeshComp->GetOwner());
    if (!Character)
        return;

    const auto FXComponent = Character->FindComponentByClass<USHGBaseFXComponent>();
    if (!FXComponent)
        return;

    FXComponent->MakeFootstepsFX(FootstepNotifyData);
}
