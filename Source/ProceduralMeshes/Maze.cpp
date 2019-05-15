#include "Maze.h"

AMaze::AMaze() {}

void AMaze::BeginPlay() {
	IsMazeNeedGenerate = true;
	Super::BeginPlay();
}

bool AMaze::HasChanges() {
	bool result{ false };
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

	if (IsMazeNeedGenerate) {
		GenerateMaze();
	}
	W = WallWidthRelative;
	H = Height;

	// Add floor
	AddPlane(
		FVector(
			-Size * (W * (X + 1) + X) / 2.0f,
			-Size * (W * (Y + 1) + Y) / 2.0f,
			0.0f
		),
		FVector(
			Size * (W * (X + 1) + X) / 2.0f,
			Size * (W * (Y + 1) + Y) / 2.0f,
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
							Size * ((W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							Size * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							0.0f
						),
						FVector(
							Size * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							Size * ((W + 1.0f) * (j + 1) - ((W + 1.0f) * Y + W) / 2.0f),
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
							Size * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
							Size * ((W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							0.0f
						),
						FVector(
							Size * ((W + 1.0f) * (i + 1) - ((W + 1.0f) * X + W) / 2.0f),
							Size * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
							H
						), EMazeCubiodFaces::Vertical
					);
				}
			}
			// Corner
			AddCuboid(
				FVector(
					Size * ((W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
					Size * ((W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
					0.0f
				),
				FVector(
					Size * (W + (W + 1.0f) * i - ((W + 1.0f) * X + W) / 2.0f),
					Size * (W + (W + 1.0f) * j - ((W + 1.0f) * Y + W) / 2.0f),
					1.1f * H
				), EMazeCubiodFaces::All
			);
		}
	}
}

void AMaze::AddPlane(FVector P1, FVector P2) {
	int32 V{ Vertices.Num() };

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
	int32 V{ Vertices.Num() };

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

	// Temporary cell state:
	//   false - not visited
	//   true  - visited
	bool* Cells = new bool[X * Y];
	for (int32 i = 0; i < X * Y; ++i) {
		Cells[i] = false;
	}
	// Path of generator
	int32* Path = new int32[X * Y];
	// Elements in path
	int32 N{ 1 };

	// Start with random cell
	Path[0] = rand() % (X * Y);
	Cells[Path[0]] = true;

	int32 it{ 0 };
	while (N > 0) {
		if (++it > MaxIterations) {
			////UE_LOG(LogProceduralMeshes, Warning, TEXT("%s: Limit of %i iteration exceeded!"), *GetName(), MaxIterations);
			break;
		}
		//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: N: %i, Processing (%i, %i)"), *GetName(), N, Path[N - 1] % X, Path[N - 1] / X);
		int32 NextCell{ GetRandomNeighbour(Path[N - 1], Cells) };
		if (NextCell >= X * Y) {
			UE_LOG(LogProceduralMeshes, Warning, TEXT("%s: Next cell index (%I) is above maximum possible (%i)!"), *GetName(), NextCell, X * Y - 1);
			break;
		}
		if (NextCell >= 0) {
			Path[N] = NextCell;
			Cells[Path[N]] = true;
			BreakWall(Path[N - 1], Path[N]);
			++N;
		} else {
			--N;
		}
	}

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

	delete[] Path;
	delete[] Cells;
}

int32 AMaze::GetRandomNeighbour(int32 Index, bool* Cells) {
	// 0 - South (-X), 1 - North (+X), 2 - East(-1), 3 - West(+1)
	bool AvailDirs[4] = { false, false, false, false };
	uint8_t Dirs{ 0 };
	// Southern edge
	if (Index >= X && !Cells[Index - X]) {
		AvailDirs[0] = true;
		++Dirs;
	}
	// Northern edge
	if (Index < (Y - 1) * X && !Cells[Index + X]) {
		AvailDirs[1] = true;
		++Dirs;
	}
	// Eastern edge
	if (Index % X != 0 && !Cells[Index - 1]) {
		AvailDirs[2] = true;
		++Dirs;
	}
	// Western edge
	if (Index % X != X - 1 && !Cells[Index + 1]) {
		AvailDirs[3] = true;
		++Dirs;
	}
	//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Neighbours %i: %i, %i, %i, %i"), *GetName(), Dirs, AvailDirs[0], AvailDirs[1], AvailDirs[2], AvailDirs[3]);
	// No neighbours
	if (Dirs <= 0) {
		return -1;
	}
	uint8_t Dir = (rand() % Dirs) + 1;
	uint8_t ValidDir{ 0 };
	for (uint8_t i = 0; i < 4; ++i) {
		ValidDir += (AvailDirs[i] ? 1 : 0);
		if (ValidDir == Dir) {
			switch (i) {
				case 0:
					//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Southern dir (%i, %i)"), *GetName(), (Index - X) % X, (Index - X) / X);
					return Index - X;
				case 1:
					//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Northern dir (%i, %i)"), *GetName(), (Index + X) % X, (Index + X) / X);
					return Index + X;
				case 2:
					//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Eastern dir (%i, %i)"), *GetName(), (Index - 1) % X, (Index - 1) / X);
					return Index - 1;
				case 3:
					//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Western dir (%i, %i)"), *GetName(), (Index + 1) % X, (Index + 1) / X);
					return Index + 1;
			}
			break;
		}
	}
	return -1;
}

void AMaze::BreakWall(int32 C1, int32 C2) {
	//UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Breaking wall (%i,%i)|(%i,%i)"), *GetName(), C1 % X, C1 / X, C2 % X, C2 / X);
	if (C1 < 0 || C2 < 0 || C1 >= X * Y || C2 >= X * Y) {
		return;
	}
	if (abs(C1 - C2) == 1 && C1 / X == C2 / X) {
		VWalls[C1 / X * (X + 1) + (C1 > C2 ? C1 % X : C2 % X)] = false;
		return;
	}
	if (abs(C1 - C2) == X) {
		HWalls[(C1 > C2 ? C1 : C2)] = false;
		return;
	}
	UE_LOG(LogProceduralMeshes, Warning, TEXT("%s: Breaking wall (%i,%i)|(%i,%i) failed!"), *GetName(), C1 % X, C1 / X, C2 % X, C2 / X);
}
