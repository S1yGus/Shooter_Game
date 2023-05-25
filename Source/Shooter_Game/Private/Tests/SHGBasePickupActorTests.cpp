// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGBasePickupActorTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Tests/SHGBasePickupActorTestable.h"
#include "Components/SphereComponent.h"

using namespace Tests;

static const char* EmptyTestLevellName{"/Game/Tests/EmptyTestLevel"};
static const char* SimpleActorBlueprintName{"Blueprint'/Game/Tests/BP_SimpleTestActor.BP_SimpleTestActor'"};
static const char* SimplePawnBlueprintName{"Blueprint'/Game/Tests/BP_SimpleTestPawn.BP_SimpleTestPawn'"};

DEFINE_LOG_CATEGORY_STATIC(BasePickupTestsLog, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBasePickupCppActorCantBeCreated, "Shooter_Game.Pickups.BasePickup.CppActorCantBeCreated",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBasePickupShouldNotBePpickedUpByNotPawn, "Shooter_Game.Pickups.BasePickup.BasePickupShouldNotBePpickedUpByNotPawn",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBasePickupShouldBePpickedUpByPawn, "Shooter_Game.Pickups.BasePickup.BasePickupShouldBePpickedUpByPawn",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBasePickupDeferredPickUpTest, "Shooter_Game.Pickups.BasePickup.DeferredPickUpTest",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBasePickupRespawnTest, "Shooter_Game.Pickups.BasePickup.RespawnTest",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBasePickupCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    FString ErrorMsg = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ASHGBasePickupActor::StaticClass()->GetName());
    AddExpectedError(ErrorMsg, EAutomationExpectedErrorFlags::Exact);

    const auto HealthPickup = World->SpawnActor<ASHGBasePickupActor>(ASHGBasePickupActor::StaticClass(), FTransform::Identity);
    if (!TestNull("HealthPickup should not exists.", HealthPickup))
        return false;

    return true;
}

bool FBasePickupShouldNotBePpickedUpByNotPawn::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FTransform InitTransform{FVector{500.0, 500.0, 0.0}};
    const auto BasePickup = World->SpawnActor<ASHGBasePickupActorTestable>(ASHGBasePickupActorTestable::StaticClass(), InitTransform);
    if (!TestNotNull("BasePickup must exists.", BasePickup))
        return false;

    const bool bReturnValue = true;
    const bool bRespawnable = false;
    BasePickup->SetPickupData(bReturnValue, bRespawnable);

    AddInfo("An attempt to take the pickup by not a pawn.");

    const auto SimpleActor = SpawnBlueprint<AActor>(World, SimpleActorBlueprintName, BasePickup->GetActorTransform());
    if (!TestNotNull("SimpleActor must exists.", SimpleActor))
        return false;

    TestTrueExpr(IsValid(BasePickup));

    return true;
}

bool FBasePickupShouldBePpickedUpByPawn::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FTransform InitTransform{FVector{500.0, 500.0, 0.0}};
    const auto BasePickup = World->SpawnActor<ASHGBasePickupActorTestable>(ASHGBasePickupActorTestable::StaticClass(), InitTransform);
    if (!TestNotNull("BasePickup must exists.", BasePickup))
        return false;

    const bool bReturnValue = true;
    const bool bRespawnable = false;
    BasePickup->SetPickupData(bReturnValue, bRespawnable);

    AddInfo("An attempt to take the pickup by a pawn.");

    const auto SimplePawn = SpawnBlueprint<APawn>(World, SimplePawnBlueprintName, BasePickup->GetActorTransform());
    if (!TestNotNull("SimplePawn must exists.", SimplePawn))
        return false;

    TestTrueExpr(!IsValid(BasePickup));

    return true;
}

bool FBasePickupDeferredPickUpTest::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    AddInfo("Creating the base pickup with temporary false return value of the function TryToGivePickupTo.");

    const FTransform InitTransform{FVector{500.0, 500.0, 0.0}};
    const auto BasePickup = World->SpawnActor<ASHGBasePickupActorTestable>(ASHGBasePickupActorTestable::StaticClass(), InitTransform);
    if (!TestNotNull("BasePickup must exists.", BasePickup))
        return false;

    bool bReturnValue = false;
    const bool bRespawnable = false;
    BasePickup->SetPickupData(bReturnValue, bRespawnable);

    AddInfo("An attempt to take the pickup by a pawn.");

    const auto SimplePawn = SpawnBlueprint<APawn>(World, SimplePawnBlueprintName, BasePickup->GetActorTransform());
    if (!TestNotNull("SimplePawn must exists.", SimplePawn))
        return false;

    if (!TestTrue("BasePickup should not be picked up!", IsValid(BasePickup)))
        return false;

    AddInfo("Switch to true return value of the function TryToGivePickupTo.");

    bReturnValue = true;
    BasePickup->SetPickupData(bReturnValue);

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            if (IsValid(BasePickup))
            {
                UE_LOG(BasePickupTestsLog, Error, TEXT("The base pickup must be taken!"));
            }
        },
        World->GetDeltaSeconds()));

    return true;
}

bool FBasePickupRespawnTest::RunTest(const FString& Parameters)
{
    const LevelScope Level(EmptyTestLevellName);

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    FTransform InitTransform{FVector{500.0, 500.0, 0.0}};
    const auto NotRespawnableBasePickup = World->SpawnActor<ASHGBasePickupActorTestable>(ASHGBasePickupActorTestable::StaticClass(), InitTransform);
    if (!TestNotNull("NotRespawnableBasePickup must exists.", NotRespawnableBasePickup))
        return false;

    bool bReturnValue = true;
    bool bRespawnable = false;
    NotRespawnableBasePickup->SetPickupData(bReturnValue, bRespawnable);

    AddInfo("Picking up the not respawnable base pickup.");

    auto SimplePawn = SpawnBlueprint<APawn>(World, SimplePawnBlueprintName, NotRespawnableBasePickup->GetActorTransform());
    if (!TestNotNull("SimplePawn must exists.", SimplePawn))
        return false;

    TestTrueExpr(!IsValid(NotRespawnableBasePickup));

    SimplePawn->Destroy();

    const auto RespawnableBasePickup = World->SpawnActor<ASHGBasePickupActorTestable>(ASHGBasePickupActorTestable::StaticClass(), InitTransform);
    if (!TestNotNull("RespawnableBasePickup must exists.", RespawnableBasePickup))
        return false;

    bRespawnable = true;
    const float RespawnTime = 2.0f;
    RespawnableBasePickup->SetPickupData(bReturnValue, bRespawnable, RespawnTime);

    const auto SphereComponent = RespawnableBasePickup->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("SphereComponent must exists.", SphereComponent))
        return false;

    AddInfo("Respawnable base pickup initial parameters.");

    TestTrueExpr(RespawnableBasePickup->CanBeTaken());
    TestTrueExpr(RespawnableBasePickup->GetRootComponent()->GetVisibleFlag());
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

    AddInfo("Picking up the respawnable base pickup.");

    SimplePawn = SpawnBlueprint<APawn>(World, SimplePawnBlueprintName, NotRespawnableBasePickup->GetActorTransform());
    if (!TestNotNull("SimplePawn must exists.", SimplePawn))
        return false;

    TestTrueExpr(!RespawnableBasePickup->CanBeTaken());
    TestTrueExpr(!RespawnableBasePickup->GetRootComponent()->GetVisibleFlag());
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    SimplePawn->Destroy();

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            if (!RespawnableBasePickup->CanBeTaken()                               //
                || !RespawnableBasePickup->GetRootComponent()->GetVisibleFlag()    //
                || SphereComponent->GetCollisionEnabled() != ECollisionEnabled::QueryOnly)
            {
                UE_LOG(BasePickupTestsLog, Error, TEXT("The base pickup should be respawned!"));
            }
        },
        RespawnTime));

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
