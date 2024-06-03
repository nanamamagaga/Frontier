// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ���忡�� ATPSPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// ATPSPlayer Ÿ������ ĳ����
	target = Cast<ATPSPlayer>(actor);
	// ���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());


	// ...
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mstate)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState() 
{
	currentTime += GetWorld()->DeltaTimeSeconds;	// 1. �ð��� ���� -> ��� �ð�
	if (currentTime > idleDelayTime)	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	{
		mstate = EEnemyState::Move;		// 3. �̵� ���·� ��ȯ
		currentTime = 0;				// 4. ��� �ð� �ʱ�ȭ
	}
}
void UEnemyFSM::MoveState() 
{
	// 1. Ÿ�� ������
	FVector destination = target->GetActorLocation();
	// 2. ����
	FVector dir = destination - me->GetActorLocation();
	// 3. �������� �̵��ϰ� �ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());

	// Ÿ��� ��������� ���� ���·� ��ȯ
	if (dir.Size() < attackRange)
	{
		mstate = EEnemyState::Attack;
	}

}
void UEnemyFSM::AttackState() 
{
	// ���� ��ǥ 1: ���� �ð��� �� ���� ����
	
	// 1. �ð��� �귯�� �Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���ݽð��� �����ϱ�
	if (currentTime > attackDelayTime)
	// 3. ����
	{
		PRINT_LOG(TEXT("Attack!!!!!!!!"));
		currentTime = 0;
	}
	// ���� ��ǥ 2: Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ

	// 1. Ÿ����� �Ÿ�
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. Ÿ����� �Ÿ��� ���� ������ ����� ��
	if (distance > attackRange)
		// 3. ���¸� �̵����� ��ȯ
		mstate = EEnemyState::Move;
}
void UEnemyFSM::DamageState() 
{
	// ���� �ð� ��ٷȴٰ� ���¸� ���� ����
	// 1. �ð��� �帧
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (currentTime > damageDelayTime)
	{
		// 3. ��� ���·� ��ȯ
		mstate = EEnemyState::Idle;
		// 4. ��� �ð� �ʱ�ȭ
		currentTime = 0;
	}
}


void UEnemyFSM::DieState() 
{
	me->Destroy();
}

// �ǰ� �˸� �̺�Ʈ �Լ�
void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mstate = EEnemyState::Damage;
	}
	else
	{
		mstate = EEnemyState::Die;
	}
}
