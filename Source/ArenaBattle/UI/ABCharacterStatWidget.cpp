// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControllBlockName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		const FName TextModifierControllBlockName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());
		
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControllBlockName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControllBlockName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
		
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierLookupPtr = ModifierLookup.Find(PropKey);
		if (ModifierLookupPtr)
		{
			(*ModifierLookupPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
