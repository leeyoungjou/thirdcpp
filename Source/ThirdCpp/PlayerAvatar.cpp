#include "PlayerAvatar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "PlayerAvatarAnimInstance.h"
#include "GameFramework/PlayerController.h"


// Sets default values
APlayerAvatar::APlayerAvatar()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.f, 500.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ✅ Load Input Actions
	static ConstructorHelpers::FObjectFinder<UInputAction> MOVE_ACTION(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (MOVE_ACTION.Object) MoveAction = MOVE_ACTION.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ATTACK_ACTION(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Attack.IA_Attack'"));
	if (ATTACK_ACTION.Object) AttackAction = ATTACK_ACTION.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> LOOK_ACTION(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (LOOK_ACTION.Object) LookAction = LOOK_ACTION.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MOUSELOOK_ACTION(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MouseLook.IA_MouseLook'"));
	if (MOUSELOOK_ACTION.Object) MouseLookAction = MOUSELOOK_ACTION.Object;

	// ✅ Load Input Mapping Context (정상 위치)
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULT_CONTEXT(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (DEFAULT_CONTEXT.Succeeded())
	{
		DefaultMappingContext = DEFAULT_CONTEXT.Object;
	}
}

// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;
	auto animInst
		= Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->State = EPlayerState::Locomotion;

	// Enhanced Input Subsystem 등록
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
}

// Called every frame
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto animInst
		= Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->Speed = GetCharacterMovement()->Velocity.Size2D();
	FString SpeedMessage = FString::Printf(TEXT("Speed: %.2f"), animInst->Speed);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(95, 1.f, FColor::Red, SpeedMessage);
	}
	if (_AttackCountingDown == AttackInterval) {
		animInst->State = EPlayerState::Attack;
	}
	if (_AttackCountingDown > 0.0f) {
		_AttackCountingDown -= DeltaTime;
	}
}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerAvatar::Attack);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerAvatar::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerAvatar::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APlayerAvatar::MouseLook);
	}
}

int APlayerAvatar::GetHealthPoints()
{
	return _HealthPoints;
}

bool APlayerAvatar::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APlayerAvatar::CanAttack()
{
	auto animInst
		= Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	return (_AttackCountingDown <= 0.0f
		&& animInst->State == EPlayerState::Locomotion);
}

void APlayerAvatar::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerAvatar::Attack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		if (CanAttack()) _AttackCountingDown = AttackInterval; //타이머 설정
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(100, 1.f, FColor::Red, TEXT("ATTACK"));
		}
	}
}
void APlayerAvatar::DieProcess() {
	Destroy();
}

void APlayerAvatar::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerAvatar::MouseLook(const FInputActionValue& Value)
{
	Look(Value); // 동일하게 처리
}

void APlayerAvatar::TestHit() {
	auto animInst
		= Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	FString StateMessage = FString::Printf(TEXT("State: %d"), static_cast<int>(animInst->State));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(65, 2.f, FColor::Red, StateMessage);
	}
	if (_HealthPoints > 0 && animInst->State == EPlayerState::Locomotion) {
		animInst->State = EPlayerState::Hit;
		if (_HealthPoints >= 100) _HealthPoints -= 100;
		else _HealthPoints = 0;
	}
}

bool APlayerAvatar::IsAttacking() {
	auto animInst
		= Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	return (animInst->State == EPlayerState::Attack);
}