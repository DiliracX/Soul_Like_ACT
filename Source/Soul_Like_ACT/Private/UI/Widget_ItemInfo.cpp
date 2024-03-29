// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_ItemInfo.h"
#include "Types/SoulItemTypes.h"
#include "Item/ItemBasic.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "BPFL/SoulSerializerBpLib.h"

void UWidget_ItemInfo::MakeItemInfo_Implementation(const FSoulItemData& ItemData)
{
    if (!(&ItemData))
    {
        LOG_FUNC_ERROR("ItemData invalid");
        return;
    }
    ItemName->SetText(ItemData.ItemBase->ItemName);

    ItemType->SetText(FText::FromString(ItemData.ItemBase->ItemType.ToString()));

    ItemLevel->SetText(FText::FromString(FString::FromInt(ItemData.ItemLevel)));

    //TODO Set primary status name
    TArray<FText> PrimStatNames, PrimStatValues;
    bool bSuccessful;
    USoulSerializerBpLib::GetPrimaryStatusFromItem(ItemData.ItemBase, PrimStatNames, PrimStatValues, bSuccessful);

    if (bSuccessful)
    {
        ItemMainStat->SetText(PrimStatValues[0]);
    }
    //Item level
    if (ItemData.ItemLevel <= 1)
    {
        ItemStat->SetVisibility(ESlateVisibility::Hidden);
        ItemStatPlus->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        ItemStat->SetVisibility(ESlateVisibility::Visible);
        ItemStatPlus->SetVisibility(ESlateVisibility::Visible);
        ItemStat->SetText(FText::FromString(FString::FromInt(ItemData.ItemLevel * 2)));
    }

    //Item Count
    if (ItemData.ItemBase->MaxCount == 1)
    {
        ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        ItemQuantity->SetVisibility(ESlateVisibility::Visible);
        ItemQuantity->SetText(FText::FromString(FString::FromInt(ItemData.ItemCount)));
    }

    //Icon
    UMaterialInstanceDynamic* DynIcon = ItemIcon->GetDynamicMaterial();
    DynIcon->SetTextureParameterValue(FName("IconTexture"), ItemData.ItemBase->ItemIcon);

    //Description
    if (ItemData.ItemBase->ItemDescription.IsEmpty())
    {
        ItemDescription->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        ItemDescription->SetVisibility(ESlateVisibility::Visible);
        ItemDescription->SetText(ItemData.ItemBase->ItemDescription);
    }
}
