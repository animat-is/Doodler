// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaintingComponent.h"
#include "MeshPaintingComponent.generated.h"

/*
 * Component prepared to paint on meshes assuming they use RenderTarget and are "canvas/plane" like mappable
 */
UCLASS( ClassGroup=(Painting), meta=(BlueprintSpawnableComponent) )
class PAINTINGSYSTEM_API UMeshPaintingComponent : public UPaintingComponent
{
	GENERATED_BODY()

	/*
	 * base overrides
	 */
public:
	UMeshPaintingComponent();
	
protected:
	virtual FRenderTargetDrawPoint CalculatePointToDraw() override;

	/*
	 * child properties
	 */
protected:
	// Mesh used to be painted on instead of world-like painting method from parent
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

public:
	// function to use intentionally one node for currently used mesh and to update world location too
	UFUNCTION(BlueprintCallable, Category = Painting)
	void PreparePaintingData(UStaticMeshComponent* PaintableMesh, FVector NewWorldLocation);

};
