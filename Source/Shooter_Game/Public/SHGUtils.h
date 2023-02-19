#pragma once

class SHGUtils
{
public:
    inline static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
    static FString FormatStringFromInt(int32 InValue, int32 MinLen, TCHAR FillChar);
    static bool AreEnemies(AController* Controller1, AController* Controller2);
    static bool ApplyRadialDamageWithImpulse(const UObject* WorldContextObject, float BaseDamage, float MinimumDamage, float ImpulsePower, const FVector& Origin,
                                             float DamageInnerRadius, float DamageOuterRadius, bool bDoFullDamage, TSubclassOf<UDamageType> DamageTypeClass,
                                             const TArray<AActor*>& IgnoreActors, AActor* DamageCauser, AController* InstigatedByController,
                                             ECollisionChannel DamagePreventionChannel = ECC_Visibility);
};
