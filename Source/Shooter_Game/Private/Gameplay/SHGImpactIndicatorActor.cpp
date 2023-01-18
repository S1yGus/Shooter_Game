// Shooter_Game, All rights reserved.

#include "Gameplay/SHGImpactIndicatorActor.h"
#include "Components/WidgetComponent.h"
#include "UI/Gameplay/SHGImpactIndicatorUserWidget.h"
#include "SHGUtils.h"

ASHGImpactIndicatorActor::ASHGImpactIndicatorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    ImpactWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ImpactWidget");
    ImpactWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    ImpactWidgetComponent->SetDrawAtDesiredSize(true);
    SetRootComponent(ImpactWidgetComponent);
}

void ASHGImpactIndicatorActor::SetImpactColor(UPhysicalMaterial* PhysicalMaterial)
{
    if (ImpactColorsMap.Contains(PhysicalMaterial))
    {
        ImpactColor = ImpactColorsMap[PhysicalMaterial];
    }
}

void ASHGImpactIndicatorActor::BeginPlay()
{
    Super::BeginPlay();

    check(ImpactWidgetComponent);

    const auto ImpactIndicatorWidget = Cast<USHGImpactIndicatorUserWidget>(ImpactWidgetComponent->GetUserWidgetObject());
    check(ImpactIndicatorWidget);
    ImpactIndicatorWidget->SetImpactWidgetText(SHGUtils::TextFromInt(DamageAmount));
    ImpactIndicatorWidget->SetImpactWidgetTextColor(ImpactColor);
    ImpactIndicatorWidget->ShowStartupAnimation();

    SetLifeSpan(LifeSpan);
}
