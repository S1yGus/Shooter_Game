// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGHealthComponentTests.h"
#include "CoreMinimal.h"
#include "Tests/SHGTestsUtils.h"
#include "Misc/AutomationTest.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SHGHealthComponent.h"
#include "Player/ShooterPlayerCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentTests, All, All);

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponentBaseFunctional, "Shooter_Game.Components.HealthComponent.BaseFunctional",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponentAutoHeal, "Shooter_Game.Components.HealthComponent.AutoHeal",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponentDelegates, "Shooter_Game.Components.HealthComponent.Delegates",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FHealthComponentBaseFunctional::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    AddInfo("The health component base functional.");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FString PlayerTestableBlueprintName{"Blueprint'/Game/Tests/BP_ShooterPlayerCharacter_Testable.BP_ShooterPlayerCharacter_Testable'"};
    const auto Player = SpawnBlueprintDeferred<AShooterPlayerCharacter>(World, PlayerTestableBlueprintName);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    const auto HealthComponent = Player->FindComponentByClass<USHGHealthComponent>();
    if (!TestNotNull("HealthComponent must exists.", HealthComponent))
        return false;

    const auto FuncName{"SetHealthData"};
    const auto MaxHealth = 200.0f;
    const FString AutoHeal{"No"};
    const auto AutoHealThreshold = 100.0f;
    const auto AutoHealRate = 0.5f;
    const auto AutoHealDelay = 2.0f;
    const auto AutoHealModifier = 1.0f;
    CallFunctionByNameWithParameters(Player,                                        //
                                     FuncName,                                      //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      AutoHeal,                                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});

    Player->FinishSpawning(FTransform::Identity);

    AddInfo("Initial values. Completely healthy.");

    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f));

    AddInfo("TryToHeal() test. Player is completely healthy.");

    TestTrueExpr(!HealthComponent->TryToHeal(MaxHealth));

    AddInfo("TryToHeal() test. Negative parameter.");

    TestTrueExpr(!HealthComponent->TryToHeal(-1.0f));

    AddInfo("GetHealth() test.");
    AddInfo("GetHealthPercent() test.");

    const auto DamageAmount = 20.0f;
    Player->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth - DamageAmount));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f - DamageAmount / MaxHealth));

    AddInfo("TryToHeal() test.");

    TestTrueExpr(HealthComponent->TryToHeal(DamageAmount));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));

    AddInfo("TryToHeal() test. Owerheal.");

    Player->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestTrueExpr(HealthComponent->TryToHeal(MaxHealth + 1.0f));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));

    AddInfo("TryToHeal() test. Zero.");

    Player->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestTrueExpr(!HealthComponent->TryToHeal(0.0f));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth - DamageAmount));

    AddInfo("IsDead() test. Fasle.");

    TestTrueExpr(!HealthComponent->IsDead());

    AddInfo("IsDead() test. True.");

    Player->TakeDamage(MaxHealth + 1.0f, FDamageEvent{}, nullptr, nullptr);

    TestTrueExpr(HealthComponent->IsDead());

    AddInfo("TryToHeal() test. Dead.");

    TestTrueExpr(!HealthComponent->TryToHeal(MaxHealth));

    return true;
}

bool FHealthComponentAutoHeal::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    AddInfo("The health component auto heal tests.");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FString PlayerTestableBlueprintName{"Blueprint'/Game/Tests/BP_ShooterPlayerCharacter_Testable.BP_ShooterPlayerCharacter_Testable'"};
    const auto Player = SpawnBlueprintDeferred<AShooterPlayerCharacter>(World, PlayerTestableBlueprintName);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    const auto HealthComponent = Player->FindComponentByClass<USHGHealthComponent>();
    if (!TestNotNull("HealthComponent must exists.", HealthComponent))
        return false;

    const auto FuncName{"SetHealthData"};
    const auto MaxHealth = 200.0f;
    const FString AutoHeal{"Yes"};
    const auto AutoHealThreshold = 120.0f;
    const auto AutoHealRate = 0.1f;
    const auto AutoHealDelay = 2.0f;
    const auto AutoHealModifier = 1.0f;
    CallFunctionByNameWithParameters(Player,                                        //
                                     FuncName,                                      //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      AutoHeal,                                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});

    Player->FinishSpawning(FTransform::Identity);

    AddInfo("Initial values. Completely healthy.");

    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f));

    AddInfo("Auto heal test. Auto heal delay.");

    const auto DamageAmount = 100.0f;
    Player->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            if (!FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth - DamageAmount + AutoHealModifier))
            {
                UE_LOG(HealthComponentTests, Error, TEXT("Auto heal should be delayed!"));
            }
        },
        AutoHealDelay));

    AddInfo("Auto heal test.");

    const auto AutoHealCompletionDelay = (AutoHealThreshold - (MaxHealth - DamageAmount)) / AutoHealModifier * AutoHealRate;
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            if (!FMath::IsNearlyEqual(HealthComponent->GetHealth(), AutoHealThreshold))
            {
                UE_LOG(HealthComponentTests, Error, TEXT("Health should be equal AutoHealThreshold!"));
            }
        },
        AutoHealCompletionDelay));

    AddInfo("Auto heal test. No heal more than the threshold.");

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            if (!FMath::IsNearlyEqual(HealthComponent->GetHealth(), AutoHealThreshold))
            {
                UE_LOG(HealthComponentTests, Error, TEXT("Health should be equal AutoHealThreshold!"));
            }
        },
        AutoHealRate));

    return true;
}

bool FHealthComponentDelegates::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    AddInfo("The health component base functional.");

    const auto World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const FString PlayerTestableBlueprintName{"Blueprint'/Game/Tests/BP_ShooterPlayerCharacter_Testable.BP_ShooterPlayerCharacter_Testable'"};
    const auto Player = SpawnBlueprintDeferred<AShooterPlayerCharacter>(World, PlayerTestableBlueprintName);
    if (!TestNotNull("Player must exists.", Player))
        return false;

    const auto HealthComponent = Player->FindComponentByClass<USHGHealthComponent>();
    if (!TestNotNull("HealthComponent must exists.", HealthComponent))
        return false;

    const auto FuncName{"SetHealthData"};
    const auto MaxHealth = 200.0f;
    const FString AutoHeal{"No"};
    const auto AutoHealThreshold = 100.0f;
    const auto AutoHealRate = 0.5f;
    const auto AutoHealDelay = 2.0f;
    const auto AutoHealModifier = 1.0f;
    CallFunctionByNameWithParameters(Player,                                        //
                                     FuncName,                                      //
                                     {FString::SanitizeFloat(MaxHealth),            //
                                      AutoHeal,                                     //
                                      FString::SanitizeFloat(AutoHealThreshold),    //
                                      FString::SanitizeFloat(AutoHealRate),         //
                                      FString::SanitizeFloat(AutoHealDelay),        //
                                      FString::SanitizeFloat(AutoHealModifier)});

    Player->FinishSpawning(FTransform::Identity);

    AddInfo("Initial values. Completely healthy.");

    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealth(), MaxHealth));
    TestTrueExpr(FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f));

    AddInfo("OnHealthChanged delegate test.");

    float DelegateHealth = 0.0f;
    float DelegateHealthPercent = 0.0f;
    HealthComponent->OnHealthChanged.AddLambda(
        [&](float NewHealth, float NewHealthPercent)
        {
            DelegateHealth = NewHealth;
            DelegateHealthPercent = NewHealthPercent;
        });
    const auto DamageAmount = 20.0f;
    Player->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    TestTrueExpr(FMath::IsNearlyEqual(DelegateHealth, MaxHealth - DamageAmount));
    TestTrueExpr(FMath::IsNearlyEqual(DelegateHealthPercent, 1.0f - DamageAmount / MaxHealth));

    AddInfo("OnTakeDamage delegate test.");

    TestTrueExpr(HealthComponent->TryToHeal(MaxHealth));
    TestTrueExpr(HealthComponent->GetHealth() == MaxHealth);
    float DelegateDamage = 0.0f;
    HealthComponent->OnTakeLocalDamage.AddLambda(
        [&](float NewDamage, const FVector& NewLoacation, UPhysicalMaterial* NewPhysicalMaterial)
        {
            DelegateDamage = NewDamage;
        });

    AddInfo("OnTakeDamage delegate test. FPointDamageEvent.");

    Player->TakeDamage(DamageAmount, FPointDamageEvent{}, nullptr, nullptr);
    TestTrueExpr(FMath::IsNearlyEqual(DelegateDamage, DamageAmount));

    AddInfo("OnTakeDamage delegate test. FRadialDamageEvent.");

    DelegateDamage = 0.0f;
    UGameplayStatics::ApplyRadialDamage(World, DamageAmount, Player->GetActorLocation(), 100.0f, nullptr, {}, nullptr, nullptr, true);
    TestTrueExpr(FMath::IsNearlyEqual(DelegateDamage, DamageAmount));

    AddInfo("OnTakeDamage delegate test. FDamageEvent.");

    DelegateDamage = 0.0f;
    Player->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestTrueExpr(FMath::IsNearlyZero(DelegateDamage));

    AddInfo("OnDeath delegate test.");

    bool bIsDead = false;
    HealthComponent->OnDeath.AddLambda(
        [&](AController* KillerController, AController* VictimController)
        {
            bIsDead = true;
        });
    Player->TakeDamage(MaxHealth + 1.0f, FDamageEvent{}, nullptr, nullptr);

    TestTrueExpr(bIsDead);

    return true;
}

#endif
