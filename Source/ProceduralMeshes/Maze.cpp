#include "Maze.h"

AMaze::AMaze() {}

void AMaze::BeginPlay() {
	IsMazeNeedGenerate = true;
	Super::BeginPlay();
}

bool AMaze::HasChanges() {
	bool result { false };
	if (Super::HasChanges()) {
		result = true;
	}
	if (HCells != X || VCells != Y) {
		IsMazeNeedGenerate = true;
		result = true;
	}
	if (WallWidthRelative != W || Height != H ) {
		result = true;
	}
	return result;
}

void AMaze::InitArrays() {
	Super::InitArrays();

	GenerateMaze();
	W = WallWidthRelative;
	H = Height;

	// One cell's size
	float CellSize { Y >= X ? InnerSize / (W * (Y + 1) + Y) : InnerSize / (W * (X + 1) + X) };

	for (int32 j = 0; j < Y + 1; ++j) {
		for (int32 i = 0; i < X + 1; ++i) {
			// Vertical walls
			if (j < Y) {
				if (VWalls[j * (X + 1) + i]) {
					AddCuboid(
						FVector(
							CellSize * ((W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							CellSize * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							0.0f
						),
						FVector(
							CellSize * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							CellSize * ((W + 1.0f) * (j + 1) - ((W + 1.0f) * Y + W) / 2.0f),
							H
						), false, false, true
					);
				}
			}
			// Horizontal walls
			if (i < X) {
				if (HWalls[j * X + i]) {
					AddCuboid(
						FVector(
							CellSize * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							CellSize * ((W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							0.0f
						),
						FVector(
							CellSize * ((W + 1.0f) * (i + 1) - ((W + 1.0f) * X + W) / 2.0f),
							CellSize * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							H
						), false, true, false
					);
				}
			}
		}
	}
}

void AMaze::AddCuboid(FVector P1, FVector P2, bool DrawBottom, bool DrawNorthSouth, bool DrawEastWest) {
	int32 V = Vertices.Num();

	FVector V0(P1.X, P1.Y, P1.Z);
	FVector V1(P1.X, P1.Y, P2.Z);
	FVector V2(P1.X, P2.Y, P1.Z);
	FVector V3(P1.X, P2.Y, P2.Z);
	FVector V4(P2.X, P1.Y, P1.Z);
	FVector V5(P2.X, P1.Y, P2.Z);
	FVector V6(P2.X, P2.Y, P1.Z);
	FVector V7(P2.X, P2.Y, P2.Z);

	if (DrawEastWest) {
		V = Vertices.Num();
		Vertices.Add(V0);
		Vertices.Add(V1);
		Vertices.Add(V2);
		Vertices.Add(V3);
		Vertices.Add(V6);
		Vertices.Add(V7);
		Vertices.Add(V4);
		Vertices.Add(V5);
		AddTriangle(V + 0, V + 3, V + 1);
		AddTriangle(V + 0, V + 2, V + 3);
		AddTriangle(V + 4, V + 7, V + 5);
		AddTriangle(V + 4, V + 6, V + 7);
	}
	if (DrawNorthSouth) {
		V = Vertices.Num();
		Vertices.Add(V2);
		Vertices.Add(V3);
		Vertices.Add(V6);
		Vertices.Add(V7);
		Vertices.Add(V4);
		Vertices.Add(V5);
		Vertices.Add(V0);
		Vertices.Add(V1);
		AddTriangle(V + 0, V + 3, V + 1);
		AddTriangle(V + 0, V + 2, V + 3);
		AddTriangle(V + 4, V + 7, V + 5);
		AddTriangle(V + 4, V + 6, V + 7);
	}
	if (DrawBottom) {
		V = Vertices.Num();
		Vertices.Add(V0);
		Vertices.Add(V2);
		Vertices.Add(V4);
		Vertices.Add(V6);
		AddTriangle(V + 0, V + 3, V + 1);
		AddTriangle(V + 0, V + 2, V + 3);
	}
	V = Vertices.Num();
	Vertices.Add(V3);
	Vertices.Add(V1);
	Vertices.Add(V7);
	Vertices.Add(V5);
	AddTriangle(V + 0, V + 3, V + 1);
	AddTriangle(V + 0, V + 2, V + 3);

	int32 sides { 1 };
	if (DrawEastWest) {
		sides += 2;
	}
	if (DrawNorthSouth) {
		sides += 2;
	}
	if (DrawBottom) {
		sides += 1;
	}
	for (int32 i = 0; i < sides; ++i) {
		UVs.Add(FVector2D(0.0f, 1.0f));
		UVs.Add(FVector2D(0.0f, 0.0f));
		UVs.Add(FVector2D(1.0f, 1.0f));
		UVs.Add(FVector2D(1.0f, 0.0f));
	}
}

void AMaze::GenerateMaze() {
	X = HCells;
	Y = VCells;

	HWalls.Empty();
	VWalls.Empty();

	// Fill walls
	for (int32 j = 0; j < Y + 1; ++j) {
		for (int32 i = 0; i < X + 1; ++i) {
			if (j < Y) {
				VWalls.Add(rand() % 2 ? true : false);
			}
			if (i < X) {
				HWalls.Add(rand() % 2 ? true : false);
			}
		}
	}

	IsMazeNeedGenerate = false;
}
