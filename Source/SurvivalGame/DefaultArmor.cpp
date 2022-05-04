// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultArmor.h"
#include "SurvivalCharacter.h"

ADefaultArmor::ADefaultArmor()
{
	ArmorInfo.inInventory = false;
	
}

void ADefaultArmor::PickupArmor(ASurvivalCharacter* Char)
{
	if (Char)
	{

		ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(Char);
		if (Player)
		{

			
			Player->EquipArmor(ArmorInfo);
			Player->AddToArmorInventory(ArmorInfo);
			this->Destroy();


		}

	}
}
