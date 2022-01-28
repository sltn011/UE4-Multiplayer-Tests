// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/MenuPlayerPawn.h"

#include "Camera/CameraComponent.h"

AMenuPlayerPawn::AMenuPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SetRootComponent(CameraComponent);

}