 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultItem.h"
#include "SurvivalCharacter.h"
#include "DefaultTool.generated.h"


/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ADefaultTool : public ADefaultItem
{
	GENERATED_BODY()

public:

	ADefaultTool();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToolVariable")
	class UCapsuleComponent* HitBox;


	UFUNCTION(BlueprintCallable)
	void PickupEquipment(class ASurvivalCharacter* Char);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Equipped = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanHit = true;

	float CanHitTimer = 0.5f;

	float CanHitClock = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipmentInfo EquipmentInfo;

	
};
