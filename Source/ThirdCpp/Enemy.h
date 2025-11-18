// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"
UCLASS()
class THIRDCPP_API AEnemy : public ACharacter {
	GENERATED_BODY()
public:
	AEnemy();
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	int HealthPoints = 100; //the character's max health points
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float Strength = 5; //the character's attack strength
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float Armor = 1;  //the character's defense armor
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float AttackRange = 200.0f; //the character's attack range
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float AttackInterval = 3.0f; //the character's attack invertal
protected:
	virtual void BeginPlay() override;
	int _HealthPoints;
	float _AttackCountingDown;
	APawn* _chasedTarget = nullptr;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent*
		PlayerInputComponent)
		override;
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy",
		meta = (DisplayName = "Get HP"))
	int GetHealthPoints(); //get current health points
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	bool IsKilled();  //check if the character has been killed
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	bool CanAttack();  //check if the character can attack
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	void Chase(APawn* targetPawn);
	void Attack();
	void Hit(int damage);
	void DieProcess();  //process when the character is killed
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;
};