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
}

