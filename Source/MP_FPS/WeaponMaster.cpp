#include "WeaponMaster.h"

AWeaponMaster::AWeaponMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SK_FP_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_FP_WeaponMesh"));
	RootComponent = SK_FP_WeaponMesh;

	SK_TP_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_TP_WeaponMesh"));
	SK_TP_WeaponMesh->SetupAttachment(RootComponent);
}

void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

