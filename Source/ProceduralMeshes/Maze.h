#pragma once

#include "CoreMinimal.h"
#include "Base.h"
#include "Maze.generated.h"

enum class EMazeCubiodFaces : uint8 {
	All,
	Vertical,
	Horizontal
};

UCLASS()
class PROCEDURALMESHES_API AMaze : public ABase {
	GENERATED_BODY()

public:
	AMaze();

protected:
	virtual void BeginPlay() override;

	virtual bool HasChanges() override;
	virtual void InitArrays() override;

	void AddPlane(FVector P1, FVector P2);
	void AddCuboid(FVector P1, FVector P2, EMazeCubiodFaces Direction);
	void GenerateMaze();

protected:
	/** Number of cells in horizontal direction */
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Maze", meta = (ClampMin = "1", ClampMax = "256", UIMin = "1", UIMax = "256"))
		int32 HCells = 1;
	/** Number of cells in vertical direction */
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Maze", meta = (ClampMin = "1", ClampMax = "256", UIMin = "1", UIMax = "256"))
		int32 VCells = 1;
	/** Width of labyrinth wall in relative units comparing with cell's size*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "0"))
		float WallWidthRelative = 0.05f;
	/** Wall height */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Maze", meta = (ClampMin = "0"))
		float Height = 0.05f;

	/** Inner number of cells in horizontal direction */
	int32 X;
	/** Inner number of cells in vercital direction */
	int32 Y;
	/** Inner WallWidthRelative */
	float W;
	/** Inner Height */
	float H;
	/** Horizontal array of walls (has size (VCells + 1) * HCells) */
	TArray<bool> HWalls;
	/** Vertical array of walls (has size VCells * (HCells + 1)) */
	TArray<bool> VWalls;
	/** Whether walls need to generate */
	bool IsMazeNeedGenerate = true;
};
