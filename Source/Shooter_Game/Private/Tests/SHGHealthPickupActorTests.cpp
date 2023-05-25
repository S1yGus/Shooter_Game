// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGHealthPickupActorTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Pickups/SHGHealthPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/SHGHealthComponent.h"
#include "Player/SHGPlayerCharacter.h"

using namespace Tests;

static const char* EmptyTestLevellName{"/Game/Tests/EmptyTestLevel"};
static const char* HealthPickupActorBlueprintName{"Blueprint'/Game/Gameplay/Pickups/BP_SHGHealthPickupActor.BP_SHGHealthPickupActor'"};
static const char* HealthPickupActorTestableBlueprintName{"Blueprint'/Game/Tests/BP_SHGHealthPickupActorTestable.BP_SHGHealthPickupActorTestable'"};
static const char* PawnWithoutHealthComponentBlueprintName{"Blueprint'/Game/Tests/BP_SimpleTestPawn.BP_SimpleTestPawn'"};
static const char* CharacterTestableBlueprintName{"Blueprint'/Game/Tests/BP_SHGPlayerCharacterTestable.BP_SHGPlayerCharacterTestable'"};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthPickupCppActorCantBeCreated, "Shooter_Game.Pickups.HealthPickup.CppActorCantBeCreated",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthPickupBlueprintShouldBeSetupCorrectly, "Shooter_Game.Pickups.HealthPickup.BlueprintShouldBeSetupCorrectly",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthPickupTryToGivePickupToOverrideFunctionTest, "Shooter_Game.Pickups.HealthPickup.TryToGivePickupToOverrideFunctionTest",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FHealthPickupCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FString ErrorMsg = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ASHGHealthPickupActor::StaticClass()->GetName());
    AddExpectedError(ErrorMsg, EAutomationExpectedErrorFlags::Exact);

    const auto HealthPickup = World->SpawnActor<ASHGHealthPickupActor>(ASHGHealthPickupActor::StaticClass(), FTransform::Identity);
    if (!TestNull("HealthPickup should not exists.", HealthPickup))
        return false;

    return true;
}

bool FHealthPickupBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const auto HealthPickup = SpawnBlueprint<ASHGHealthPickupActor>(World, HealthPickupActorBlueprintName);
    if (!TestNotNull("HealthPickup must exists.", HealthPickup))
        return false;

    const auto SphereComponent = HealthPickup->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("SphereComponent must exists.", SphereComponent))
        return false;

    TestTrueExpr(SphereComponent->GetScaledSphereRadius() > 0.0f);
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    ForEach<ECollisionChannel>(
        [&](ECollisionChannel Elem)
        {
            if (Elem != ECollisionChannel::ECC_OverlapAll_Deprecated)
            {
                TestTrueExpr(SphereComponent->GetCollisionResponseToChannel(Elem) == ECollisionResponse::ECR_Overlap);
            }
        });

    TestTrueExpr(HealthPickup->GetRootComponent() == SphereComponent);
    TestTrueExpr(HealthPickup->PrimaryActorTick.bCanEverTick);

    return true;
}

bool FHealthPickupTryToGivePickupToOverrideFunctionTest::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FTransform InitTransform{FVector{500.0, 500.0, 0.0}};
    const auto HealthPickup = SpawnBlueprint<ASHGHealthPickupActor>(World, HealthPickupActorTestableBlueprintName, InitTransform);
    if (!TestNotNull("HealthPickup must exists.", HealthPickup))
        return false;

    const float HealthAmount = 10.0f;
    const bool bRespawnable = false;
    const float RespawnTime = 0.0f;
    CallFunctionByNameWithParameters(HealthPickup,                             //
                                     "SetPickupData",                          //
                                     {FString::SanitizeFloat(HealthAmount),    //
                                      bRespawnable ? "Yes" : "No",             //
                                      FString::SanitizeFloat(RespawnTime)});

    AddInfo("An attempt to heal a pawn without health component.");

    const auto PawnWithoutHealthComponent = SpawnBlueprint<APawn>(World, PawnWithoutHealthComponentBlueprintName, HealthPickup->GetActorTransform());
    if (!TestNotNull("PawnWithoutHealthComponent must exists.", PawnWithoutHealthComponent))
        return false;

    if (!TestTrue("HealthPickup should not be picked up!", IsValid(HealthPickup)))
        return false;

    AddInfo("An attempt to heal a dead character.");

    const auto DeadCharacter = SpawnBlueprintDeferred<ASHGPlayerCharacter>(World, CharacterTestableBlueprintName);
    if (!TestNotNull("DeadCharacter must exists.", DeadCharacter))
        return false;

    const auto MaxHealth = 100.0f;
    const bool bAutoHeal = false;
    const auto AutoHealThreshold = 0.0f;
    const auto AutoHealRate = 0.0f;
    const auto AutoHealDelay = 0.0f;
    const auto AutoHealModifier = 0.0f;
    CallFunctionByNameWithParameters(DeadCharacter,                                 //
                                     "SetHealthData",                               //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      bAutoHeal ? "Yes" : "No",                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});
    DeadCharacter->FinishSpawning(FTransform::Identity);

    DeadCharacter->TakeDamage(MaxHealth, {}, nullptr, nullptr);
    DeadCharacter->SetActorLocation(HealthPickup->GetActorLocation());

    if (!TestTrue("HealthPickup should not be picked up!", IsValid(HealthPickup)))
        return false;

    AddInfo("An attempt to heal a wounded character.");

    const auto WoundedCharacter = SpawnBlueprintDeferred<ASHGPlayerCharacter>(World, CharacterTestableBlueprintName);
    if (!TestNotNull("WoundedCharacter must exists.", WoundedCharacter))
        return false;

    CallFunctionByNameWithParameters(WoundedCharacter,                              //
                                     "SetHealthData",                               //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      bAutoHeal ? "Yes" : "No",                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});
    WoundedCharacter->FinishSpawning(FTransform::Identity);

    const auto HealthComponent = WoundedCharacter->FindComponentByClass<USHGHealthComponent>();
    if (!TestNotNull("HealthComponent must exists.", HealthComponent))
        return false;

    const float DamageAmount = 20.0f;
    WoundedCharacter->TakeDamage(DamageAmount, {}, nullptr, nullptr);

    TestTrueExpr(HealthComponent->GetHealth() == MaxHealth - DamageAmount);

    WoundedCharacter->SetActorLocation(HealthPickup->GetActorLocation());

    TestTrueExpr(HealthComponent->GetHealth() == MaxHealth - DamageAmount + HealthAmount);
    if (!TestTrue("HealthPickup should be picked up!", !IsValid(HealthPickup)))
        return false;

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
