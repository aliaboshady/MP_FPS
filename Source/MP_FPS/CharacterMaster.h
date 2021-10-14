#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterMaster.generated.h"

class UCameraComponent;
class AWeaponMaster;

UCLASS()
class MP_FPS_API ACharacterMaster : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterMaster();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartSprint();
	void Client_StartSprint();
	void StartSprint();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopSprint();
	void Client_StopSprint();
	void StopSprint();

	void SpawnWeapons();

private:
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ThirdPersonMeshComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ArmsMeshComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TorsoMeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponMaster> Class_WeaponMaster;

	UPROPERTY(EditAnywhere, Replicated)
	AWeaponMaster* CurrentWeapon;

	UPROPERTY(EditAnywhere)
	float WalkSpeed;

	UPROPERTY(EditAnywhere)
	float SprintSpeed;

	UPROPERTY(Replicated, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	bool bIsSprinting;
};
