// Shooter_Game, All rights reserved.

#include "Gameplay/SHGForceSphereActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Components/SHGMovementComponent.h"

constexpr static float ForceTimerRate = 0.016f;

ASHGForceSphereActor::ASHGForceSphereActor()
{
    PrimaryActorTick.bCanEverTick = true;

    ForceField = CreateDefaultSubobject<USphereComponent>("ForceField");
    ForceField->SetSphereRadius(40.0f);
    ForceField->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ForceField->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(ForceField);
}

void ASHGForceSphereActor::Tick(float DeltaSeconds)
{
    for (const auto& [Actor, ComponentsArray] : OverlapComponentsMap)
    {
        if (!IsValid(Actor))
        {
            OverlapComponentsMap.Remove(Actor);
            continue;
        }

        if (const auto Character = Cast<ACharacter>(Actor))
        {
            if (const auto CharacterMovement = Character->GetCharacterMovement())
            {
                CharacterMovement->AddRadialForce(GetActorLocation(),                                        //
                                                  ForceField->GetScaledSphereRadius(),                       //
                                                  (bForceOut ? 1.0f : -1.0f) * ForcePower * DeltaSeconds,    //
                                                  ERadialImpulseFalloff::RIF_Linear);
            }
        }

        for (const auto Component : ComponentsArray)
        {
            if (!Component->IsSimulatingPhysics())
                continue;

            Component->AddRadialForce(GetActorLocation(),                                        //
                                      ForceField->GetScaledSphereRadius(),                       //
                                      (bForceOut ? 1.0f : -1.0f) * ForcePower * DeltaSeconds,    //
                                      ERadialImpulseFalloff::RIF_Linear);
        }
    }
}

void ASHGForceSphereActor::BeginPlay()
{
    Super::BeginPlay();

    check(ForceField);

    ForceField->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
    ForceField->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);
}

bool ASHGForceSphereActor::TryToSetConditionToInForceField(AActor* Actor, bool bCondition)
{
    if (const auto Character = Cast<ACharacter>(Actor))
    {
        if (const auto CharacterMovement = Character->GetCharacterMovement<USHGMovementComponent>())
        {
            CharacterMovement->InForceField(bCondition);

            return true;
        }
    }

    return false;
}

void ASHGForceSphereActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherComp)
    {
        TryToSetConditionToInForceField(OtherActor, true);

        auto& Components = OverlapComponentsMap.FindOrAdd(OtherActor);
        Components.Add(OtherComp);
    }
}

void ASHGForceSphereActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    TryToSetConditionToInForceField(OtherActor, false);

    OverlapComponentsMap.Remove(OtherActor);
}
