#include "CharacterMaster.h"
#include "WeaponMaster.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ACharacterMaster::ACharacterMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	SprintSpeed = 600;
	WalkSpeed = 180;
	bIsSprinting = false;

	ThirdPersonMeshComponent = GetMesh();
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	ArmsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMeshComponent"));
	ArmsMeshComponent->SetupAttachment(Camera);
	
	TorsoMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TorsoMeshComponent"));
	TorsoMeshComponent->SetupAttachment(RootComponent);
}

void ACharacterMaster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterMaster, bIsSprinting);
	DOREPLIFETIME(ACharacterMaster, CurrentWeapon);
}

void ACharacterMaster::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapons();
}

void ACharacterMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterMaster::Client_StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterMaster::Client_StopSprint);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &ACharacterMaster::Client_StartFireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", IE_Released, this, &ACharacterMaster::Client_StopFireWeapon);
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
	if(bIsSprinting)
	{
		if(Value <= 0) Client_StopSprint();
		else Client_StartSprint();
	}
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////--- Server Actions ---//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ACharacterMaster::Server_StartSprint_Implementation()
{
	if(GetLocalRole() == ROLE_AutonomousProxy) return;
	StartSprint();
}

bool ACharacterMaster::Server_StartSprint_Validate()
{
	return true;
}

void ACharacterMaster::Client_StartSprint()
{
	StartSprint();
	Server_StartSprint();
}

void ACharacterMaster::StartSprint()
{
	bIsSprinting = true;
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if(CharacterMovementComponent) CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
}

void ACharacterMaster::Server_StopSprint_Implementation()
{
	if(GetLocalRole() == ROLE_AutonomousProxy) return;
	StopSprint();
}

bool ACharacterMaster::Server_StopSprint_Validate()
{
	return true;
}

void ACharacterMaster::Client_StopSprint()
{
	StopSprint();
	Server_StopSprint();
}

void ACharacterMaster::StopSprint()
{
	bIsSprinting = false;
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if(CharacterMovementComponent) CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void ACharacterMaster::Server_StartFireWeapon_Implementation()
{
	if(GetLocalRole() == ROLE_AutonomousProxy) return;
	StartFireWeapon();
}

bool ACharacterMaster::Server_StartFireWeapon_Validate()
{
	return true;
}

void ACharacterMaster::Client_StartFireWeapon()
{
	StartFireWeapon();
	Server_StartFireWeapon();
}

void ACharacterMaster::StartFireWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Start Fire"));
	if(CurrentWeapon) CurrentWeapon->OnWeaponFire();
}

void ACharacterMaster::Server_StopFireWeapon_Implementation()
{
	if(GetLocalRole() == ROLE_AutonomousProxy) return;
	StopFireWeapon();
}

bool ACharacterMaster::Server_StopFireWeapon_Validate()
{
	return true;
}

void ACharacterMaster::Client_StopFireWeapon()
{
	StopFireWeapon();
	Server_StopFireWeapon();
}

void ACharacterMaster::StopFireWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Stop Fire"));
	if(CurrentWeapon) CurrentWeapon->OffWeaponFire();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////--- Functions ---///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void ACharacterMaster::SpawnWeapons()
{
	if(Class_WeaponMaster && HasAuthority())
	{
		FTransform SocketTransform = ArmsMeshComponent->GetSocketTransform(FName("WeaponSocket"));
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		CurrentWeapon = Cast<AWeaponMaster>(GetWorld()->SpawnActor(Class_WeaponMaster, &SocketTransform, SpawnParameters));
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->SetInstigator(this);
		CurrentWeapon->CharacterMaster = this;

		FAttachmentTransformRules AttachmentRules =  FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		CurrentWeapon->SK_FP_WeaponMesh->AttachToComponent(ArmsMeshComponent, AttachmentRules, FName("WeaponSocket"));
		CurrentWeapon->SK_TP_WeaponMesh->AttachToComponent(ThirdPersonMeshComponent, AttachmentRules, FName("WeaponSocket"));
	}
}

//UE_LOG(LogTemp, Warning, TEXT("Turn"));
