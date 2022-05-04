// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultItem.generated.h"

class ADefaultTool;


UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Plants			UMETA(DeplayName = "Plants"),
	EIT_Material		UMETA(DeplayName = "Material"),
	EIT_Equipment		UMETA(DeplayName = "Equipment"),
	EIT_Food			UMETA(DeplayName = "Food"),
	EIT_Armor			UMETA(DeplayName = "Armor"),

	EIT_MAX				UMETA(DeplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common			UMETA(DeplayName = "Common"),
	EIR_UnCommon		UMETA(DeplayName = "UnCommon"),
	EIR_Rare			UMETA(DeplayName = "Rare"),
	EIR_Epic			UMETA(DeplayName = "Epic"),
	EIR_Legendary		UMETA(DeplayName = "Legendary"),

	EIR_MAX				UMETA(DeplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	EAT_Head			UMETA(DeplayName = "Head"),
	EAT_Chest			UMETA(DeplayName = "Chest"),
	EAT_Pants			UMETA(DeplayName = "Legs"),
	EAT_Boots			UMETA(DeplayName = "Boots"),
	EAT_Belt			UMETA(DeplayName = "Belt"),
	EAT_Ring			UMETA(DeplayName = "Ring"),
	EAT_EarRing			UMETA(DeplayName = "EarRing"),

	EAT_MAX				UMETA(DeplayName = "DefaultMAX")
};


UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_Hand			UMETA(DeplayName = "Hand"),
	EET_Sword			UMETA(DeplayName = "Sword"),
	EET_Pickaxe			UMETA(DeplayName = "Pickaxe"),
	EET_Axe				UMETA(DeplayName = "Axe"),
	EET_Knife			UMETA(DeplayName = "Knife"),

	EET_MAX				UMETA(DeplayName = "DefaultMAX")
};


USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PickupAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartingWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoodItem")
	float HealthGiven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoodItem")
	float HungerHiven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inInventory;

	
};

USTRUCT(BlueprintType)
struct FEquipmentInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADefaultTool> ToolBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScalingFactor = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScalingDamage;

	

};

USTRUCT(BlueprintType)
struct FArmorInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArmorType ArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageNegation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeedPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool inInventory;



};


UCLASS()
class SURVIVALGAME_API ADefaultItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ItemInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemVariables")
	class USphereComponent* PickupCollider{ nullptr };

	

	void Pickup(class ASurvivalCharacter* Char);



};
