// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatarAnimInstance.h"
#include "PlayerAvatar.h" 
void UPlayerAvatarAnimInstance::OnStateAnimationEnds() {
    if (State == EPlayerState::Attack) {
        State = EPlayerState::Locomotion;
        // Attack animation에서OnStateAnimationEnds 호출을위한notify 지정하여 사용
    }
    else {
        auto playerAvatar = Cast<APlayerAvatar>(GetOwningActor());
        if (State == EPlayerState::Hit) {
            if (playerAvatar->GetHealthPoints() > 0.0f) {
                State = EPlayerState::Locomotion;
            }
            else {
                State = EPlayerState::Die;
            }
        }
        else if (State == EPlayerState::Die) {
            playerAvatar->DieProcess();
        }
    }
}
