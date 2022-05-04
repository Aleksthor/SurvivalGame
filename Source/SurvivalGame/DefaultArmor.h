// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultItem.h"
#include "DefaultArmor.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ADefaultArmor : public ADefaultItem
{
	GENERATED_BODY()

public:

	ADefaultArmor();

	UFUNCTION(BlueprintCallable)
	void PickupArmor(class ASurvivalCharacter* Char);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FArmorInfo ArmorInfo;
	
};
