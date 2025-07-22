// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	SetMaxHp(200.0f);
	SetCurrentHp(GetMaxHp());
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetCurrentHp(GetMaxHp());
}

void UABCharacterStatComponent::SetCurrentHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(GetCurrentHp());
}

float UABCharacterStatComponent::ApplyDamage(float InDamge)
{
	const float PrevHp = GetCurrentHp();
	const float ActualDamage = FMath::Clamp<float>(InDamge, 0.0f, InDamge);

	SetCurrentHp(PrevHp - ActualDamage);
	if (GetCurrentHp() <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

