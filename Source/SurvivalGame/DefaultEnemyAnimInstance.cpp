// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultEnemyAnimInstance.h"
#include "DefaultEnemy.h"

void UDefaultEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<ADefaultEnemy>(Pawn);
		}
	}
}

void UDefaultEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		Enemy = Cast<ADefaultEnemy>(Pawn);
		Agro = Enemy->Agro;
	}
}