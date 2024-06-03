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

	// 월드에서 ATPSPlayer 타겟 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// ATPSPlayer 타입으로 캐스팅
	target = Cast<ATPSPlayer>(actor);
	// 소유 객체 가져오기
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
	currentTime += GetWorld()->DeltaTimeSeconds;	// 1. 시간의 누적 -> 경과 시간
	if (currentTime > idleDelayTime)	// 2. 만약 경과 시간이 대기 시간을 초과했다면
	{
		mstate = EEnemyState::Move;		// 3. 이동 상태로 전환
		currentTime = 0;				// 4. 경과 시간 초기화
	}
}
void UEnemyFSM::MoveState() 
{
	// 1. 타겟 목적지
	FVector destination = target->GetActorLocation();
	// 2. 방향
	FVector dir = destination - me->GetActorLocation();
	// 3. 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());

	// 타깃과 가까워지면 공격 상태로 전환
	if (dir.Size() < attackRange)
	{
		mstate = EEnemyState::Attack;
	}

}
void UEnemyFSM::AttackState() 
{
	// 구현 목표 1: 일정 시간에 한 번씩 공격
	
	// 1. 시간이 흘러야 한다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 공격시간이 됐으니깐
	if (currentTime > attackDelayTime)
	// 3. 공격
	{
		PRINT_LOG(TEXT("Attack!!!!!!!!"));
		currentTime = 0;
	}
	// 구현 목표 2: 타겟이 공격 범위를 벗어나면 상태를 이동으로 전환

	// 1. 타깃과의 거리
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. 타깃과의 거리가 공격 범위를 벗어났을 때
	if (distance > attackRange)
		// 3. 상태를 이동으로 전환
		mstate = EEnemyState::Move;
}
void UEnemyFSM::DamageState() 
{
	// 일정 시간 기다렸다가 상태를 대기로 변경
	// 1. 시간의 흐름
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약 경과 시간이 대기 시간을 초과하면
	if (currentTime > damageDelayTime)
	{
		// 3. 대기 상태로 전환
		mstate = EEnemyState::Idle;
		// 4. 경과 시간 초기화
		currentTime = 0;
	}
}


void UEnemyFSM::DieState() 
{
	me->Destroy();
}

// 피격 알림 이벤트 함수
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
