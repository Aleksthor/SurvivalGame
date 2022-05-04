// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "DefaultTool.h"
#include "DefaultArmor.h"
#include "Chest.h"

#include "InventoryComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASurvivalCharacter::ASurvivalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Spring Arm and attach it to the Root(CapsuleComponent)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f; // Distance from player
	SpringArm->bUsePawnControlRotation = true; // Rotate arm based on controller

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("PickupColldier"));
	PickupCollider->SetupAttachment(GetRootComponent());

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Allows the Character to stand still and face same direction 
	// Do not rotate when Controller does
	// That only affects the Camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bisRun = false;
	bisWalk = false;
	bisHoldSpace = false;
	bisLocomotion = true;
	bisSnapRotation = false;
	bisPickup = false;
	bisAttack = false;

	Speed_Run = 450.f;
	Speed_Jog = 350.f;
	Speed_Walk = 100.f;


	MaxHealth = 100.f;
	Health = MaxHealth;

	bChestOpen = false;

	EmptyArmor.Name = "";
	EmptyArmor.Category = EItemType::EIT_Armor;
	EmptyArmor.ArmorType = EArmorType::EAT_MAX;
	EmptyArmor.MovementSpeedPercent = 0.f;
	EmptyArmor.DamageNegation = 0.f;



	// Player Base Stats

	Health = 100.f;
	MaxHealth = 100.f;
	Hunger = 50.f;
	HungerTick = 1.f;
	MaxHunger = 50.f;
	Stamina = 50.f;
	MaxStamina = 50.f;
	XP = 0.f;
	XPNeededToLevel = 100.f;
	Level = 1;
	Vigor = 1;
	Strength = 1;
	Endurance = 1;


	runTimer = 0;
	pre_runTimer = 0;


	GetCharacterMovement()->JumpZVelocity = 700.f;
}

// Called when the game starts or when spawned
void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = Speed_Jog;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &ASurvivalCharacter::OnOverlapPickup);
	PickupCollider->OnComponentEndOverlap.AddDynamic(this, &ASurvivalCharacter::OnOverlapEndPickup);

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "DisableAllScreenMessages");

	

}

// Called every frame
void ASurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	// Fix Some Movement in Air
	if (GetCharacterMovement()->IsFalling())
	{

		GetCharacterMovement()->bOrientRotationToMovement = false;

	}
	else
	{

		GetCharacterMovement()->bOrientRotationToMovement = true;

	}

	// Check if SnapRotation is on - if so run function
	if (bisSnapRotation)
	{

		SnapRotationFn();

	}

	// Handle Movement Speed and Rotation Rate
	RunorWalk();

	if (Retrigger)
	{
		RetriggerClock += DeltaTime;

		if (RetriggerClock > RetriggerDelay)
		{
			Retrigger = false;
			bisSnapRotation = false;
			RetriggerClock = 0.f;
		}
	}

	if (PickupableItems.Num() == 0)
	{
		SetActiveOverlappingItem(nullptr);
	}

	UpdateStats();
	UpdateHunger(DeltaTime);


}

// Called to bind functionality to input
void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Moving with WASD
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalCharacter::MoveRight);

	// Turn Camera with mouse
	PlayerInputComponent->BindAxis("TurnCamera", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpCamera", this, &APawn::AddControllerPitchInput);

	// Use
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ASurvivalCharacter::Pickup);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASurvivalCharacter::Attack);

	// Sprint
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASurvivalCharacter::SprintPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASurvivalCharacter::SprintReleased);


	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
}

// Input Functions

void ASurvivalCharacter::MoveForward(float Value)
{
	inputY = Value;

	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= inputY;
	RightVector *= inputX;

	if (!FMath::IsNearlyZero(Value) && bisLocomotion)
	{
		AddMovementInput(ForwardVector);
	}
	// Set inputVector_World
	inputVector_World = ((ForwardVector * inputY) + (RightVector * inputX));


	// Set Local X and Y inputs
	FTransform Transform = GetActorTransform();
	FVector LocalXYVector = UKismetMathLibrary::InverseTransformDirection(Transform, inputVector_World);

	inputX_Local = LocalXYVector.Y;
	inputY_Local = LocalXYVector.X;

}

void ASurvivalCharacter::MoveRight(float Value)
{
	inputX = Value;


	FRotator ControlRotation = Controller->GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

	ForwardVector *= inputY;
	RightVector *= inputX;

	if (!FMath::IsNearlyZero(Value) && bisLocomotion)
	{
		AddMovementInput(RightVector);
	}



	// Set inputVector_World
	inputVector_World = ((ForwardVector)+(RightVector));

	// Set Local X and Y inputs
	FTransform Transform = GetActorTransform();
	FVector LocalXYVector = UKismetMathLibrary::InverseTransformDirection(Transform, inputVector_World);

	inputX_Local = LocalXYVector.Y;
	inputY_Local = LocalXYVector.X;

}

void ASurvivalCharacter::Pickup()
{
	if (ActiveOverlappingChest)
	{
		bChestOpen = true;

		ChestInventory.ArmorList.Empty();
		ChestInventory.EquipmentList.Empty();
		ChestInventory.ItemList.Empty();

		for (int i{}; i < ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList.Num(); i++)
		{

			ChestInventory.ArmorList.Add(ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList[i]);

		}

		for (int i{}; i < ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList.Num(); i++)
		{

			ChestInventory.EquipmentList.Add(ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList[i]);

		}

		for (int i{}; i < ActiveOverlappingChest->InventoryComponent->Inventory.ItemList.Num(); i++)
		{

			ChestInventory.ItemList.Add(ActiveOverlappingChest->InventoryComponent->Inventory.ItemList[i]);

		}
		
	}
	else
	{
		if (ActiveOverlappingItem)
		{
			bisPickup = true;
			bool isAlreadyPickedUp = false;
			ADefaultTool* Tool = Cast<ADefaultTool>(ActiveOverlappingItem);
			if (Tool)
			{
				if (Tool->EquipmentInfo.Category == EItemType::EIT_Equipment)
				{
					Tool->PickupEquipment(this);
				}
				isAlreadyPickedUp = true;

			}
			ADefaultArmor* Armor = Cast<ADefaultArmor>(ActiveOverlappingItem);
			if (Armor && !isAlreadyPickedUp)
			{

				if (Armor->ArmorInfo.Category == EItemType::EIT_Armor)
				{
					Armor->PickupArmor(this);
				}

				isAlreadyPickedUp = true;

			}

			ADefaultItem* Item = Cast<ADefaultItem>(ActiveOverlappingItem);
			if (Item && !isAlreadyPickedUp)
			{
				Item->Pickup(this);
				Item->Destroy();

			}


			TryGetNewOverlappingItem();


		}
	}

}

void ASurvivalCharacter::Attack()
{
	if (EquippedItem)
	{
		bisAttack = true;
	}
	
}

void ASurvivalCharacter::SprintPressed()
{
	bisHoldSpace = true;
}

void ASurvivalCharacter::SprintReleased()
{
	bisHoldSpace = false;

	pre_runTimer = 0;

	runTimer = 0;

	StaminaClock = 0.f;
	
}

// Tick

void ASurvivalCharacter::SnapRotationFn()
{
	FRotator ControlRotation = GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	float DeltaSeconds{};
	UWorld* World = GetWorld();
	if (World)
	{

		DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

	}

	float Alpha = FMath::FInterpTo(0.f, 1.f, DeltaSeconds, 20.f);

	FRotator NewRotation = UKismetMathLibrary::RLerp(GetActorRotation(), ControlRotation, Alpha, true);

	SetActorRotation(NewRotation);
}

void ASurvivalCharacter::RunorWalk()
{

	if (bisHoldSpace && bisLocomotion && Stamina > 0.f)
	{
		pre_runTimer++;

		if (pre_runTimer > 20)
		{

			bisRun = true;


			float MaxSpeedTemp = GetCharacterMovement()->MaxWalkSpeed;


			GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
			bIsTargetLock = false;

			float DeltaSeconds{};
			UWorld* World = GetWorld();
			if (World)
			{

				DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

			}

			float ReturnValue = FMath::FInterpTo(MaxSpeedTemp, Speed_Run, DeltaSeconds, 10.f);


			GetCharacterMovement()->MaxWalkSpeed = ReturnValue;

			runTimer++;

			UE_LOG(LogTemp, Warning, TEXT("State: Running"));


			// Stamina 

			StaminaClock += DeltaSeconds;

			if (StaminaClock > StaminaTimer)
			{
				StaminaClock = 0.f;
				Stamina -= StaminaTick;
				Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
				Stamina = FMath::RoundHalfFromZero(Stamina);
			}

		}
	}
	else
	{
		if (bisWalk)
		{

			float MaxSpeedTemp = GetCharacterMovement()->MaxWalkSpeed;


			float DeltaSeconds{};
			UWorld* World = GetWorld();
			if (World)
			{

				DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

			}

			float ReturnValue = FMath::FInterpTo(MaxSpeedTemp, Speed_Walk, DeltaSeconds, 10.f);


			GetCharacterMovement()->MaxWalkSpeed = ReturnValue;

			// Stamina 

			StaminaClock += DeltaSeconds;

			if (StaminaClock > StaminaTimer)
			{
				StaminaClock = 0.f;
				Stamina += StaminaTick/4.f;
				Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
				Stamina = FMath::RoundHalfFromZero(Stamina);
			}


		}
		else
		{
			float MaxSpeedTemp = GetCharacterMovement()->MaxWalkSpeed;


			float DeltaSeconds{};
			UWorld* World = GetWorld();
			if (World)
			{

				DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(World);

			}

			float ReturnValue = FMath::FInterpTo(MaxSpeedTemp, Speed_Jog, DeltaSeconds, 10.f);


			GetCharacterMovement()->MaxWalkSpeed = ReturnValue;


			// Stamina 

			StaminaClock += DeltaSeconds;

			if (StaminaClock > StaminaTimer)
			{
				StaminaClock = 0.f;
				Stamina += StaminaTick / 2.f;
				Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
				Stamina = FMath::RoundHalfFromZero(Stamina);
			}


		}
	}
}

void ASurvivalCharacter::SnapRotation()
{
	bisSnapRotation = true;
	Retrigger = true;
	RetriggerClock = 0.f;
}

// Reset Functions used in AnimationBp

void ASurvivalCharacter::ResetAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Attack"));
	bisAttack = false;
}
void ASurvivalCharacter::SetHitBox()
{
	if (EquippedItem)
	{
		Cast<ADefaultTool>(EquippedItem)->HitBox->SetGenerateOverlapEvents(true);
	}
	
}

void ASurvivalCharacter::ResetHitBox()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset HitBox"));
	if (EquippedItem)
	{
		Cast<ADefaultTool>(EquippedItem)->HitBox->SetGenerateOverlapEvents(false);
	}
}

void ASurvivalCharacter::ResetPickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Pickup"));
	bisPickup = false;
}




// Inventory

	// ADD TO INVENTORY

void ASurvivalCharacter::AddToInventory(FItemInfo Item)
{
	Item.inInventory = true;
	Inventory.ItemList.Add(Item);
	
}

void ASurvivalCharacter::AddToEquipmentInventory(FEquipmentInfo Equipment)
{
	Equipment.inInventory = true;
	Inventory.EquipmentList.Add(Equipment);
	
}

void ASurvivalCharacter::AddToArmorInventory(FArmorInfo Item)
{
	Item.inInventory = true;
	Inventory.ArmorList.Add(Item);
}

	// REMOVE FROM INVENTORY

void ASurvivalCharacter::RemoveFromInventory(FItemInfo Item, int Quantity)
{

	if (QueryInventory(Item, Quantity))
	{
		int Find{};
		if (Inventory.ItemList.Num() > 0)
		{
			for (int i{}; i < Inventory.ItemList.Num(); i++)
			{
				if (Inventory.ItemList[i].Name == Item.Name)
				{
					Find = i;
				}
			}
			Inventory.ItemList[Find].StackAmount -= Quantity;

			if (Inventory.ItemList[Find].StackAmount == 0)
			{
				Inventory.ItemList.RemoveAt(Find);
			}
		}
	}
}


void ASurvivalCharacter::RemoveFromEquipmentInventory(FEquipmentInfo Equipment)
{
	if (QueryEquipmentInventory(Equipment))
	{
		int Find{};
		if (Inventory.EquipmentList.Num() > 0)
		{
			for (int i{}; i < Inventory.EquipmentList.Num(); i++)
			{
				if (Inventory.EquipmentList[i].Name == Equipment.Name)
				{
					Find = i;
				}
			}
			Inventory.EquipmentList.RemoveAt(Find);
		}
		
	}
}

void ASurvivalCharacter::RemoveFromArmorInventory(FArmorInfo Armor)
{
	if (QueryArmorInventory(Armor))
	{
		int Find{};
		if (Inventory.ArmorList.Num() > 0)
		{
			for (int i{}; i < Inventory.ArmorList.Num(); i++)
			{
				if (Inventory.ArmorList[i].Name == Armor.Name)
				{
					Find = i;
				}
			}
			Inventory.ArmorList.RemoveAt(Find);
		}

	}
}

	// QUERY INVENTORY

bool ASurvivalCharacter::QueryInventory(FItemInfo Item, int Quantity)
{
	int Find{};
	if (Inventory.ItemList.Num() > 0)
	{
		for (int i{}; i < Inventory.ItemList.Num(); i++)
		{
			if (Inventory.ItemList[i].Name == Item.Name)
			{
				Find = i;
			}
		}
		if (Inventory.ItemList[Find].StackAmount >= Quantity)
		{

			return true;
		}
	}


	return false;

}

bool ASurvivalCharacter::QueryEquipmentInventory(FEquipmentInfo Equipment)
{
	if (Inventory.EquipmentList.Num() > 0)
	{
		for (int i{}; i < Inventory.EquipmentList.Num(); i++)
		{
			if (Inventory.ItemList[i].Name == Equipment.Name)
			{
				return true;
			}
		}
		
	}
	return false;
}

bool ASurvivalCharacter::QueryArmorInventory(FArmorInfo Item)
{
	if (Inventory.ArmorList.Num() > 0)
	{
		for (int i{}; i < Inventory.ArmorList.Num(); i++)
		{
			if (Inventory.ArmorList[i].Name == Item.Name)
			{
				return true;
			}
		}
	}
	return false;
}


	// ADD TO CHEST INVENTORY

void ASurvivalCharacter::AddItemToChestInventory(FItemInfo Item)
{
	Item.inInventory = false;
	ChestInventory.ItemList.Add(Item);

	if (ActiveOverlappingChest)
	{
		ActiveOverlappingChest->InventoryComponent->Inventory.ItemList.Add(Item);
	}

}

void ASurvivalCharacter::AddEquipmentToChestInventory(FEquipmentInfo Equipment)
{
	Equipment.inInventory = false;
	ChestInventory.EquipmentList.Add(Equipment);

	if (ActiveOverlappingChest)
	{
		ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList.Add(Equipment);
	}
}

void ASurvivalCharacter::AddArmorToChestInventory(FArmorInfo Armor)
{
	Armor.inInventory = false;
	ChestInventory.ArmorList.Add(Armor);

	if (ActiveOverlappingChest)
	{
		ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList.Add(Armor);
	}
}

	// REMOVE FROM CHEST INVENTORY

void ASurvivalCharacter::RemoveItemFromChestInventory(FItemInfo Item, int Quantity)
{
	if (ActiveOverlappingChest)
	{
		if (ActiveOverlappingChest->InventoryComponent->Inventory.ItemList.Num() > 0)
		{
			for (int i{}; i < ActiveOverlappingChest->InventoryComponent->Inventory.ItemList.Num(); i++)
			{
				if (ActiveOverlappingChest->InventoryComponent->Inventory.ItemList[i].Name == Item.Name)
				{
					ActiveOverlappingChest->InventoryComponent->Inventory.ItemList.RemoveAt(i);
				}
			}
		}
	}

	if (QueryChestItemInventory(Item, Quantity))
	{
		int Find{};
		if (ChestInventory.ItemList.Num() > 0)
		{
			for (int i{}; i < ChestInventory.ItemList.Num(); i++)
			{
				if (ChestInventory.ItemList[i].Name == Item.Name)
				{
					Find = i;
				}
			}
			ChestInventory.ItemList[Find].StackAmount -= Quantity;

			if (ChestInventory.ItemList[Find].StackAmount == 0)
			{
				ChestInventory.ItemList.RemoveAt(Find);
			}
		}
	
	}

}

void ASurvivalCharacter::RemoveEquipmentFromChestInventory(FEquipmentInfo Equipment)
{
	if (ActiveOverlappingChest)
	{
		if (ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList.Num() > 0)
		{
			for (int i{}; i < ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList.Num(); i++)
			{
				if (ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList[i].Name == Equipment.Name)
				{
					ActiveOverlappingChest->InventoryComponent->Inventory.EquipmentList.RemoveAt(i);
				}
			}
		}
	}
	if (QueryEquipmentInventory(Equipment))
	{
		int Find{};
		if (ChestInventory.EquipmentList.Num() > 0)
		{
			for (int i{}; i < ChestInventory.EquipmentList.Num(); i++)
			{
				if (ChestInventory.EquipmentList[i].Name == Equipment.Name)
				{
					Find = i;
				}
			}
			ChestInventory.EquipmentList.RemoveAt(Find);
		}

	}
}

void ASurvivalCharacter::RemoveArmorFromChestInventory(FArmorInfo Armor)
{
	if (ActiveOverlappingChest)
	{
		if (ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList.Num() > 0)
		{
			for (int i{}; i < ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList.Num(); i++)
			{
				if (ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList[i].Name == Armor.Name)
				{
					ActiveOverlappingChest->InventoryComponent->Inventory.ArmorList.RemoveAt(i);
				}
			}
		}
	}
	if (QueryArmorInventory(Armor))
	{
		int Find{};
		if (ChestInventory.ArmorList.Num() > 0)
		{
			for (int i{}; i < ChestInventory.ArmorList.Num(); i++)
			{
				if (ChestInventory.ArmorList[i].Name == Armor.Name)
				{
					Find = i;
				}
			}
			ChestInventory.ArmorList.RemoveAt(Find);
		}

	}
}

	// QUERY CHEST INVENTORY

bool ASurvivalCharacter::QueryChestItemInventory(FItemInfo Item, int Quantity)
{
	int Find{};
	if (ChestInventory.ItemList.Num() > 0)
	{
		for (int i{}; i < ChestInventory.ItemList.Num(); i++)
		{
			if (ChestInventory.ItemList[i].Name == Item.Name)
			{
				Find = i;
			}
		}
		if (ChestInventory.ItemList[Find].StackAmount >= Quantity)
		{

			return true;
		}
	}


	return false;
	
}

bool ASurvivalCharacter::QueryChestEquipmentInventory(FEquipmentInfo Equipment)
{
	if (ChestInventory.EquipmentList.Num() > 0)
	{
		for (int i{}; i < ChestInventory.EquipmentList.Num(); i++)
		{
			if (ChestInventory.EquipmentList[i].Name == Equipment.Name)
			{
				return true;
			}
		}
	}


	return false;
}

bool ASurvivalCharacter::QueryChestArmorInventory(FArmorInfo Item)
{
	if (ChestInventory.ArmorList.Num() > 0)
	{
		for (int i{}; i < ChestInventory.ArmorList.Num(); i++)
		{
			if (ChestInventory.ArmorList[i].Name == Item.Name)
			{
				return true;
			}
		}
	}
	return false;
}



void ASurvivalCharacter::Equip(FEquipmentInfo Tool)
{
	
	if (EquippedItem)
	{
		if (Tool.Name == Cast<ADefaultTool>(EquippedItem)->EquipmentInfo.Name)
		{
			EquippedItem->Destroy();
			SetEquippedItem(nullptr);
		}
		else
		{

			ADefaultTool* ToolSpawning = GetWorld()->SpawnActor<ADefaultTool>(Tool.ToolBP, GetActorLocation(), FRotator(0.f, 0.f, 0.f));
			if (ToolSpawning)
			{
				ToolSpawning->PickupCollider->SetGenerateOverlapEvents(false);

				ToolSpawning->Equipped = true;
				EquipmentType = ToolSpawning->EquipmentInfo.EquipmentType;

				const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");
				if (RightHandSocket)
				{
					RightHandSocket->AttachActor(ToolSpawning, GetMesh());
					SetEquippedItem(ToolSpawning);

				}

			}
		}


	}
	else
	{

		ADefaultTool* ToolSpawning = GetWorld()->SpawnActor<ADefaultTool>(Tool.ToolBP, GetActorLocation(), FRotator(0.f, 0.f, 0.f));
		if (ToolSpawning)
		{
			ToolSpawning->PickupCollider->SetGenerateOverlapEvents(false);

			ToolSpawning->Equipped = true;
			EquipmentType = ToolSpawning->EquipmentInfo.EquipmentType;

			const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");
			if (RightHandSocket)
			{
				RightHandSocket->AttachActor(ToolSpawning, GetMesh());
				SetEquippedItem(ToolSpawning);

			}

		}
	}
	

}

void ASurvivalCharacter::EquipArmor(FArmorInfo Armor)
{
	switch (Armor.ArmorType)
	{
	case EArmorType::EAT_Head:
		if (EquippedHeadPiece.Name == Armor.Name)
		{
			SetEquippedHeadPiece(EmptyArmor);
		}
		else
		{
			SetEquippedHeadPiece(Armor);
		}
		break;
	case EArmorType::EAT_Chest:
		if (EquippedChestPiece.Name == Armor.Name)
		{
			SetEquippedChestPiece(EmptyArmor);
		}
		else
		{
			SetEquippedChestPiece(Armor);
		}
		break;
	case EArmorType::EAT_Pants:
		if (EquippedPants.Name == Armor.Name)
		{
			SetEquippedPants(EmptyArmor);
		}
		else
		{
			SetEquippedPants(Armor);
		}
		break;
	case EArmorType::EAT_Boots:
		if (EquippedBoots.Name == Armor.Name)
		{
			SetEquippedBoots(EmptyArmor);
		}
		else
		{
			SetEquippedBoots(Armor);
		}
		break;
	case EArmorType::EAT_Belt:
		if (EquippedBelt.Name == Armor.Name)
		{
			SetEquippedBelt(EmptyArmor);
		}
		else
		{
			SetEquippedBelt(Armor);
		}
		break;
	case EArmorType::EAT_Ring:
		if (EquippedRing.Name == Armor.Name)
		{
			SetEquippedRing(EmptyArmor);
		}
		else
		{
			SetEquippedRing(Armor);
		}
		break;
	case EArmorType::EAT_EarRing:
		if (EquippedEarRing.Name == Armor.Name)
		{
			SetEquippedEarRing(EmptyArmor);
		}
		else
		{
			SetEquippedEarRing(Armor);
		}
		break;


	default:
		break;
	}
}

void ASurvivalCharacter::SetEquippedItem(ADefaultItem* ItemToSet)
{
	if (EquippedItem)
	{
		EquippedItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedItem->Destroy();
	}
	EquippedItem = ItemToSet;
}


void ASurvivalCharacter::SetEquippedHeadPiece(FArmorInfo ItemToSet)
{

	EquippedHeadPiece = ItemToSet;
}

void ASurvivalCharacter::SetEquippedChestPiece(FArmorInfo ItemToSet)
{
	
	EquippedChestPiece = ItemToSet;

}

void ASurvivalCharacter::SetEquippedPants(FArmorInfo ItemToSet)
{
	EquippedPants = ItemToSet;
}

void ASurvivalCharacter::SetEquippedBoots(FArmorInfo ItemToSet)
{

	EquippedBoots = ItemToSet;
}

void ASurvivalCharacter::SetEquippedBelt(FArmorInfo ItemToSet)
{

	EquippedBelt = ItemToSet;
}

void ASurvivalCharacter::SetEquippedRing(FArmorInfo ItemToSet)
{

	EquippedRing = ItemToSet;
}



void ASurvivalCharacter::SetEquippedEarRing(FArmorInfo ItemToSet)
{
	
	EquippedEarRing = ItemToSet;
}

void ASurvivalCharacter::TryGetNewOverlappingItem()
{

	if (PickupableItems.Num() > 0)
	{
		for (int i{}; i < PickupableItems.Num(); i++)
		{
			SetActiveOverlappingItem(PickupableItems[i]);
		}
	}
	else
	{
		SetActiveOverlappingItem(nullptr);
	}
}

void ASurvivalCharacter::UpdateStats()
{
	Speed_Jog = 350.f + (350.f * (EquippedHeadPiece.MovementSpeedPercent + EquippedChestPiece.MovementSpeedPercent + EquippedPants.MovementSpeedPercent
		+ EquippedBoots.MovementSpeedPercent + EquippedBelt.MovementSpeedPercent + EquippedEarRing.MovementSpeedPercent + EquippedRing.MovementSpeedPercent) / 100.f);

	Speed_Run = 450.f + (450.f * (EquippedHeadPiece.MovementSpeedPercent + EquippedChestPiece.MovementSpeedPercent + EquippedPants.MovementSpeedPercent
		+ EquippedBoots.MovementSpeedPercent + EquippedBelt.MovementSpeedPercent + EquippedEarRing.MovementSpeedPercent + EquippedRing.MovementSpeedPercent) / 100.f);

	Vigor = FMath::Clamp(Vigor, 0, 20);
	switch(Vigor)
	{
	case 1:
		MaxHealth = 100.f;
		break;
	case 2:
		MaxHealth = 125.f;
		break;
	case 3:
		MaxHealth = 150.f;
		break;
	case 4:
		MaxHealth = 173.f;
		break;
	case 5:
		MaxHealth = 194.f;
		break;
	case 6:
		MaxHealth = 214.f;
		break;
	case 7:
		MaxHealth = 233.f;
		break;
	case 8:
		MaxHealth = 250.f;
		break;
	case 9:
		MaxHealth = 265.f;
		break;
	case 10:
		MaxHealth = 280.f;
		break;
	case 11:
		MaxHealth = 288.f;
		break;
	case 12:
		MaxHealth = 296.f;
		break;
	case 13:
		MaxHealth = 304.f;
		break;
	case 14:
		MaxHealth = 311.f;
		break;
	case 15:
		MaxHealth = 318.f;
		break;
	case 16:
		MaxHealth = 321.f;
		break;
	case 17:
		MaxHealth = 324.f;
		break;
	case 18:
		MaxHealth = 326.f;
		break;
	case 19:
		MaxHealth = 328.f;
		break;
	case 20:
		MaxHealth = 330.f;
		break;
	default:
		break;
	}
	Endurance = FMath::Clamp(Endurance, 0, 20);
	switch (Endurance)
	{
	case 1:
		MaxStamina = 50.f;
		MaxHunger = 50.f;
		break;
	case 2:
		MaxStamina = 55.f;
		MaxHunger = 50.f;
		break;
	case 3:
		MaxStamina = 60.f;
		MaxHunger = 50.f;
		break;
	case 4:
		MaxStamina = 65.f;
		MaxHunger = 50.f;
		break;
	case 5:
		MaxStamina = 70.f;
		MaxHunger = 50.f;
		break;
	case 6:
		MaxStamina = 74.f;
		MaxHunger = 52.f;
		break;
	case 7:
		MaxStamina = 78.f;
		MaxHunger = 54.f;
		break;
	case 8:
		MaxStamina = 82.f;
		MaxHunger = 56.f;
		break;
	case 9:
		MaxStamina = 86.f;
		MaxHunger = 58.f;
		break;
	case 10:
		MaxStamina = 90.f;
		MaxHunger = 60.f;
		break;
	case 11:
		MaxStamina = 93.f;
		MaxHunger = 63.f;
		break;
	case 12:
		MaxStamina = 96.f;
		MaxHunger = 66.f;
		break;
	case 13:
		MaxStamina = 99.f;
		MaxHunger = 70.f;
		break;
	case 14:
		MaxStamina = 101.f;
		MaxHunger = 74.f;
		break;
	case 15:
		MaxStamina = 104.f;
		MaxHunger = 78.f;
		break;
	case 16:
		MaxStamina = 106.f;
		MaxHunger = 81.f;
		break;
	case 17:
		MaxStamina = 107.f;
		MaxHunger = 84.f;
		break;
	case 18:
		MaxStamina = 108.f;
		MaxHunger = 86.f;
		break;
	case 19:
		MaxStamina = 109.f;
		MaxHunger = 88.f;
		break;
	case 20:
		MaxStamina = 110.f;
		MaxHunger = 90.f;
		break;
	default:
		break;
	}
	
	if (XP > XPNeededToLevel)
	{
		XP = 0.f;
		Level++;
	}
}

void ASurvivalCharacter::LoseHealth(float PureDamage)
{
	float Damage = PureDamage - PureDamage * 2 * FMath::Clamp((EquippedHeadPiece.DamageNegation + EquippedChestPiece.DamageNegation + EquippedPants.DamageNegation
		+ EquippedBoots.DamageNegation + EquippedBelt.DamageNegation + EquippedEarRing.DamageNegation + EquippedRing.DamageNegation) / 100.f, 0.f,1.f);


	Damage = FMath::Floor(Damage);
	Health -= Damage;

}

void ASurvivalCharacter::UpdateHunger(float Delta)
{

	Hunger = FMath::Clamp(Hunger, 0.f, MaxHunger);

	HungerClock += Delta;
	if (HungerClock > HungerTimer)
	{
		HungerClock = 0.f;
		HungerTimer = FMath::FRandRange(10.f, 15.f);
		Hunger -= HungerTick;
	}
	if (FMath::IsNearlyZero(Hunger))
	{
		isStarve = true;
		StarveClock += Delta;
		if (StarveClock > StarveTimer)
		{
			StarveClock = 0.f;
			Health -= StarveTick;
		}
	}
	else
	{
		isStarve = false;
	}
	
}

void ASurvivalCharacter::GetXP(float Points)
{
	XP += Points;
}

void ASurvivalCharacter::OnOverlapPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	ADefaultItem* Item = Cast<ADefaultItem>(OtherActor);
	if (Item)
	{
		if (OtherComponent == Item->PickupCollider)
		{
			
			ADefaultTool* Tool = Cast<ADefaultTool>(Item);
			if (Tool && !Tool->Equipped)
			{
				
				SetActiveOverlappingItem(Tool);
				PickupableItems.Add(Tool);
			}
			else
			{
				ADefaultArmor* Armor = Cast<ADefaultArmor>(Item);
				if (Armor)
				{
					SetActiveOverlappingItem(Armor);
					PickupableItems.Add(Armor);
				}
				else
				{
				
					SetActiveOverlappingItem(Item);
					PickupableItems.Add(Item);
				}
			
			}

		}
	}

	AChest* Chest = Cast<AChest>(OtherActor);
	if (Chest)
	{
		if (OtherComponent == Chest->PickupCollider)
		{
			SetActiveOverlappingChest(Chest);
		}

	}

}

void ASurvivalCharacter::OnOverlapEndPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ADefaultItem* Item = Cast<ADefaultItem>(OtherActor);
	if (Item)
	{
		if (OtherComponent == Item->PickupCollider )
		{ 
			if (PickupableItems.Num() > 0)
			{
				for (int i{}; i < PickupableItems.Num(); i++)
				{
					if (PickupableItems[i] == Item)
					{
						PickupableItems.RemoveAt(i);
					}
					else
					{
						SetActiveOverlappingItem(PickupableItems[i]);
					}
					
				}
			}
			else
			{
				SetActiveOverlappingItem(nullptr);
			}
		}
	}

	AChest* Chest = Cast<AChest>(OtherActor);
	if (Chest)
	{
		if (OtherComponent == Chest->PickupCollider)
		{
			ChestInventory.ArmorList.Empty();
			ChestInventory.EquipmentList.Empty();
			ChestInventory.ItemList.Empty();
			SetActiveOverlappingChest(nullptr);
		}
	
	
	}

}


