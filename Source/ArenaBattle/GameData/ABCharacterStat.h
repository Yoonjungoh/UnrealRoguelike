
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float MovementSpeed;

	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		FABCharacterStat Result;

		Result.MaxHp = MaxHp + Other.MaxHp;
		Result.Attack = Attack + Other.Attack;
		Result.AttackRange = AttackRange + Other.AttackRange;
		Result.AttackSpeed = AttackSpeed + Other.AttackSpeed;
		Result.MovementSpeed = MovementSpeed + Other.MovementSpeed;

		return Result;
	}

};
