// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle			UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DeplayName = "Attacking"),

	EMS_MAX				UMETA(DeplayName = "DefaultMAX")
};
class USphereComponent;

UCLASS()
class SURVIVALGAME_API ADefaultEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* DamageCollider { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* AgroCollider {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* AttackCollider{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyVariables | Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyVariables | AI")
	class AAIController* AIController{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyVariables | AI")
	class ASurvivalCharacter* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables | AI")
	bool Agro = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SetAgro = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AgroTimer = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AgroClock = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
	float Damage = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
	float Health = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
	float MaxHealth = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
	float XPGiven = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyVariables")
	bool isAttacking;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(ASurvivalCharacter* Player);

	UFUNCTION()
	void OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapLooseHealth(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AgroColldierBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AgroColldierEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapStartAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
