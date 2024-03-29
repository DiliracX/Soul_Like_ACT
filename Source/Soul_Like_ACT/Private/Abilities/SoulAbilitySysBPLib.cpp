// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulAbilitySysBPLib.h"
#include "AbilitySystemComponent.h"

USoulAbilitySysBPLib::USoulAbilitySysBPLib(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void USoulAbilitySysBPLib::CreateEventData(const AActor* Target, const AActor* Source, const FHitResult& InpHitResult,
                                           const FGameplayTag EventTag, const float EventMagnitude,
                                           FGameplayEventData& OutpEventData)
{
    FGameplayEffectContextHandle TempContextHandle(new FGameplayEffectContext());
    TempContextHandle.AddHitResult(InpHitResult);

    FGameplayEventData TempEventData;
    TempEventData.Instigator = Source;
    TempEventData.Target = Target;
    TempEventData.EventMagnitude = EventMagnitude;
    TempEventData.EventTag = EventTag;
    TempEventData.ContextHandle = TempContextHandle;

    OutpEventData = TempEventData;
}

void USoulAbilitySysBPLib::CreateEventDataWithoutHitResult(const AActor* Target, const AActor* Source,
    const FGameplayTag EventTag, const float EventMagnitude, FGameplayEventData& OutpEventData)
{
    FGameplayEventData TempEventData;
    TempEventData.Instigator = Source;
    TempEventData.Target = Target;
    TempEventData.EventMagnitude = EventMagnitude;
    TempEventData.EventTag = EventTag;
    OutpEventData = TempEventData;
}

bool USoulAbilitySysBPLib::OverrideActorGameplayTag(UAbilitySystemComponent* AbilitySysComp,
                                                    const FGameplayTag& GameplayTag, bool bAdd)
{
    if (AbilitySysComp)
    {
        AbilitySysComp->SetTagMapCount(GameplayTag, bAdd);
        return true;
    }

    return false;
}

bool USoulAbilitySysBPLib::DoesActorHasTag(UAbilitySystemComponent* AbilitySysComp, FGameplayTag InTag)
{
    if (AbilitySysComp)
        return AbilitySysComp->HasMatchingGameplayTag(InTag);

    return false;
}

bool USoulAbilitySysBPLib::DoesActorHasAnyTags(UAbilitySystemComponent* AbilitySysComp,
                                               FGameplayTagContainer InTagContainer)
{
    if (AbilitySysComp)
        return AbilitySysComp->HasAnyMatchingGameplayTags(InTagContainer);

    return false;
}

UGameplayEffectUIData* USoulAbilitySysBPLib::GetActiveGameplayEffectUIData(FActiveGameplayEffectHandle Handle)
{
    UAbilitySystemComponent* ASC = Handle.GetOwningAbilitySystemComponent();
    const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(Handle);
    if (ActiveGE)
    {
        return ActiveGE->Spec.Def->UIData;
    }

    return nullptr;
}
