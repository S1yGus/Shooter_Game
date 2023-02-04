// Shooter_Game, All rights reserved.

#include "Gameplay/SHGJumpPadActor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"

ASHGJumpPadActor::ASHGJumpPadActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    SetRootComponent(BoxComponent);

    ArrowComponent = CreateDefaultSubobject<UArrowComponent>("LaunchDirection");
    ArrowComponent->SetupAttachment(BoxComponent);
}

void ASHGJumpPadActor::BeginPlay()
{
    Super::BeginPlay();

    check(BoxComponent);
    check(ArrowComponent);

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void ASHGJumpPadActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                               bool bFromSweep, const FHitResult& SweepResult)
{
    if (const auto Character = Cast<ACharacter>(OtherActor))
    {
        Character->LaunchCharacter(ArrowComponent->GetForwardVector() * ImpulseMultiplier, true, true);
    }
    else if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddVelocityChangeImpulseAtLocation(ArrowComponent->GetForwardVector() * ImpulseMultiplier, GetActorLocation());
    }
}
