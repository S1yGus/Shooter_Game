// Shooter_Game, All rights reserved.

#include "AI/SHGAICharacter.h"
#include "AI/SHGAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SHGAIWeaponComponent.h"
#include "Components/SHGHealthComponent.h"
#include "BrainComponent.h"
#include "UI/Gameplay/SHGAIHealthBarUserWidget.h"

constexpr static float HealthWidgetVisibilityUpdateTimerRate = 0.05f;

ASHGAICharacter::ASHGAICharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USHGAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASHGAIController::StaticClass();

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->RotationRate = FRotator{0.0f, 300.0f, 0.0f};

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    HealthWidgetComponent->SetDrawSize(FVector2D{100.0, 6.0});
    HealthWidgetComponent->SetDrawAtDesiredSize(false);
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
}

void ASHGAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);
    check(GetCharacterMovement());

    HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
    OnHealthChanged(HealthComponent->GetHealth(), HealthComponent->GetHealthPercent());    // To initialize the value of the health widget.

    FTimerHandle UpdateHealthWidgetVisibilityTimerHandle;
    GetWorldTimerManager().SetTimer(UpdateHealthWidgetVisibilityTimerHandle,    //
                                    this,                                       //
                                    &ThisClass::OnUpdateHealthWidget,           //
                                    HealthWidgetVisibilityUpdateTimerRate,      //
                                    true);
}

void ASHGAICharacter::OnHealthChanged(float Health, float HealthPercent)
{
    if (const auto HealthWidget = Cast<USHGAIHealthBarUserWidget>(HealthWidgetComponent->GetUserWidgetObject()))
    {
        HealthWidget->SetHealthPercent(HealthPercent);
    }
}

void ASHGAICharacter::OnUpdateHealthWidget()
{
    const auto PC = GetWorld()->GetFirstPlayerController();
    if (!PC || !PC->PlayerCameraManager)
        return;

    const auto PlayersPawn = PC->GetPawn();
    if (!PlayersPawn)
        return;

    const auto DistanceBetween = FVector::Distance(PlayersPawn->GetActorLocation(), GetActorLocation());
    const bool bVisible = DistanceBetween < HealthWidgetVisibilityDistance;

    if (HealthWidgetComponent->IsVisible() != bVisible)
    {
        HealthWidgetComponent->SetVisibility(bVisible);
    }

    if (bVisible)
    {
        auto NewRotation = PC->PlayerCameraManager->GetCameraRotation();
        NewRotation.Yaw += 180.0f;     // To flip to face the player.
        NewRotation.Pitch *= -1.0f;    // To invert pitch.
        HealthWidgetComponent->SetWorldRotation(NewRotation);
    }
}

void ASHGAICharacter::OnDeath(AController* KillerController, AController* VictimController)
{
    Super::OnDeath(KillerController, VictimController);

    if (const auto AIController = Cast<AAIController>(Controller); AIController && AIController->BrainComponent)
    {
        AIController->BrainComponent->Cleanup();
    }
}
