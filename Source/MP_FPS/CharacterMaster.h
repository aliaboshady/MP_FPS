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

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

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

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartFireWeapon();
	void Client_StartFireWeapon();
	void StartFireWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopFireWeapon();
	void Client_StopFireWeapon();
	void StopFireWeapon();

	void SpawnWeapons();

private:
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
