// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PaintingTypes.h"
#include "PaintingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGSYSTEM_API UPaintingSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()


private:
	TArray<FRenderTargetDrawPoint> DrawPoints;
	
	TMap<UTextureRenderTarget2D*, TArray<FRenderTargetDrawPoint>> MappedDrawPoints;

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	virtual bool IsTickableInEditor() const override { return false; }
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return true; }

	UFUNCTION(BlueprintCallable)
	void RegisterPointToDraw(FRenderTargetDrawPoint NewPoint, UTextureRenderTarget2D* RenderTargetToDrawOn);


protected:
	
	void DrawPointOnCanvas(UCanvas* LocalCanvas, FVector2d LocalCanvasSize, FRenderTargetDrawPoint DrawPoint);
	
	void PaintCurrentStack();

};
