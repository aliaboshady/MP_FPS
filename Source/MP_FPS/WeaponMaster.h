#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMaster.generated.h"

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

protected:
	virtual void BeginPlay() override;

private:
};
