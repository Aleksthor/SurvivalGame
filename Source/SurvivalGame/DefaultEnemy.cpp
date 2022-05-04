// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "SurvivalCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DefaultTool.h"

// Sets default values
ADefaultEnemy::ADefaultEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AgroCollider"));
	AgroCollider->SetupAttachment(GetRootComponent());
	AgroCollider->InitSphereRadius(500.f);

	AttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollider"));
	AttackCollider->SetupAttachment(GetRootComponent());
	
	
	
	DamageCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(GetMesh(), TEXT("DamageSocket"));
	DamageCollider->SetGenerateOverlapEvents(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	//bUseControllerRotationYaw = false;

	isAttacking = false;
}

// Called when the game starts or when spawned
void ADefaultEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());
	
	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ADefaultEnemy::OnOverlapAttack);

	AgroCollider->OnComponentBeginOverlap.AddDynamic(this, &ADefaultEnemy::AgroColldierBeginOverlap);
	AgroCollider->OnComponentEndOverlap.AddDynamic(this, &ADefaultEnemy::AgroColldierEndOverlap);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ADefaultEnemy::OnOverlapStartAttack);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &ADefaultEnemy::OnOverlapEndAttack);

	Cast<UCapsuleComponent>(GetRootComponent())->OnComponentBeginOverlap.AddDynamic(this, &ADefaultEnemy::OnOverlapLooseHealth);
}

// Called every frame
void ADefaultEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		CombatTarget->GetXP(XPGiven);
		this->Destroy();
		
	}
	if (SetAgro)
	{
		AgroClock += DeltaTime;
		if (AgroClock > AgroTimer)
		{
			Agro = true;
			
		}
	}

}

// Called to bind functionality to input
void ADefaultEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADefaultEnemy::MoveToTarget(ASurvivalCharacter* Player)
{
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Player);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;

		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

		AIController->MoveTo(MoveRequest, &NavPath);
		AIController->SetFocus(CombatTarget);
	}
}





void ADefaultEnemy::OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(OtherActor);
	if (Player)
	{
		if (OtherComponent == Player->GetRootComponent())
		{
			Player->LoseHealth(Damage);
		}
	}
}

void ADefaultEnemy::OnOverlapLooseHealth(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADefaultTool* Tool = Cast<ADefaultTool>(OtherActor);
	if (Tool)
	{
		if (OtherComponent == Tool->HitBox)
		{
			Health -= Tool->EquipmentInfo.BaseDamage + Tool->EquipmentInfo.ScalingDamage;
			SetAgro = true;
		}
	}
}

void ADefaultEnemy::AgroColldierBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivalCharacter* Player = Cast< ASurvivalCharacter>(OtherActor);
	if (Player)
	{
		if (OtherComponent == Player->GetRootComponent())
		{
			CombatTarget = Player;
		}
		
	}
}

void ADefaultEnemy::AgroColldierEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ASurvivalCharacter* Player = Cast< ASurvivalCharacter>(OtherActor);
	if (Player)
	{
		if (OtherComponent == Player->GetRootComponent())
		{
			
			CombatTarget = nullptr;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				
			
		}
	}
}

void ADefaultEnemy::OnOverlapStartAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASurvivalCharacter* Player = Cast< ASurvivalCharacter>(OtherActor);
	if (Player)
	{
		if (OtherComponent == Player->GetRootComponent())
		{
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}

}

void ADefaultEnemy::OnOverlapEndAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
}
