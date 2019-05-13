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

	// Add floor
	AddPlane(
		FVector(
			-CellSize * (W * (X + 1) + X) / 2.0f,
			-CellSize * (W * (Y + 1) + Y) / 2.0f,
			0.0f
		),
		FVector(
			CellSize * (W * (X + 1) + X) / 2.0f,
			CellSize * (W * (Y + 1) + Y) / 2.0f,
			0.0f
		)
	);

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
						), EMazeCubiodFaces::Horizontal
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
						), EMazeCubiodFaces::Vertical
					);
				}
			}
			// Corner
			AddCuboid(
				FVector(
					CellSize * ((W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
					CellSize * ((W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
					0.0f
				),
				FVector(
					CellSize * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
					CellSize * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
					1.1f * H
				), EMazeCubiodFaces::All
			);
		}
	}
}

void AMaze::AddPlane(FVector P1, FVector P2) {
	int32 V { Vertices.Num() };

	FVector V0(P1);
	FVector V1(P2.X, P1.Y, (P1.Z + P2.Z) / 2.0f);
	FVector V2(P1.X, P2.Y, (P1.Z + P2.Z) / 2.0f);
	FVector V3(P2);

	Vertices.Add(V0);
	Vertices.Add(V1);
	Vertices.Add(V2);
	Vertices.Add(V3);

	AddTriangle(V + 0, V + 3, V + 1);
	AddTriangle(V + 0, V + 2, V + 3);

	UVs.Add(FVector2D(0.0f, 1.0f));
	UVs.Add(FVector2D(0.0f, 0.0f));
	UVs.Add(FVector2D(1.0f, 1.0f));
	UVs.Add(FVector2D(1.0f, 0.0f));
}

void AMaze::AddCuboid(FVector P1, FVector P2, EMazeCubiodFaces Direction) {
	int32 V { Vertices.Num() };

	FVector V0(P1);
	FVector V1(P1.X, P1.Y, P2.Z);
	FVector V2(P1.X, P2.Y, P1.Z);
	FVector V3(P1.X, P2.Y, P2.Z);
	FVector V4(P2.X, P1.Y, P1.Z);
	FVector V5(P2.X, P1.Y, P2.Z);
	FVector V6(P2.X, P2.Y, P1.Z);
	FVector V7(P2);

	auto AddHorizontalVertices = [=]() {
		Vertices.Add(V0);
		Vertices.Add(V1);
		Vertices.Add(V2);
		Vertices.Add(V3);
		Vertices.Add(V6);
		Vertices.Add(V7);
		Vertices.Add(V4);
		Vertices.Add(V5);
	};

	auto AddVerticalVertices = [=]() {
		Vertices.Add(V2);
		Vertices.Add(V3);
		Vertices.Add(V6);
		Vertices.Add(V7);
		Vertices.Add(V4);
		Vertices.Add(V5);
		Vertices.Add(V0);
		Vertices.Add(V1);
	};

	switch (Direction) {
		case EMazeCubiodFaces::Horizontal:
			AddHorizontalVertices();
			break;
		case EMazeCubiodFaces::Vertical:
			AddVerticalVertices();
			break;
		default:
			AddHorizontalVertices();
			AddVerticalVertices();
			break;
	}
	Vertices.Add(V3);
	Vertices.Add(V1);
	Vertices.Add(V7);
	Vertices.Add(V5);

	for (int32 i = 0; i < (Vertices.Num() - V) / 4; ++i) {
		AddTriangle(V + i * 4, V + i * 4 + 3, V + i * 4 + 1);
		AddTriangle(V + i * 4, V + i * 4 + 2, V + i * 4 + 3);
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

	// Init all walls with
	HWalls.Init(true, (Y + 1) * X);
	VWalls.Init(true, Y * (X + 1));

	//// Fill walls
	//for (int32 j = 0; j < Y + 1; ++j) {
	//	for (int32 i = 0; i < X + 1; ++i) {
	//		if (j < Y) {
	//			VWalls.Add(rand() % 2 ? true : false);
	//		}
	//		if (i < X) {
	//			HWalls.Add(rand() % 2 ? true : false);
	//		}
	//	}
	//}

	IsMazeNeedGenerate = false;
}
