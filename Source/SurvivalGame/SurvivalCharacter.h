// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultItem.h"
#include "SurvivalCharacter.generated.h"


USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemInfo> ItemList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquipmentInfo> EquipmentList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FArmorInfo> ArmorList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InventoryWeightLimit;


};

UCLASS()
class SURVIVALGAME_API ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PickupCollider;

	// Equipped Tool

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	class ADefaultItem* EquippedItem{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	EEquipmentType EquipmentType;

		// Function to set and get weapon

	UFUNCTION(BlueprintCallable)
	void SetEquippedItem(ADefaultItem* ItemToSet);
	FORCEINLINE ADefaultItem* GetEquippedItem() { return EquippedItem; }





	// Equipped HeadPiece

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedHeadPiece;

		// Function to set and get headpiece

	UFUNCTION(BlueprintCallable)
	void SetEquippedHeadPiece(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedHeadPIece() { return EquippedHeadPiece; }

	// Equipped Chest


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedChestPiece;

		// Function to set and get Chestpiece
	UFUNCTION(BlueprintCallable)
	void SetEquippedChestPiece(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedChestPIece() { return EquippedChestPiece; }


	// Equipped Pants


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedPants;

		// Function to set and get PantsPiece

	UFUNCTION(BlueprintCallable)
	void SetEquippedPants(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedPants() { return EquippedPants; }



	// Equipped Boots


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedBoots;

		// Function to set and get Boots

	UFUNCTION(BlueprintCallable)
	void SetEquippedBoots(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedBoots() { return EquippedBoots; }


	// Equipped Belt


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedBelt;

		// Function to set and get PantsPiece

	UFUNCTION(BlueprintCallable)
	void SetEquippedBelt(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedBelt() { return EquippedBelt; }


	// Equipped Ring


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedRing;

		// Function to set and get PantsPiece

	UFUNCTION(BlueprintCallable)
	void SetEquippedRing(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedRing() { return EquippedRing; }



	// Equipped EarRing


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | ActiveEquipment")
	FArmorInfo EquippedEarRing;

		// Function to set and get PantsPiece

	UFUNCTION(BlueprintCallable)
	void SetEquippedEarRing(FArmorInfo ItemToSet);
	FORCEINLINE FArmorInfo GetEquippedEarRing() { return EquippedEarRing; }






	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	class ADefaultItem* ActiveOverlappingItem{ nullptr };

	FORCEINLINE void SetActiveOverlappingItem(ADefaultItem* ItemOverlapping) { ActiveOverlappingItem = ItemOverlapping; }

	void TryGetNewOverlappingItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Chest")
	class AChest* ActiveOverlappingChest{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Chest")
	bool bChestOpen;

	FORCEINLINE void SetActiveOverlappingChest(AChest* ChestOverlapping) { ActiveOverlappingChest = ChestOverlapping; }




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forward/backwards input */
	void MoveForward(float Value);

	/** Called for Right/Left input */
	void MoveRight(float Value);

	/** Called when we press Use button*/
	void Pickup();

	/** Called when we press Attack */
	void Attack();

	/** Called when we press SpaceBar */
	void SprintPressed();

	/** Called when we release SpaceBar */
	void SprintReleased();

	// Animation Blueprint Components

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	int runTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	int pre_runTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisRun;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisHoldSpace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisWalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisLocomotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bIsTargetLock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool isStarve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	bool bisSnapRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputY;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputX_Local;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float inputY_Local;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float Speed_Run;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float Speed_Jog;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	float Speed_Walk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector inputVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector inputVector_World;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector LineEndTemp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Animation")
	FVector DamagePosition;

	bool Retrigger;
	float RetriggerDelay = 0.2;
	float RetriggerClock = 0.f;


	// Snap Rotation Fn
	void SnapRotationFn();

	void RunorWalk();

	UFUNCTION(BlueprintCallable)
	void SnapRotation();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION(BlueprintCallable)
	void SetHitBox();

	UFUNCTION(BlueprintCallable)
	void ResetHitBox();

	UFUNCTION(BlueprintCallable)
	void ResetPickup();


	// Inventory

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Inventory")
	FInventory Inventory;

		// Add to Inventory

	UFUNCTION(BlueprintCallable)
	void AddToInventory(FItemInfo Item);

	UFUNCTION(BlueprintCallable)
	void AddToEquipmentInventory(FEquipmentInfo Equipment);

	UFUNCTION(BlueprintCallable)
	void AddToArmorInventory(FArmorInfo Armor);

		// Remove From Inventory

	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(FItemInfo Item, int Quantity);

	UFUNCTION(BlueprintCallable)
	void RemoveFromEquipmentInventory(FEquipmentInfo Equipment);

	UFUNCTION(BlueprintCallable)
	void RemoveFromArmorInventory(FArmorInfo Armor);

		// Query Inventory

	UFUNCTION(BlueprintCallable)
	bool QueryInventory(FItemInfo Item, int Quantity);

	UFUNCTION(BlueprintCallable)
	bool QueryEquipmentInventory(FEquipmentInfo Equipment);

	UFUNCTION(BlueprintCallable)
	bool QueryArmorInventory(FArmorInfo Item);



	// Chest

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Chest")
	FInventory ChestInventory;

		// Add To Chest Inventory

	UFUNCTION(BlueprintCallable)
	void AddItemToChestInventory(FItemInfo Item);

	UFUNCTION(BlueprintCallable)
	void AddEquipmentToChestInventory(FEquipmentInfo Equipment);

	UFUNCTION(BlueprintCallable)
	void AddArmorToChestInventory(FArmorInfo Armor);

		// Remove From Chest Inventory

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromChestInventory(FItemInfo Item, int Quantity);

	UFUNCTION(BlueprintCallable)
	void RemoveEquipmentFromChestInventory(FEquipmentInfo Equipment);

	UFUNCTION(BlueprintCallable)
	void RemoveArmorFromChestInventory(FArmorInfo Armor);

		// Query Chest Inventory

	UFUNCTION(BlueprintCallable)
	bool QueryChestItemInventory(FItemInfo Item, int Quantity);

	UFUNCTION(BlueprintCallable)
	bool QueryChestEquipmentInventory(FEquipmentInfo Equipment);

	UFUNCTION(BlueprintCallable)
	bool QueryChestArmorInventory(FArmorInfo Item);



	UFUNCTION(BlueprintCallable)
	void Equip(FEquipmentInfo Tool);

	UFUNCTION(BlueprintCallable)
	void EquipArmor(FArmorInfo Tool);
	FArmorInfo EmptyArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables")
	TArray<ADefaultItem*> PickupableItems;






	// Player Stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float HungerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float MaxHunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float XP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	float XPNeededToLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	int Vigor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	int Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stats")
	int Endurance;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Hunger")
	float HungerClock = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Hunger")
	float HungerTimer = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Hunger")
	float StarveTick = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Hunger")
	float StarveClock = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Hunger")
	float StarveTimer = 1.2f;





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stamina")
	float StaminaTick = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stamina")
	float StaminaClock = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariables | Stamina")
	float StaminaTimer = 0.2f;




	void UpdateStats();

	void LoseHealth(float PureDamage);

	void UpdateHunger(float Delta);

	void GetXP(float Points);





	// Overlap Functions

	UFUNCTION()
	void OnOverlapPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	



};
