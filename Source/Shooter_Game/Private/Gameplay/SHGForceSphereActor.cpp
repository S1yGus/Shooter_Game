// Shooter_Game, All rights reserved.

#include "Gameplay/SHGForceSphereActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Components/SHGMovementComponent.h"

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
    Super::Tick(DeltaSeconds);

    for (const auto& [Actor, ComponentsArray] : OverlapComponentsMap)
    {
        if (!IsValid(Actor))
        {
            continue;
        }

        const auto DistanceMultiplier = FMath::GetMappedRangeValueClamped(TRange<float>{0.0f, ForceField->GetScaledSphereRadius()},    //
                                                                          TRange<float>{MinForcePowerMultiplier, 1.0f},                //
                                                                          static_cast<float>(FVector::Distance(GetActorLocation(), Actor->GetActorLocation())));

        if (const auto Character = Cast<ACharacter>(Actor))
        {
            if (const auto CharacterMovement = Character->GetCharacterMovement())
            {
                CharacterMovement->AddRadialForce(GetActorLocation(),                                //
                                                  ForceField->GetScaledSphereRadius(),               //
                                                  ForcePower * DeltaSeconds * DistanceMultiplier,    //
                                                  ERadialImpulseFalloff::RIF_Linear);
            }
        }

        for (const auto Component : ComponentsArray)
        {
            if (Component->IsSimulatingPhysics())
            {
                Component->AddRadialForce(GetActorLocation(),                                //
                                          ForceField->GetScaledSphereRadius(),               //
                                          ForcePower * DeltaSeconds * DistanceMultiplier,    //
                                          ERadialImpulseFalloff::RIF_Linear);
            }
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