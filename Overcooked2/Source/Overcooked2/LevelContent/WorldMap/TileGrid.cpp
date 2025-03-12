// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/WorldMap/TileGrid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Global/OC2Global.h"

// TODO: Organize

ATileGrid::ATileGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Temp 10
	CreateTiles(Tiles, 10);
	SetStaticMeshes(Tiles);

	MulRadius = RADIUS * FMath::Sqrt(3.f);
}

void ATileGrid::CreateTiles(TMap<int8, FTileData>& _RefMap, int _Size)
{
	_RefMap.Empty();

	for (int i = 0; i < _Size; ++i)
	{
		FString Name = FString::Printf(TEXT("ISMComp_%d"), i);
		UInstancedStaticMeshComponent* Inst = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*Name);
		Inst->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Inst->SetMobility(EComponentMobility::Movable);

		FTileData Data;
		Data.TileInst = Inst;
		_RefMap.Add({ i, Data });
	}
}

void ATileGrid::SetStaticMeshes(TMap<int8, FTileData>& _Tiles)
{
	FString MeshName = "Mesh_TileDesertGrassBlend";
	FString MeshPath = "";

	UOC2Global::GetAssetPackageName(UStaticMesh::StaticClass(), MeshName, MeshPath);
	if (!MeshPath.IsEmpty())
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(*MeshPath);
		if (!MeshAsset.Succeeded())
		{
			UE_LOG(LogTemp, Fatal, TEXT("Check the mesh: %s"), *MeshName);
		}

		for (TPair<int8, FTileData>& Elem : _Tiles)
		{
			Elem.Value.TileInst->SetStaticMesh(MeshAsset.Object);
		}
	}
}

void ATileGrid::SetTileMaterials(TMap<int8, FTileData>& _Tiles)
{
	for (TPair<int8, FTileData>& Elem : _Tiles)
	{
		UMaterialInstanceDynamic* MatInstD = UMaterialInstanceDynamic::Create(Elem.Value.TileInst->GetMaterial(0), this);
		MatInstD->SetScalarParameterValue("AlphaValue", 0.0f);
		Elem.Value.TileInst->SetMaterial(0, MatInstD);
	}
}

// Temp
bool ATileGrid::IsExclude(int i, int j)
{
	bool ret = false;
	if (i == 0)
	{
		if (j == 0 || j == 1 || j == 2 || j == 3 || j == 7 || j == 8 || j == 9)
		{
			ret = true;
		}
	}
	else if (i == 1)
	{
		if (j == 0 || j == 1 || j == 9)
		{
			ret = true;
		}
	}
	else if (i == 3)
	{
		if (j == 8 || j == 9)
		{
			return true;
		}
	}
	else if (i == 4)
	{
		if (j == 0 || j == 8 || j == 9)
		{
			ret = true;
		}
	}
	else if (i == 5)
	{
		if (j == 0 || j == 1 || j == 2 || j == 8 || j == 9)
		{
			ret = true;
		}
	}
	return ret;
}

TArray<FIntVector2> ATileGrid::GetSurroundedNewIdxs(TArray<FIntVector2>& _Centers, int _Idx)
{
	TArray<FIntVector2> Arr;

	for (const FIntVector2& Center : _Centers)
	{
		int Row = static_cast<int>(Center.X);
		int Col = static_cast<int>(Center.Y);

		TArray<FIntVector2> NewArr;
		NewArr.Reserve(6);
		NewArr.Emplace(FIntVector2(Row, Col - 1));
		NewArr.Emplace(FIntVector2(Row, Col + 1));
		NewArr.Emplace(FIntVector2(Row - 1, Col));
		NewArr.Emplace(FIntVector2(Row + 1, Col));

		if (static_cast<int>(Center.Y) % 2 == 0)
		{
			NewArr.Emplace(FIntVector2(Row + 1, Col - 1));
			NewArr.Emplace(FIntVector2(Row + 1, Col + 1));
		}
		else
		{
			NewArr.Emplace(FIntVector2(Row - 1, Col - 1));
			NewArr.Emplace(FIntVector2(Row - 1, Col + 1));
		}

		for (const FIntVector2& AddIdx : NewArr)
		{
			if (Arr.Contains(AddIdx) == false && AddIdx != Center)
			{
				Arr.Emplace(AddIdx);
			}
		}
	}

	return Arr;
}

// TODO: Serialize
TMap<FIntVector2, int8> ATileGrid::GetSurroundedIdxs(const FIntVector2& _RowCol, uint8 _Size)
{
	TMap<FIntVector2, int8> _OrderMap;

	int Row = static_cast<int>((_RowCol.X - 1) / 2);
	int Col = static_cast<int>((_RowCol.Y - 1) / 2);
	int Idx = 0;

	if (Row < 1 || Col < 1)
	{
		return _OrderMap;
	}

	FIntVector2 Center = FIntVector2(Row, Col);
	_OrderMap.Add({ Center, Idx });

	TArray<FIntVector2> NewCenters;

	NewCenters.Emplace(Center);

	while (++Idx != _Size)
	{
		NewCenters = GetSurroundedNewIdxs(NewCenters, Idx);
		for (const FIntVector2& Vec : NewCenters)
		{
			if (!_OrderMap.Contains(Vec))
			{
				_OrderMap.Add({ Vec, Idx });
			}
		}
	}

	return _OrderMap;
}

void ATileGrid::BeginPlay()
{
	Super::BeginPlay();

	int Idx = -1;

	FIntVector2 XY{ NumI, NumJ };
	OrderMap = GetSurroundedIdxs(XY, 10);	// Temp 10

	for (int i = 0; i < NumI; ++i)
	{
		for (int j = 0; j < NumJ; ++j)
		{
			if (IsExclude(i, j))
			{
				continue;
			}

			FTransform trfm;
			FIntVector2 Vec = GetXY(i, j);

			trfm.SetLocation(FVector(Vec.X, Vec.Y, InitZ));

			int8* _Order = OrderMap.Find(FIntVector2{ i, j });

			// TODO: Handle exception
			if (_Order == nullptr)
			{
				UE_LOG(LogTemp, Fatal, TEXT("No Order"));
			}
			else
			{
				int8 Order = *_Order;
				if (Tiles.Contains(Order))
				{
					Tiles[Order].TileInst->AddInstance(trfm);
				}
			}
		}
	}

	SetTileMaterials(Tiles);
}

void ATileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElpasedSecs += DeltaTime;
	ElpasedRotateSecs += DeltaTime;
	
	if (ElpasedSecs > .01f)
	{
		ElpasedSecs = 0.f;
		FQuat NewRot = FQuat(FVector(1, 0, 0), FMath::DegreesToRadians(5)) * DeltaTime;
		NewRot.Normalize();

		bool _IsAllEnd = true;
		for (TPair<int8, FTileData>& Elem : Tiles)
		{
			int8 Order = Elem.Key;
			FTileData& TileData = Elem.Value;
			UInstancedStaticMeshComponent* TileInst = TileData.TileInst;
			int Cnt = TileInst->GetInstanceCount();

			if (Cnt == 0 || TileData.IsFinishedAnim)
			{
				continue;
			}
			
			_IsAllEnd = false;
			if (Order < RotateIdx)
			{
				bool isRotated = false;
				float Rad = 0.f;
				FVector Axis;

				FTransform trfm;
				TileInst->GetInstanceTransform(0, trfm);

				FQuat OrgRot = trfm.GetRotation();
				OrgRot.Normalize();

				OrgRot.ToAxisAndAngle(Axis, Rad);

				if (Rad < 3.14)
				{
					isRotated = true;

					float NewLocZ = 0.f;
					static float Frequency = 1.f;

					if (TileData.AmplitudeZ > 0)
					{
						float NewRad = Rad * 2.f;
						NewLocZ = TileData.AmplitudeZ-- * FMath::Sin(NewRad);
						//UE_LOG(LogTemp, Log, TEXT("NewRad: %f, NewLocZ: %f"), NewRad, NewLocZ);
					}

					for (int i = 0; i < Cnt; ++i)
					{
						TileInst->GetInstanceTransform(i, trfm);

						FVector NewLoc = trfm.GetLocation();
						NewLoc.Z = NewLocZ;
						trfm.SetLocation(NewLoc);
						trfm.SetRotation(trfm.GetRotation() * NewRot);
						TileInst->UpdateInstanceTransform(i, trfm, false);
					}
				}

				if (isRotated)
				{
					float Deg = FMath::RadiansToDegrees(Rad);
					int DegRatioI = static_cast<int>(Deg / 180.f * 10.f) + 1;
					float DegRatioF = DegRatioI / 10.f;
					//UE_LOG(LogTemp, Log, TEXT("DIV: %f"), Div);

					if (!FMath::IsNearlyEqual(TileData.PrevAlpha, DegRatioF, 1e-2))
					{
						TileData.PrevAlpha = DegRatioF;

						UMaterialInterface* MI = TileInst->GetMaterial(0);
						if (MI != nullptr)
						{
							UMaterialInstanceDynamic* MatInstD = Cast<UMaterialInstanceDynamic>(MI);
							if (MatInstD != nullptr)
							{
								MatInstD->SetScalarParameterValue("AlphaValue", DegRatioF);
							}
						}
					}
				}
				else  // Finished rotation and moving
				{
					for (int i = 0; i < Cnt; ++i)
					{
						TileInst->GetInstanceTransform(i, trfm);

						FVector NewLoc = trfm.GetLocation();
						NewLoc.Z = 0.f;
						trfm.SetLocation(NewLoc);
						trfm.SetRotation(FQuat::Identity);	// 180 deg?
						TileInst->UpdateInstanceTransform(i, trfm, false);
					}

					TileData.IsFinishedAnim = true;
				}
			}
		}

		if (_IsAllEnd)
		{
			UE_LOG(LogTemp, Log, TEXT("IsAllEnd!!!!!!!"));
			IsAllEnd = true;
			SetActorTickEnabled(false);
		}
	}

	if (ElpasedRotateSecs > .25f)
	{
		ElpasedRotateSecs = 0.f;

		if (RotateIdx < 10)
		{
			++RotateIdx;
		}
	}
}

FIntVector2 ATileGrid::GetXY(int _IdxI, int _IdxJ)
{
	float X = RADIUS * 2.f * 0.75f * _IdxJ;
	float Y = 0.f;

	if (_IdxJ % 2 == 0)
	{
		Y = MulRadius / 2 + MulRadius * _IdxI;
	}
	else
	{
		Y = MulRadius * _IdxI;
	}

	return FIntVector2(X, Y);
}

bool ATileGrid::IsEndTransition() const
{
	return IsAllEnd;
}