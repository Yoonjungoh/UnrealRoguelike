// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"
#include "Character/ABCharacterBase.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage, AActor* DamageCauser)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		TransferGoldToDamageCauser(DamageCauser);
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
}

void UABCharacterStatComponent::TransferGoldToDamageCauser(AActor* DamageCauser)
{
	if (DamageCauser == nullptr)
		return;

	AABCharacterBase* CharacterDamageCauser = Cast<AABCharacterBase>(DamageCauser);

	TObjectPtr<UABCharacterStatComponent> KillerStatComponent = CharacterDamageCauser->GetStat();
	
	if (KillerStatComponent == nullptr)
		return;

	int32 AcquiredGold = GetGold();
	
	// óġ�� ĳ������ ��忡 ���� ��常ŭ ȹ��
	// ���Ͱ� ���͸� ���̸� ��尡 ���� - �Ŀ� ��Ƽ�� ��Ʋ�ξ� ���
	KillerStatComponent->SetGold(KillerStatComponent->GetGold() + AcquiredGold);

	UE_LOG(LogTemp, Warning, TEXT("ȹ���� ���: %d, ���� ���: %d"), AcquiredGold, KillerStatComponent->GetGold());
}

