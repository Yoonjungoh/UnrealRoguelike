// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"

AABGameMode::AABGameMode()
{
	// static�� �����ν� ĳ�̵� �ǰ� �����ڰ� ȣ��ɶ����� �ش� �Լ��� ȣ��Ǵ� �� �ƴ϶� ����
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (DefaultPawnClassRef.Class != nullptr)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}	
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}