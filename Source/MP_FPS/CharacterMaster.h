#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterMaster.generated.h"

class UCameraComponent;

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

private:
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ThirdPersonMeshComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ArmsMeshComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TorsoMeshComponent;
};
