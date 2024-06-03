// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mstate = EEnemyState::Idle;

	void IdleState();	// 대기 상태
	void MoveState();	// 이동 상태
	void AttackState();	// 공격 상태
	void DamageState();	// 피격 상태
	void DieState();	// 죽음 상태

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2;	// 대기 시간
	float currentTime = 0;		// 경과 시간

	// 타겟
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ATPSPlayer* target;
	// 소유 액터
	UPROPERTY()
	class AEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;

	// 공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 2.0f;

	// 피격 알림 이벤트 함수
	void OnDamageProcess();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 hp = 3;

	// 피격 시 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.0f;
};
