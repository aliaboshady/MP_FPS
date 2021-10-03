#include "CharacterMaster.h"
#include "Camera/CameraComponent.h"

ACharacterMaster::ACharacterMaster()
{
	PrimaryActorTick.bCanEverTick = true;

	ThirdPersonMeshComponent = GetMesh();
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	ArmsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMeshComponent"));
	ArmsMeshComponent->SetupAttachment(Camera);
	
	TorsoMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TorsoMeshComponent"));
	TorsoMeshComponent->SetupAttachment(RootComponent);
}

void ACharacterMaster::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterMaster::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterMaster::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterMaster::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterMaster::LookUp);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////--- MOVEMENT ---////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void ACharacterMaster::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ACharacterMaster::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ACharacterMaster::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ACharacterMaster::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


//UE_LOG(LogTemp, Warning, TEXT("Turn"));
