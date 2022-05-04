// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "DefaultTool.h"
#include "Components/CapsuleComponent.h"
#include "SurvivalCharacter.h"

// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	SetRootComponent(HitBox);


}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ATree::OnHit);
	
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATree::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADefaultTool* Tool = Cast<ADefaultTool>(OtherActor);
	if (Tool)
	{
		if (Tool->EquipmentInfo.EquipmentType == EEquipmentType::EET_Axe)
		{
			if (OtherComponent == Tool->HitBox && Tool->CanHit)
			{
				FVector Random = FVector(FMath::FRandRange(GetActorLocation().X - 200.f, GetActorLocation().X + 200.f), FMath::FRandRange(GetActorLocation().Y - 200.f, GetActorLocation().Y + 200.f), GetActorLocation().Z + 100.f);
				GetWorld()->SpawnActor<ADefaultItem>(SticksBP, Random, FRotator(0.f, 0.f, 0.f));
				Tool->CanHit = false;
			}
			
		}
	}
}

