// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrushAsset.h"
#include "PaintingSubsystem.h"
#include "Components/SceneComponent.h"
#include "PaintingComponent.generated.h"

/*
 * Component prepared to paint with given brushes on specified Render Target
 * Remember to activate component to work!
 */
UCLASS( ClassGroup=(Painting), meta=(BlueprintSpawnableComponent) )
class PAINTINGSYSTEM_API UPaintingComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	/********
	 * default overrides
	 ********/

	UPaintingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual bool ShouldActivate() const override;
	
	/********
	 * Painting properties
	 ********/
public:
	// if true it deactivates right after activation and sending the painting data to paint subsystem 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseOnlyOnceAfterActivation = true;

	// radius in world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PaintRadius = 64.f;

	// color to be applied to brush
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DrawColor = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UBrushAsset> PaintingBrushAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* RenderTargetToDrawOn;

protected:
	FPaintingBrush PaintingBrushData;

private:
	FRenderTargetDrawPoint MostRecentDrawPoint;
	UPaintingSubsystem* PaintingSubsystem;
	
	/*
	 *Approximation
	 */
protected:

	// if true it deactivates right after activation and sending the painting data to paint subsystem 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = "!bUseOnlyOnceAfterActivation"))
	bool bTryApproximateInBetweenPoints = true;
	
	// if true it deactivates right after activation and sending the painting data to paint subsystem 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = "bTryApproximateInBetweenPoints"))
	float MaxLocalTextureDistanceToTryApproximate = 0.2;
	
	// multiplies the inbetween points between recent and target draw point 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = "bTryApproximateInBetweenPoints"))
	int ApproximationComplexity = 3;

	/********
	 * Painting 
	 ********/
	
public:	
	// Editor Tool To Show Some Kind Of Preview for set radius. Shows radius for FULL texture, not just visible part
	UFUNCTION(CallInEditor, BlueprintCallable)
	void ShowRadiusPreview() const;

	// get currently loaded painting brush settings
	UFUNCTION(BlueprintPure)
	FPaintingBrush GetPaintingBrushData();
protected:
	// Main paint method called every tick (until optimized)
	virtual void Paint();
	// function adding more points for smoother experience if needed
	void PostDrawApproximation(FRenderTargetDrawPoint JustDrawnPoint);
	virtual FRenderTargetDrawPoint CalculatePointToDraw();

private:
	UFUNCTION()
	virtual void ActivatePainting(UActorComponent* Component, bool bReset);
	UFUNCTION()
	virtual void DeactivatePainting(UActorComponent* Component);
};