// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture.h"
#include "SoulAssetManager.h"
#include "ItemBasic.generated.h"

class USoulGameplayAbility;

UENUM(BlueprintType)
enum class EGearType : uint8
{
    Non_Gear,
    Weapon,
    BodyArmor,
    Helmet,
    Boots,
    Gloves,
    Ring,
    Amulet,
    Reliquary,
    Jewel
};

/** Base class for all items, do not blueprint directly */
UCLASS(Abstract, BlueprintType)
class SOUL_LIKE_ACT_API USoulItem : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    /** Constructor */
    USoulItem()
        : MaxCount(1)
          , MaxLevel(1)
    {
    }

    /** Type of this item, set in native parent class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
    FPrimaryAssetType ItemType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
    EGearType ItemSlotType;

    /** User-visible short name */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
    FText ItemName;

    /** User-visible long description */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
    FText ItemDescription;

    /** Icon to display */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
    UTexture* ItemIcon;

    /** Maximum number of instances that can be in inventory at once, <= 0 means infinite */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Max)
    int32 MaxCount;

    /** Returns if the item is consumable (MaxCount <= 0)*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
    bool IsConsumable() const;

    /** Maximum level this item can be, <= 0 means infinite */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Max)
    int32 MaxLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
    int32 MaxSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
    TSubclassOf<class USoulPrimaryStatusGameplayAbility> PrimaryAbility;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
    int32 PrimaryAbilityLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
    TMap<TSubclassOf<class USoulModifierGameplayAbility>, int32> Modifiers;

    /** Returns the logical name, equivalent to the primary asset id */
    UFUNCTION(BlueprintCallable, Category = Item)
    FString GetIdentifierString() const;

    /** Overridden to use saved type */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};

UCLASS()
class SOUL_LIKE_ACT_API USoulWeaponItem : public USoulItem
{
    GENERATED_BODY()

public:
    /** Constructor */
    USoulWeaponItem()
    {
        ItemType = USoulAssetManager::WeaponItemType;
        ItemSlotType = EGearType::Weapon;
    }
};

UCLASS()
class SOUL_LIKE_ACT_API USoulArmourItem : public USoulItem
{
    GENERATED_BODY()

public:

    /** Constructor */
    USoulArmourItem()
    {
        ItemType = USoulAssetManager::ArmourItemType;
        ItemSlotType = EGearType::BodyArmor;
    }
};

UCLASS()
class SOUL_LIKE_ACT_API USoulPotionItem : public USoulItem
{
    GENERATED_BODY()

public:
    /** Constructor */
    USoulPotionItem()
    {
        ItemType = USoulAssetManager::PotionItemType;
        MaxCount = 10; // Infinite
        ItemSlotType = EGearType::Non_Gear;
    }
};

UCLASS()
class SOUL_LIKE_ACT_API USoulJewelItem : public USoulItem
{
    GENERATED_BODY()

public:
    /** Constructor */
    USoulJewelItem()
    {
        ItemType = USoulAssetManager::JewelItemType;
        ItemSlotType = EGearType::Jewel;
    }
};
