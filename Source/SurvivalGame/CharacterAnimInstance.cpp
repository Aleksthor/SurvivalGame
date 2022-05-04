// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // Inherited from UAnimInstance
	}
	Equipped = false;

}

void UCharacterAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // Inherited from UAnimInstance
	}
	if (Pawn)
	{
		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(Pawn);
		if (Character)
		{
			Initialize();
			bisInAir = Character->GetMovementComponent()->IsFalling();
			bisPickup = Character->bisPickup;
			EquipType = Character->EquipmentType;
			bisAttack = Character->bisAttack;
			if (Character->EquippedItem)
			{
				Equipped = true;
			}
			else
			{
				Equipped = false;
			}
			Speed_X = UKismetMathLibrary::Dot_VectorVector(Character->GetVelocity(), Character->GetActorForwardVector());
			Speed_Y = UKismetMathLibrary::Dot_VectorVector(Character->GetVelocity(), Character->GetActorRightVector());
			Speed_Length = FMath::FInterpTo(Speed_Length, Character->GetVelocity().Size(), UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 20.f);
			if (bisAcceleration)
			{
				Speed_Degree = UKismetMathLibrary::Atan2(Speed_Y, Speed_X);
			}
			if (bisAcceleration && Speed_Length > 200.f)
			{
				LastSpeed_Degree = Speed_Degree;
			}
			inputDirection_Degree = UKismetMathLibrary::Atan2(inputDirection_Y, inputDirection_X);


			if (inputDirection_X == 0.f && inputDirection_Y == 0.f)
			{
				inputDirection_Degree = 180.f;
			}
		}



		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}
}

void UCharacterAnimInstance::Initialize()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner(); // Inherited from UAnimInstance
	}
	if (Pawn)
	{
		ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(Pawn);
		if (Character)
		{
		
			DamagePosition = Character->DamagePosition;
			bisRun = Character->bisRun;
			inputDirection_X = Character->inputVector.X;
			inputDirection_Y = Character->inputVector.Y;
			bisAcceleration = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

			ViewRotation = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetControlRotation(), Character->GetActorRotation());

			bisWalk = Character->bisWalk;

		}

	

	}

}
