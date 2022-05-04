// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultItem.h"
#include "Components/SphereComponent.h"
#include "SurvivalCharacter.h"

// Sets default values
ADefaultItem::ADefaultItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("PickupColldier"));
	SetRootComponent(PickupCollider);
	PickupCollider->InitSphereRadius(100.f);

	ItemInfo.Weight = 1.f;
	ItemInfo.Name = "Item";
	ItemInfo.StackAmount = 0;
	ItemInfo.PickupAmount = 1;
	ItemInfo.inInventory = false;

	
	

	ItemInfo.StartingWeight = ItemInfo.Weight;
}

// Called when the game starts or when spawned
void ADefaultItem::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ADefaultItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADefaultItem::Pickup(ASurvivalCharacter* Char)
{
	if (Char)
	{

		ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(Char);
		if (Player)
		{
		

			ItemInfo.StackAmount += ItemInfo.PickupAmount;
			ItemInfo.Weight = ItemInfo.StartingWeight * ItemInfo.StackAmount;

			for (int i{}; i < Player->Inventory.ItemList.Num(); i++)
			{
				if (Player->Inventory.ItemList[i].Name == ItemInfo.Name)
				{
					ItemInfo.inInventory = true;
					Player->Inventory.ItemList[i].StackAmount += ItemInfo.PickupAmount;
					Player->Inventory.ItemList[i].Weight = Player->Inventory.ItemList[i].StackAmount * Player->Inventory.ItemList[i].StartingWeight;
				}
			}
			if (!ItemInfo.inInventory)
			{
				Player->AddToInventory(ItemInfo);
				
			}
			

		}
	
	}


	
}




