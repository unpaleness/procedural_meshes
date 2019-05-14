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

	GenerateMaze();
	W = WallWidthRelative;
	H = Height;

	// One cell's size
	float CellSize{ Y >= X ? InnerSize / (W * (Y + 1) + Y) : InnerSize / (W * (X + 1) + X) };

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
	while (N >= 0) {
		if (++it > 10000) {
			UE_LOG(LogProceduralMeshes, Warning, TEXT("%s: Limit of 10000 iteration exceeded!"), *GetName());
			break;
		}
		UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Processing (%i, %i)"), *GetName(), Path[N - 1] % X, Path[N - 1] / X);
		int32 NextCell{ GetRandomNeighbour(Path[N - 1], Cells) };
		if (NextCell >= X * Y) {
			UE_LOG(LogProceduralMeshes, Warning, TEXT("%s: Next cell is %i!"), *GetName(), NextCell);
			break;
		} else {
			UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Next cell is %i"), *GetName(), NextCell);
		}
		if (NextCell > 0) {
			Path[N] = NextCell;
			BreakWall(Path[N - 1], Path[N]);
			Cells[N] = true;
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
	// 1 - North, 2 - South, 4 - West, 8 - East
	uint8_t DirFlags{ 0 };
	uint8_t Dirs{ 0 };
	// Northern edge
	if (Index >= 40 && !Cells[Index - X]) {
		DirFlags += 1;
		++Dirs;
	}
	// Southern edge
	if (Index < (Y - 1) * X && !Cells[Index + X]) {
		DirFlags += 2;
		++Dirs;
	}
	// Western edge
	if (Index % X != 0 && !Cells[Index - 1]) {
		DirFlags += 4;
		++Dirs;
	}
	// Eastern edge
	if (Index % X != X - 1 && !Cells[Index + 1]) {
		DirFlags += 8;
		++Dirs;
	}
	UE_LOG(LogProceduralMeshes, Display, TEXT("%s: Neighbours %i"), *GetName(), DirFlags);
	// No neighbours
	if (Dirs <= 0) {
		return -1;
	}
	uint8_t Dir = rand() % Dirs;
	uint8_t ValidDir{ 0 };
	for (uint8_t i = 0; i < 4; ++i) {
		ValidDir += (DirFlags >> i) % 2;
		if (ValidDir == Dir) {
			switch (i) {
				case 0: return Index - X;
				case 1: return Index + X;
				case 2: return Index - 1;
				case 3: return Index + 1;
			}
			break;
		}
	}
	return -1;
}

void AMaze::BreakWall(int32 C1, int32 C2) {
	if (C1 < 0 || C2 < 0 || C1 >= X * Y || C2 >= X * Y || (C1 % X == 0 && C2 % X == X - 1) || (C1 % X == X - 1 && C2 % X == 0)) {
		return;
	}
	if (abs(C1 - C2) == 1) {
		VWalls[C1 / X * (X + 1) + (C1 > C2 ? C1 % X : C2 % X)] = false;
		return;
	}
	if (abs(C1 - C2) == X) {
		HWalls[(C1 > C2 ? C1 : C2) + C1 % X] = false;
		return;
	}
}
