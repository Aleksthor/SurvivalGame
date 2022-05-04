// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SurvivalCharacter.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: 
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed_X;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed_Y;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed_Degree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float LastSpeed_Degree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed_Length;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EEquipmentType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ComboState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisHurt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisWalk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisPickup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float inputDirection_X;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float inputDirection_Y;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float inputDirection_Degree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector DamagePosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	int HurtType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bisAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FRotator ViewRotation;

};
