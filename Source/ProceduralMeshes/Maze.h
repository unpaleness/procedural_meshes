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
	/** Generates maze using Recursive Backtracking algorithm */
	void GenerateMaze();
	/**
	 * Gets index of not visited neighbour cell
	 * @param Index                Index of current cell
	 * @param Cells                Array of cells, where "true" - visited, "false" - unvisited
	 * @return Index of not visited neighbour cell or -1 if where is no such cell
	 */
	int32 GetRandomNeighbour(int32 Index, bool* Cells);
	/**
	 * Tries to break a wall between two cells
	 * @param C1                   Index of first cell
	 * @param C2                   Index of second cell
	 */
	void BreakWall(int32 C1, int32 C2);

protected:
	/** Number of cells in horizontal direction */
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "ProceduralMesh|Maze", meta = (ClampMin = "1", ClampMax = "256", UIMin = "1", UIMax = "256"))
		int32 HCells = 1;
	/** Number of cells in vertical direction */
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "ProceduralMesh|Maze", meta = (ClampMin = "1", ClampMax = "256", UIMin = "1", UIMax = "256"))
		int32 VCells = 1;
	/** Width of labyrinth wall in relative units comparing with cell's size*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProceduralMesh|Maze", meta = (ClampMin = "0"))
		float WallWidthRelative = 0.05f;
	/** Wall height */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProceduralMesh|Maze", meta = (ClampMin = "0"))
		float Height = 0.05f;
	/** Interation limit for generator */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProceduralMesh|Maze", meta = (ClampMin = "0"))
		int32 MaxIterations = 100000;

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
