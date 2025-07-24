// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		// uid를 키값으로 해도 되지만 현재 레벨 순서대로 가기 때문에 배열에 저장해주는 게 더 유리함
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::Get()
{
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}
	
	// 여기까지 올리는 없지만 혹시 모르니 로그 찍기
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UABGameSingleton>();
}
