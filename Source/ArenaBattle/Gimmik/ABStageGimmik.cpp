// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmik/ABStageGimmik.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Item/ABItemBox.h"

// Sets default values
AABStageGimmik::AABStageGimmik()
{
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmik::OnStageTriggerBeginOverlap);

	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	for (FName GateSocket : GateSockets)
	{
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		Gate->SetStaticMesh(GateMeshRef.Object);
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		Gates.Add(GateSocket, Gate);

		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmik::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket);	// ĳ���Ͱ� �������� ����Ʈ �� ��� ������ �ĺ��ϱ� ���� �±װ�
		
		GateTriggers.Add(GateTrigger);
		
	}

	// State Section
	CurrentState = EStageState::READY;
	StateChangeActions.Add(EStageState::READY, FStageChangedDelgateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmik::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelgateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmik::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelgateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmik::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelgateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmik::SetChooseNext)));

	// Fight Section
	OpponentSpawnTime = 2.0f;
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	// Reward Section
	RewardBoxClass = AABItemBox::StaticClass();
	for (FName GateSocket : GateSockets)
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

void AABStageGimmik::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetState(CurrentState);
}

void AABStageGimmik::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	SetState(EStageState::FIGHT);
}

void AABStageGimmik::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1);	// �±� ���� Ȯ�� assert
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));	// (+X, -X, +Y, -Y) �Ľ�
	check(Stage->DoesSocketExist(SocketName));	// ���� ���� üũ

	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	// �ش� ��ġ�� �̹� ���������� �ֳ� �˻�
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);	// ����(���� ��������) �ݸ��� üũ���� ����
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		FCollisionShape::MakeSphere(775.0f),	// �ش� ��ġ�� �� ����
		CollisionQueryParam
	);

	// �ƹ� �͵� ������ ��������(��� ����) ��ȯ
	if (bResult == false)
	{
		GetWorld()->SpawnActor<AABStageGimmik>(NewLocation, FRotator::ZeroRotator);
	}
}

void AABStageGimmik::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmik::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmik::SetState(EStageState InNewState)
{
	CurrentState = InNewState;
	if (StateChangeActions.Contains(CurrentState))
	{
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void AABStageGimmik::SetReady()
{
	// �� ���� ���� Ʈ���� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	for (auto GateTrigger : GateTriggers)
	{
		// ���� �ݸ��� Ȱ��ȭ �� ��
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	OpenAllGates();
}

void AABStageGimmik::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmik::OnOpponentSpawn, OpponentSpawnTime, false);
}

void AABStageGimmik::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();
	SpawnRewardBoxes();
}

void AABStageGimmik::SetChooseNext()
{
	// �� ���� ���� Ʈ���� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		// ���� �ݸ��� Ȱ��ȭ �� ��
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	OpenAllGates();
}

void AABStageGimmik::OnOpponentDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void AABStageGimmik::OnOpponentSpawn()
{
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);
	if (ABOpponentCharacter)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmik::OnOpponentDestroyed);
	}
}

void AABStageGimmik::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		// �� ������ nullptr�� �� ������ üũ
		if (RewardBox.IsValid())
		{
			AABItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox)
			{
				// ���� �ϳ��� ����
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}

void AABStageGimmik::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f);
		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmik::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}

