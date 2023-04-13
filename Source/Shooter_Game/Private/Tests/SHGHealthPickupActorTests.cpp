// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGHealthPickupActorTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/SHGTestsUtils.h"
#include "Pickups/SHGHealthPickupActor.h"
#include "Components/SphereComponent.h"
#include "Player/SHGPlayerCharacter.h"

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "Shooter_Game.Pickups.HealthPickup.CppActorCantBeCreated",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "Shooter_Game.Pickups.HealthPickup.BlueprintShouldBeSetupCorrectly",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTryToGivePickupToOverrideFunctionTest, "Shooter_Game.Pickups.HealthPickup.TryToGivePickupToOverrideFunctionTest",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    FString ErrorMsg = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ASHGHealthPickupActor::StaticClass()->GetName());
    AddExpectedError(ErrorMsg, EAutomationExpectedErrorFlags::Exact);

    const auto HealthPickup = World->SpawnActor<ASHGHealthPickupActor>(ASHGHealthPickupActor::StaticClass(), FTransform::Identity);
    if (!TestNull("HealthPickup should not exists.", HealthPickup))
        return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FString HealthPickupActorBlueprintName{"Blueprint'/Game/Gameplay/Pickups/BP_SHGHealthPickupActor.BP_SHGHealthPickupActor'"};
    const auto HealthPickup = SpawnBlueprint<ASHGHealthPickupActor>(World, HealthPickupActorBlueprintName);
    if (!TestNotNull("HealthPickup must exists.", HealthPickup))
        return false;

    const auto SphereComponent = HealthPickup->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("SphereComponent must exists.", SphereComponent))
        return false;

    TestTrueExpr(SphereComponent->GetScaledSphereRadius() > 0.0f);
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    ENUM_LOOP_START(ECollisionChannel, Elem)
    if (Elem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(SphereComponent->GetCollisionResponseToChannel(Elem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    TestTrueExpr(HealthPickup->GetRootComponent() == SphereComponent);
    TestTrueExpr(HealthPickup->PrimaryActorTick.bCanEverTick);

    return true;
}

bool FTryToGivePickupToOverrideFunctionTest::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    FTransform InitTransform{FVector{500.0, 500.0, 0.0}};
    const FString HealthPickupActorBlueprintName{"Blueprint'/Game/Gameplay/Pickups/BP_SHGHealthPickupActor.BP_SHGHealthPickupActor'"};
    const auto HealthPickup = SpawnBlueprint<ASHGHealthPickupActor>(World, HealthPickupActorBlueprintName, InitTransform);
    if (!TestNotNull("HealthPickup must exists.", HealthPickup))
        return false;

    const auto SphereComponent = HealthPickup->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("SphereComponent must exists.", SphereComponent))
        return false;

    AddInfo("An attempt to heal a pawn without health component.");

    TestTrueExpr(HealthPickup->CanBeTaken());
    TestTrueExpr(HealthPickup->GetRootComponent()->GetVisibleFlag());
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    const FString PawnWithoutHealthComponentBlueprintName{"Blueprint'/Game/Tests/BP_SimpleTestPawn.BP_SimpleTestPawn'"};
    const auto PawnWithoutHealthComponent = SpawnBlueprint<APawn>(World, PawnWithoutHealthComponentBlueprintName, HealthPickup->GetActorTransform());

    TestTrueExpr(HealthPickup->CanBeTaken());
    TestTrueExpr(HealthPickup->GetRootComponent()->GetVisibleFlag());
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    AddInfo("An attempt to heal a dead character.");

    const FString PlayerTestableBlueprintName{"Blueprint'/Game/Tests/BP_SHGPlayerCharacterTestable.BP_SHGPlayerCharacterTestable'"};
    const auto DeadCharacter = SpawnBlueprintDeferred<ASHGPlayerCharacter>(World, PlayerTestableBlueprintName);
    if (!TestNotNull("DeadCharacter must exists.", DeadCharacter))
        return false;

    const auto MaxHealth = 100.0f;
    const FString AutoHeal{"No"};
    const auto AutoHealThreshold = 100.0f;
    const auto AutoHealRate = 0.5f;
    const auto AutoHealDelay = 2.0f;
    const auto AutoHealModifier = 1.0f;
    CallFunctionByNameWithParameters(DeadCharacter,                                 //
                                     "SetHealthData",                               //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      AutoHeal,                                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});
    DeadCharacter->FinishSpawning(FTransform::Identity);

    DeadCharacter->TakeDamage(MaxHealth, {}, nullptr, nullptr);
    DeadCharacter->SetActorLocation(HealthPickup->GetActorLocation());

    TestTrueExpr(HealthPickup->CanBeTaken());
    TestTrueExpr(HealthPickup->GetRootComponent()->GetVisibleFlag());
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    AddInfo("An attempt to heal a wounded character.");

    const auto WoundedCharacter = SpawnBlueprintDeferred<ASHGPlayerCharacter>(World, PlayerTestableBlueprintName);
    if (!TestNotNull("WoundedCharacter must exists.", WoundedCharacter))
        return false;

    CallFunctionByNameWithParameters(WoundedCharacter,                              //
                                     "SetHealthData",                               //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      AutoHeal,                                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});
    WoundedCharacter->FinishSpawning(FTransform::Identity);

    WoundedCharacter->TakeDamage(MaxHealth - 1.0f, {}, nullptr, nullptr);
    WoundedCharacter->SetActorLocation(HealthPickup->GetActorLocation());

    TestTrueExpr(!HealthPickup->CanBeTaken());
    TestTrueExpr(!HealthPickup->GetRootComponent()->GetVisibleFlag());
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
