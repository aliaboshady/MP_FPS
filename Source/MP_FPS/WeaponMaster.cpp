#include "WeaponMaster.h"
#include "CharacterMaster.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

AWeaponMaster::AWeaponMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	FireDistance = 10000;
	BulletRadius = 5;

	SK_FP_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_FP_WeaponMesh"));
	RootComponent = SK_FP_WeaponMesh;

	SK_TP_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_TP_WeaponMesh"));
	SK_TP_WeaponMesh->SetupAttachment(RootComponent);
}

void AWeaponMaster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponMaster, CharacterMaster);
}

void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponMaster::OnWeaponFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Fire"));
	if(!CharacterMaster) return;

	FVector Location_Start = CharacterMaster->Camera->GetComponentLocation();
	FVector Location_End = Location_Start + CharacterMaster->Camera->GetComponentRotation().Vector() * FireDistance;
	AActor* CharacterActor = Cast<AActor>(CharacterMaster);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CharacterActor);
	FHitResult OutHit;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Location_Start, Location_End, BulletRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true);

	FVector Trace_Start = SK_FP_WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
	FVector Trace_End = Location_End;
	FHitResult TraceHit;
	if(OutHit.bBlockingHit) Trace_End = OutHit.ImpactPoint;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Trace_Start, Trace_End, BulletRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, TraceHit, true, FLinearColor::Yellow, FLinearColor::Blue);
	Client_ShootInfo(OutHit, TraceHit);
}

void AWeaponMaster::OffWeaponFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Fire"));
}

void AWeaponMaster::Server_ShootInfo_Implementation(FHitResult ScreenHit, FHitResult MuzzleHit)
{
}

bool AWeaponMaster::Server_ShootInfo_Validate(FHitResult ScreenHit, FHitResult MuzzleHit)
{
	return true;
}

void AWeaponMaster::Client_ShootInfo(FHitResult ScreenHit, FHitResult MuzzleHit)
{
}

void AWeaponMaster::ShootInfo(FHitResult ScreenHit, FHitResult MuzzleHit)
{
}
