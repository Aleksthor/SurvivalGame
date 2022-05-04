// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("PickupColldier"));
	SetRootComponent(PickupCollider);
	PickupCollider->InitSphereRadius(100.f);
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

