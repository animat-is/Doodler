// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PaintingSystemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PAINTINGSYSTEM_API UPaintingSystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static FVector2D RemapWorldLocationToRenderTargetLocalCoords(FVector WorldLocation, UStaticMeshComponent* MeshComponent);
};
