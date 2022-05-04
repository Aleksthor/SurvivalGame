// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultTool.h"
#include "SurvivalCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ADefaultTool::ADefaultTool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetRootComponent());

	HitBox->SetGenerateOverlapEvents(false);

	EquipmentInfo.inInventory = false;
	
	
}

void ADefaultTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player)
	{
		EquipmentInfo.ScalingDamage = Player->Strength * EquipmentInfo.ScalingFactor;
	}
	

	if (!CanHit)
	{
		HitBox->SetGenerateOverlapEvents(false);
		CanHitClock += DeltaTime;
		if (CanHitClock > CanHitTimer)
		{
			
			CanHit = true;
			CanHitClock = 0.f;
		}
	}
}


void ADefaultTool::PickupEquipment(class ASurvivalCharacter* Char)
{
	
	if (Char)
	{

		ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(Char);
		if (Player)
		{
		
		
			Player->Equip(EquipmentInfo);
			Player->AddToEquipmentInventory(EquipmentInfo);
			this->Destroy();
			
		

		}

	}
}
