// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }

	FORCEINLINE void SetMaxHp(float hp) { MaxHp = hp; }
	void SetCurrentHp(float NewHp);

	float ApplyDamage(float InDamge);

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;	// VisibleInstanceOnly: 인스턴스마다 별도 관리

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;	// Transient: 디스크에 저장 안 함
};
