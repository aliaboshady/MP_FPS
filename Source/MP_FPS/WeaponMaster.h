#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMaster.generated.h"

class ACharacterMaster;

UCLASS()
class MP_FPS_API AWeaponMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponMaster();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SK_FP_WeaponMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SK_TP_WeaponMesh;

	void OnWeaponFire();
	void OffWeaponFire();
	
	UPROPERTY(Replicated)
	ACharacterMaster* CharacterMaster;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ShootInfo(FHitResult ScreenHit, FHitResult MuzzleHit);
	void Client_ShootInfo(FHitResult ScreenHit, FHitResult MuzzleHit);
	void ShootInfo(FHitResult ScreenHit, FHitResult MuzzleHit);

private:
	UPROPERTY(EditAnywhere)
	float FireDistance;

	UPROPERTY(EditAnywhere)
	float BulletRadius;
};
