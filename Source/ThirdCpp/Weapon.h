// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Weapon.generated.h"

UCLASS()
class THIRDCPP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACharacter* Holder = nullptr; // Holder: PlayerAvatar, Enemy
	UPROPERTY(EditAnywhere, Category = "Weapon Params")
	float Strength = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* _StaticMesh; // Static Mesh
	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	// OnActorBeginOverlap.AddDynamic(¡¦)
	// OnActorBeginOverlap: event delegator
	// OverlappedActor: overlapped (triggered) actor
	// OtherActor: target
	bool IsWithinAttackRange(float AttackRange, AActor* Target);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
