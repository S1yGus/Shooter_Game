// Shooter_Game, All rights reserved.

#include "UI/ShooterImpactIndicatorActor.h"
#include "Components/WidgetComponent.h"
#include "UI/ShooterImpactIndicatorUserWidget.h"
#include "ShooterUtils.h"

AShooterImpactIndicatorActor::AShooterImpactIndicatorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    ImpactWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ImpactWidget");
    ImpactWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    ImpactWidgetComponent->SetDrawAtDesiredSize(true);
    SetRootComponent(ImpactWidgetComponent);
}

void AShooterImpactIndicatorActor::SetImpactColor(UPhysicalMaterial* PhysicalMaterial)
{
    if (ImpactColorsMap.Contains(PhysicalMaterial))
    {
        ImpactColor = ImpactColorsMap[PhysicalMaterial];
    }
}

void AShooterImpactIndicatorActor::BeginPlay()
{
    Super::BeginPlay();
    
    SetLifeSpan(LifeSpawn);

    if (!ImpactWidgetComponent)
        return;

    const auto ImpactIndicatorWidget = Cast<UShooterImpactIndicatorUserWidget>(ImpactWidgetComponent->GetUserWidgetObject());
    ImpactIndicatorWidget->SetImpactWidgetText(ShooterUtils::TextFromInt(DamageAmount));
    ImpactIndicatorWidget->SetImpactWidgetTextColor(ImpactColor);
    ImpactIndicatorWidget->ShowStartupAnimation();
}
