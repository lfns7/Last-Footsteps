// Fill out your copyright notice in the Description page of Project Settings.


#include "OBS_ObstacleSpawner.h"
#include "OBS_ObstaclePlatformSpawner.h"
#include "Last_Footsteps_GamemodeBase.h"
#include "Kismet/GameplayStatics.h"
#include <random>
#include <vector>

using namespace std;

// Sets default values for this component's properties
UOBS_ObstacleSpawner::UOBS_ObstacleSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//static ConstructorHelpers::FClassFinder<AActor> SwingPointBP(TEXT("/Game/Blueprints/Character/CHR_Rope_SwingPoint.CHR_Rope_SwingPoint"));
	//if (SwingPointBP.Succeeded())
	//{
	//	RopeObstacleSwingPoint = SwingPointBP.Class;
	//}


	// 시멘트 포대
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> CementBag(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Ind_Con_Bag_Cement_Closed_01/SM_Ind_Con_Bag_Cement_Closed_01.SM_Ind_Con_Bag_Cement_Closed_01'"));
	if (CementBag.Succeeded())
	{
		FRandomObstacleData CementBagData;
		CementBagData.ObsObstacleMesh = CementBag.Object;
		CementBagData.ObsObstacleLocation = FVector(0, 0, 0);
		CementBagData.ObsObstacleRotation = FRotator(0, 0, 0);
		CementBagData.ObsObstacleScale = FVector(2.5f, 2.5f, 2.5f);

		OneLaneObstacle.Add(CementBagData);
	}

	// 표지판
	// 1레인 장애물
	// 스케일 5.0, 5.0, 5.0
	// 회전 0, 0, 0(or 180) -> 0일 떄 : SLOW 문구, 180일 떄 : STOP 문구
	ConstructorHelpers::FObjectFinder<UStaticMesh> MetalWornSlow(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Urb_Roa_Sign_StopSlow_Metal_Worn_01/SM_Urb_Roa_Sign_StopSlow_Metal_Worn_01.SM_Urb_Roa_Sign_StopSlow_Metal_Worn_01'"));	
	if (MetalWornSlow.Succeeded())
	{
		FRandomObstacleData MetalWornSlowData;
		MetalWornSlowData.ObsObstacleMesh = MetalWornSlow.Object;
		MetalWornSlowData.ObsObstacleLocation = FVector(0, 0, 0);
		MetalWornSlowData.ObsObstacleRotation = FRotator(0, 0, 0);
		MetalWornSlowData.ObsObstacleScale = FVector(5.0f, 5.0f, 5.0f);

		OneLaneObstacle.Add(MetalWornSlowData);
	}

	// 표지판
	// 1레인 장애물
	// 스케일 5.0, 5.0, 5.0
	// 회전 0, 0, 0(or 180) -> 0일 떄 : SLOW 문구, 180일 떄 : STOP 문구
	ConstructorHelpers::FObjectFinder<UStaticMesh> MetalWornStop(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Urb_Roa_Sign_StopSlow_Metal_Worn_01/SM_Urb_Roa_Sign_StopSlow_Metal_Worn_01.SM_Urb_Roa_Sign_StopSlow_Metal_Worn_01'"));
	if (MetalWornStop.Succeeded())
	{
		FRandomObstacleData MetalWornStopData;
		MetalWornStopData.ObsObstacleMesh = MetalWornStop.Object;
		MetalWornStopData.ObsObstacleLocation = FVector(0, 0, 0);
		MetalWornStopData.ObsObstacleRotation = FRotator(0, 180, 0);
		MetalWornStopData.ObsObstacleScale = FVector(5.0f, 5.0f, 5.0f);

		OneLaneObstacle.Add(MetalWornStopData);
	}

	// 철제 바리케이드
	// 1레인 장애물
	// 스케일 1.5, 1.5, 1.5
	ConstructorHelpers::FObjectFinder<UStaticMesh> Metal_Barricade(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Urb_Roa_Barricade_Metal_Worn_02/SM_Urb_Roa_Barricade_Metal_Worn_02.SM_Urb_Roa_Barricade_Metal_Worn_02'"));
	if (Metal_Barricade.Succeeded())
	{
		FRandomObstacleData Metal_BarricadeData;
		Metal_BarricadeData.ObsObstacleMesh = Metal_Barricade.Object;
		Metal_BarricadeData.ObsObstacleLocation = FVector(0, 0, 0);
		Metal_BarricadeData.ObsObstacleRotation = FRotator(0, 0, 0);
		Metal_BarricadeData.ObsObstacleScale = FVector(2.0f, 2.0f, 2.0f);

		OneLaneObstacle.Add(Metal_BarricadeData);
	}

	// 소형 콘크리트 바리케이드 1
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Ind_Roa_Barrier_Concrete_M_01/SM_Ind_Roa_Barrier_Concrete_M_01.SM_Ind_Roa_Barrier_Concrete_M_01'"));	
	if (Concrete_Single_1.Succeeded())
	{
		FRandomObstacleData Concrete_Single_1Data;
		Concrete_Single_1Data.ObsObstacleMesh = Concrete_Single_1.Object;
		Concrete_Single_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_1Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_1Data);
	}

	// 소형 콘크리트 바리케이드 2
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_2(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_01.SM_Block_01'"));
	if (Concrete_Single_2.Succeeded())
	{
		FRandomObstacleData Concrete_Single_2Data;
		Concrete_Single_2Data.ObsObstacleMesh = Concrete_Single_2.Object;
		Concrete_Single_2Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_2Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_2Data.ObsObstacleScale = FVector(0.8f, 0.8f, 1.25f);

		OneLaneObstacle.Add(Concrete_Single_2Data);
	}

	// 소형 콘크리트 바리케이드 3
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_3(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_02.SM_Block_02'"));
	if (Concrete_Single_3.Succeeded())
	{
		FRandomObstacleData Concrete_Single_3Data;
		Concrete_Single_3Data.ObsObstacleMesh = Concrete_Single_3.Object;
		Concrete_Single_3Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_3Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_3Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_3Data);
	}

	// 소형 콘크리트 바리케이드 4
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_4(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_03.SM_Block_03'"));
	if (Concrete_Single_4.Succeeded())
	{
		FRandomObstacleData Concrete_Single_4Data;
		Concrete_Single_4Data.ObsObstacleMesh = Concrete_Single_4.Object;
		Concrete_Single_4Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_4Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_4Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_4Data);
	}

	// 소형 콘크리트 바리케이드 5
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_5(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_04.SM_Block_04'"));
	if (Concrete_Single_5.Succeeded())
	{
		FRandomObstacleData Concrete_Single_5Data;
		Concrete_Single_5Data.ObsObstacleMesh = Concrete_Single_5.Object;
		Concrete_Single_5Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_5Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_5Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_5Data);
	}

	// 소형 콘크리트 바리케이드 6
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_6(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_05.SM_Block_05'"));
	if (Concrete_Single_6.Succeeded())
	{
		FRandomObstacleData Concrete_Single_6Data;
		Concrete_Single_6Data.ObsObstacleMesh = Concrete_Single_6.Object;
		Concrete_Single_6Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_6Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_6Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_6Data);
	}

	// 소형 콘크리트 바리케이드 7
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_7(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_06.SM_Block_06'"));
	if (Concrete_Single_7.Succeeded())
	{
		FRandomObstacleData Concrete_Single_7Data;
		Concrete_Single_7Data.ObsObstacleMesh = Concrete_Single_7.Object;
		Concrete_Single_7Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_7Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_7Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_7Data);
	}

	// 소형 콘크리트 바리케이드 8
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Single_8(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Block_07.SM_Block_07'"));
	if (Concrete_Single_8.Succeeded())
	{
		FRandomObstacleData Concrete_Single_8Data;
		Concrete_Single_8Data.ObsObstacleMesh = Concrete_Single_8.Object;
		Concrete_Single_8Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Single_8Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Single_8Data.ObsObstacleScale = FVector(1.25f, 1.25f, 1.5f);

		OneLaneObstacle.Add(Concrete_Single_8Data);
	}

	// 전단함
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> ElectricBox(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/AbandonedFactory/Environment/ElectricBox_01/sm_ElectricBox_03_01.sm_ElectricBox_03_01'"));
	if (ElectricBox.Succeeded())
	{
		FRandomObstacleData ElectricBoxData;
		ElectricBoxData.ObsObstacleMesh = ElectricBox.Object;
		ElectricBoxData.ObsObstacleLocation = FVector(0, 0, 0);
		ElectricBoxData.ObsObstacleRotation = FRotator(0, 197, 0);
		ElectricBoxData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		OneLaneObstacle.Add(ElectricBoxData);
	}

	// 노란 고깔 1
	// 1레인 장애물
	// 스케일 1.5, 1.5, 1.5
	ConstructorHelpers::FObjectFinder<UStaticMesh> Cone_Yellow_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Urb_Roa_Barrel_Plastic_Orange_01/SM_Urb_Roa_Barrel_Plastic_Orange_01.SM_Urb_Roa_Barrel_Plastic_Orange_01'"));
	if (Cone_Yellow_1.Succeeded())
	{
		FRandomObstacleData Cone_Yellow_1Data;
		Cone_Yellow_1Data.ObsObstacleMesh = Cone_Yellow_1.Object;
		Cone_Yellow_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		Cone_Yellow_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Cone_Yellow_1Data.ObsObstacleScale = FVector(1.5f, 1.5f, 1.5f);

		OneLaneObstacle.Add(Cone_Yellow_1Data);
	}

	// 노란 고깔 2
	// 1레인 장애물
	// 스케일 5.0, 5.0, 5.0
	ConstructorHelpers::FObjectFinder<UStaticMesh> Cone_Yellow_2(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Urb_Roa_Light_Plastic_Yellow_06/SM_Urb_Roa_Light_Plastic_Yellow_06.SM_Urb_Roa_Light_Plastic_Yellow_06'"));
	if (Cone_Yellow_2.Succeeded())
	{
		FRandomObstacleData Cone_Yellow_2Data;
		Cone_Yellow_2Data.ObsObstacleMesh = Cone_Yellow_2.Object;
		Cone_Yellow_2Data.ObsObstacleLocation = FVector(0, 0, 0);
		Cone_Yellow_2Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Cone_Yellow_2Data.ObsObstacleScale = FVector(5.0f, 5.0f, 5.0f);

		OneLaneObstacle.Add(Cone_Yellow_2Data);
	}

	// 앰뷸런스
	// 2레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> AmbulanceTruck_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Abandoned_Ambulance/source.source'"));
	if (AmbulanceTruck_1.Succeeded())
	{
		FRandomObstacleData AmbulanceTruck_1Data;
		AmbulanceTruck_1Data.ObsObstacleMesh = AmbulanceTruck_1.Object;
		AmbulanceTruck_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		AmbulanceTruck_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		AmbulanceTruck_1Data.ObsObstacleScale = FVector(0.58f, 0.58f, 0.58f);

		OneLaneObstacle.Add(AmbulanceTruck_1Data);
	}

	// 부서진 차 1
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> JunkCar_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Abandoned___junk_Car/abandoned_junk_car.abandoned_junk_car'"));
	if (JunkCar_1.Succeeded())
	{
		FRandomObstacleData JunkCar_1Data;
		JunkCar_1Data.ObsObstacleMesh = JunkCar_1.Object;
		JunkCar_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		JunkCar_1Data.ObsObstacleRotation = FRotator(5, -5, 13);
		JunkCar_1Data.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		OneLaneObstacle.Add(JunkCar_1Data);
	}

	// 부서진 차 2
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> JunkCar_2(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Infrod/Meshes/ST_Infrod_Body.ST_Infrod_Body'"));
	if (JunkCar_2.Succeeded())
	{
		FRandomObstacleData JunkCar_2Data;
		JunkCar_2Data.ObsObstacleMesh = JunkCar_2.Object;
		JunkCar_2Data.ObsObstacleLocation = FVector(0, 0, 0);
		JunkCar_2Data.ObsObstacleRotation = FRotator(0, 0, 0);
		JunkCar_2Data.ObsObstacleScale = FVector(0.9f, 0.9f, 0.9f);

		OneLaneObstacle.Add(JunkCar_2Data);
	}

	// LightMachine
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> LightMachine(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_LightMachine_Body.SM_LightMachine_Body'"));
	if (LightMachine.Succeeded())
	{
		FRandomObstacleData LightMachineData;
		LightMachineData.ObsObstacleMesh = LightMachine.Object;
		LightMachineData.ObsObstacleLocation = FVector(0, 0, 0);
		LightMachineData.ObsObstacleRotation = FRotator(0, 0, 0);
		LightMachineData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		OneLaneObstacle.Add(LightMachineData);
	}

	// 트럭 1
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Truck_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Abandoned_Truck/truck_011.truck_011'"));
	if (Truck_1.Succeeded())
	{
		FRandomObstacleData Truck_1Data;
		Truck_1Data.ObsObstacleMesh = Truck_1.Object;
		Truck_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		Truck_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Truck_1Data.ObsObstacleScale = FVector(0.5f, 0.5f, 0.7f);

		OneLaneObstacle.Add(Truck_1Data);
	}

	// 트럭 2
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Truck_2(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/VehicleVarietyPack/Meshes/SM_Truck_Box.SM_Truck_Box'"));
	if (Truck_2.Succeeded())
	{
		FRandomObstacleData Truck_2Data;
		Truck_2Data.ObsObstacleMesh = Truck_2.Object;
		Truck_2Data.ObsObstacleLocation = FVector(0, 0, 0);
		Truck_2Data.ObsObstacleRotation = FRotator(0, 90, 0);
		Truck_2Data.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		OneLaneObstacle.Add(Truck_2Data);
	}

	// 트럭 3
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> Truck_3(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/VehicleVarietyPack/Meshes/SM_Pickup.SM_Pickup'"));
	if (Truck_3.Succeeded())
	{
		FRandomObstacleData Truck_3Data;
		Truck_3Data.ObsObstacleMesh = Truck_3.Object;
		Truck_3Data.ObsObstacleLocation = FVector(0, 0, 0);
		Truck_3Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Truck_3Data.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		OneLaneObstacle.Add(Truck_3Data);
	}

	// 원통형 콘크리트
	// 1레인 장애물
	// 스케일 5.0, 5.0, 5.0
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Pipe(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Urb_Roa_Pipe_Concrete_M_03/SM_Urb_Roa_Pipe_Concrete_M_03.SM_Urb_Roa_Pipe_Concrete_M_03'"));
	if (Concrete_Pipe.Succeeded())
	{
		FRandomObstacleData Concrete_PipeData;
		Concrete_PipeData.ObsObstacleMesh = Concrete_Pipe.Object;
		Concrete_PipeData.ObsObstacleLocation = FVector(0, 0, -25);
		Concrete_PipeData.ObsObstacleRotation = FRotator(0, 90, 0);
		Concrete_PipeData.ObsObstacleScale = FVector(5.0f, 5.0f, 5.0f);

		OneLaneObstacle.Add(Concrete_PipeData);
	}


	// 철골
	// 1레인 장애물
	// 스케일 2.0 2.0 3.0
	ConstructorHelpers::FObjectFinder<UStaticMesh> SteelBeam(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PostApocalypticSandbox/Meshes/SteelBeamHor.SteelBeamHor'"));
	if (SteelBeam.Succeeded())
	{
		FRandomObstacleData SteelBeamData;
		SteelBeamData.ObsObstacleMesh = SteelBeam.Object;
		SteelBeamData.ObsObstacleLocation = FVector(0, 0, 0);
		SteelBeamData.ObsObstacleRotation = FRotator(0, 117, 0);
		SteelBeamData.ObsObstacleScale = FVector(2.0f, 2.0f, 3.0f);

		OneLaneObstacle.Add(SteelBeamData);
	}


	// 도로 바리케이트 1
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TrafficBarrier_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Ways/Freeway/Meshes/SM_FWYBarrier_Traffic_A01_N1.SM_FWYBarrier_Traffic_A01_N1'"));
	if (TrafficBarrier_1.Succeeded())
	{
		FRandomObstacleData TrafficBarrier_1Data;
		TrafficBarrier_1Data.ObsObstacleMesh = TrafficBarrier_1.Object;
		TrafficBarrier_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		TrafficBarrier_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		TrafficBarrier_1Data.ObsObstacleScale = FVector(0.95f, 0.95f, 0.95f);

		OneLaneObstacle.Add(TrafficBarrier_1Data);
	}

	// 도로 방향 표시기
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SignDisplayBoard(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Ways/Freeway/Meshes/SM_FWYSignConst_DisplayBoard_A01_N1.SM_FWYSignConst_DisplayBoard_A01_N1'"));
	if (SignDisplayBoard.Succeeded())
	{
		FRandomObstacleData SignDisplayBoardData;
		SignDisplayBoardData.ObsObstacleMesh = SignDisplayBoard.Object;
		SignDisplayBoardData.ObsObstacleLocation = FVector(0, 0, 0);
		SignDisplayBoardData.ObsObstacleRotation = FRotator(0, 0, 0);
		SignDisplayBoardData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		OneLaneObstacle.Add(SignDisplayBoardData);
	}

	// 가시 함정
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SpikeTrap(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Spike_Trap/spike_trap_01.spike_trap_01'"));
	if (SpikeTrap.Succeeded())
	{
		FRandomObstacleData SpikeTrapData;
		SpikeTrapData.ObsObstacleMesh = SpikeTrap.Object;
		SpikeTrapData.ObsObstacleLocation = FVector(0, 0, 0);
		SpikeTrapData.ObsObstacleRotation = FRotator(0, 0, 0);
		SpikeTrapData.ObsObstacleScale = FVector(4.0f, 4.0f, 5.0f);

		OneLaneObstacle.Add(SpikeTrapData);
	}

	// 모래 주머니 1
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SandBags_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Ways/Freeway/Meshes/SM_Sandbags_H01_N1.SM_Sandbags_H01_N1'"));
	if (SandBags_1.Succeeded())
	{
		FRandomObstacleData SandBags_1Data;
		SandBags_1Data.ObsObstacleMesh = SandBags_1.Object;
		SandBags_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		SandBags_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		SandBags_1Data.ObsObstacleScale = FVector(2.25f, 2.25f, 2.25f);

		OneLaneObstacle.Add(SandBags_1Data);
	}

	// 모래 주머니 2
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SandBags_2(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Ways/Freeway/Meshes/SM_Sandbags_H04_N1.SM_Sandbags_H04_N1'"));
	if (SandBags_2.Succeeded())
	{
		FRandomObstacleData SandBags_2Data;
		SandBags_2Data.ObsObstacleMesh = SandBags_2.Object;
		SandBags_2Data.ObsObstacleLocation = FVector(0, 0, 0);
		SandBags_2Data.ObsObstacleRotation = FRotator(0, 0, 0);
		SandBags_2Data.ObsObstacleScale = FVector(2.25f, 2.25f, 2.25f);

		OneLaneObstacle.Add(SandBags_2Data);
	}

	// 나무판자 울타리
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> BoardsFence(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PostApocalypticSandbox/Meshes/BoardsFence.BoardsFence'"));
	if (BoardsFence.Succeeded())
	{
		FRandomObstacleData BoardsFenceData;
		BoardsFenceData.ObsObstacleMesh = BoardsFence.Object;
		BoardsFenceData.ObsObstacleLocation = FVector(0, 0, 0);
		BoardsFenceData.ObsObstacleRotation = FRotator(0, 90, 0);
		BoardsFenceData.ObsObstacleScale = FVector(0.8f, 0.8f, 0.8f);

		OneLaneObstacle.Add(BoardsFenceData);
	}

	// 나무상자
	// 1레인 장애물	
	ConstructorHelpers::FObjectFinder<UStaticMesh> BigCrate(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Big_Crate/big_crate.big_crate'"));
	if (BigCrate.Succeeded())
	{
		FRandomObstacleData BigCrateData;
		BigCrateData.ObsObstacleMesh = BigCrate.Object;
		BigCrateData.ObsObstacleLocation = FVector(0, 0, 125);
		BigCrateData.ObsObstacleRotation = FRotator(0, 0, 0);
		BigCrateData.ObsObstacleScale = FVector(1.25f, 1.25f, 1.25f);

		OneLaneObstacle.Add(BigCrateData);
	}

	// 컨테이너 1
	// 1레인 장애물
	ConstructorHelpers::FObjectFinder<UStaticMesh> FreightContainer_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Freight_shipping_container_-_Rusted/freight_shipping_container_rusted.freight_shipping_container_rusted'"));
	if (FreightContainer_1.Succeeded())
	{
		FRandomObstacleData FreightContainer_1Data;
		FreightContainer_1Data.ObsObstacleMesh = FreightContainer_1.Object;
		FreightContainer_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		FreightContainer_1Data.ObsObstacleRotation = FRotator(0, 90, 0);
		FreightContainer_1Data.ObsObstacleScale = FVector(0.5f, 0.5f, 0.5f);

		OneLaneObstacle.Add(FreightContainer_1Data);
	}


	// 콘크리트 바리케이드 1 
	// 3레인 점프 장애물
	// 스케일 1.5, 1.5, 1.5
	ConstructorHelpers::FObjectFinder<UStaticMesh> Concrete_Large_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Scene_RoadsideConstruction/Assets/MS/3D/Ind_Con_Concrete_L_02/SM_Ind_Con_Jersey_Concrete_L_02.SM_Ind_Con_Jersey_Concrete_L_02'"));
	if (Concrete_Large_1.Succeeded())
	{
		FRandomObstacleData Concrete_Large_1Data;
		Concrete_Large_1Data.ObsObstacleMesh = Concrete_Large_1.Object;
		Concrete_Large_1Data.ObsObstacleLocation = FVector(0, 0, 0);
		Concrete_Large_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		Concrete_Large_1Data.ObsObstacleScale = FVector(1.5f, 1.5f, 1.5f);

		ThreeLaneObstacle.Add(Concrete_Large_1Data);
	}

	// 로드 배리어
	// 3레인 슬라이드 장애물
	//스케일 1, 1, 0.9
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> Road_Barrier(TEXT("/Script/Engine.StaticMesh'/Game/Fab/Road_Barrier/RoadBarrier.RoadBarrier'"));
	if (Road_Barrier.Succeeded())
	{
		FRandomObstacleData Road_BarrierData;
		Road_BarrierData.ObsObstacleMesh = Road_Barrier.Object;
		Road_BarrierData.ObsObstacleLocation = FVector(0, 0, 50);
		Road_BarrierData.ObsObstacleRotation = FRotator(0, 90, 0);
		Road_BarrierData.ObsObstacleScale = FVector(1.0f, 1.0f ,0.9f);

		ThreeLaneObstacle.Add(Road_BarrierData);
	}

	//철골2
	// 3레인 복합 장애물
	//스케일 3.5 3.5 3.5
	//회전 12, 17, 12
	ConstructorHelpers::FObjectFinder<UStaticMesh> MetalColumn(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_ElevatorStructure_MetalColumn02.SM_ElevatorStructure_MetalColumn02'"));
	if (MetalColumn.Succeeded())
	{
		FRandomObstacleData MetalColumnData;
		MetalColumnData.ObsObstacleMesh = MetalColumn.Object;
		MetalColumnData.ObsObstacleLocation = FVector(0, 0, 100);
		MetalColumnData.ObsObstacleRotation = FRotator(17, 12, 12);
		MetalColumnData.ObsObstacleScale = FVector(3.5f, 3.5f, 3.5f);
		

		ThreeLaneObstacle.Add(MetalColumnData);
	}

	//낮은 전광판
	//3레인 슬라이드 장애물
	//스케일 2.5 2.5 1.2
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> UpperBillboard_1(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_UpperBillboard_01.SM_UpperBillboard_01'"));
	if (UpperBillboard_1.Succeeded())
	{
		FRandomObstacleData UpperBillboard_1Data;
		UpperBillboard_1Data.ObsObstacleMesh = UpperBillboard_1.Object;
		UpperBillboard_1Data.ObsObstacleLocation = FVector(0, 0, -5);
		UpperBillboard_1Data.ObsObstacleRotation = FRotator(0, 0, 0);
		UpperBillboard_1Data.ObsObstacleScale = FVector(3.15f, 2.5f, 0.9f);

		ThreeLaneObstacle.Add(UpperBillboard_1Data);
	}


	//바리케이드용 책상
	//3레인 점프 장애물
	//스케일 2.2 1.5 2.2
	//회전 90, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> BarricadeDesk(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_OldDesk.SM_OldDesk'"));
	if (BarricadeDesk.Succeeded())
	{
		FRandomObstacleData BarricadeDeskData;
		BarricadeDeskData.ObsObstacleMesh = BarricadeDesk.Object;
		BarricadeDeskData.ObsObstacleLocation = FVector(0, 0, 30);
		BarricadeDeskData.ObsObstacleRotation = FRotator(0, 0, 90);
		BarricadeDeskData.ObsObstacleScale = FVector(2.2f, 1.5f, 2.2f);

		ThreeLaneObstacle.Add(BarricadeDeskData);
	}


	 //전광판
	 //3레인 점프 장애물
	 //스케일 1.5, 1.5, 1.0
	 //회전 57, 175, 180
	ConstructorHelpers::FObjectFinder<UStaticMesh> BigBillboard(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_BigBillboard_01.SM_BigBillboard_01'"));
	if (BigBillboard.Succeeded())
	{
		FRandomObstacleData BigBillboardData;
		BigBillboardData.ObsObstacleMesh = BigBillboard.Object;
		BigBillboardData.ObsObstacleLocation = FVector(0, 0, 90);
		BigBillboardData.ObsObstacleRotation = FRotator(185, 25, 85);
		BigBillboardData.ObsObstacleScale = FVector(1.5f, 1.5f, 1.0f);

		ThreeLaneObstacle.Add(BigBillboardData);
	}


	//흙 더미
	//3레인 점프 장애물
	//스케일 2.5 2.0 1.75
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> Dirty(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Dirty01.SM_Dirty01'"));
	if (Dirty.Succeeded())
	{
		FRandomObstacleData DirtyData;
		DirtyData.ObsObstacleMesh = Dirty.Object;
		DirtyData.ObsObstacleLocation = FVector(0, 0, 0);
		DirtyData.ObsObstacleRotation = FRotator(0, 0, 0);
		DirtyData.ObsObstacleScale = FVector(2.5f, 2.0f, 1.75f);

		ThreeLaneObstacle.Add(DirtyData);
	}

	//넘어진 전봇대
	//3레인 점프 장애물
	//스케일 1.0 1.25 2.0
	//회전 0, 90, -30
	ConstructorHelpers::FObjectFinder<UStaticMesh> Electric(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/PA_UrbanCity/StaticMeshes/SM_Electric.SM_Electric'"));
	if (Electric.Succeeded())
	{
		FRandomObstacleData ElectricData;
		ElectricData.ObsObstacleMesh = Electric.Object;
		ElectricData.ObsObstacleLocation = FVector(0, 0, 50);
		ElectricData.ObsObstacleRotation = FRotator(90, -30, 0);
		ElectricData.ObsObstacleScale = FVector(1.0f, 1.25f, 2.0f);

		ThreeLaneObstacle.Add(ElectricData);
	}

	//차 무더기
	//3레인 점프 장애물
	//스케일 1.0 1.0 1.0
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> Cars(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/SM_Cars.SM_Cars'"));
	if (Cars.Succeeded())
	{
		FRandomObstacleData CarsData;
		CarsData.ObsObstacleMesh = Cars.Object;
		CarsData.ObsObstacleLocation = FVector(0, 0, 60);
		CarsData.ObsObstacleRotation = FRotator(0, 0, 0);
		CarsData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		ThreeLaneObstacle.Add(CarsData);
	}

	//콘테이너 무더기
	//3레인 슬라이드 장애물
	//스케일 1.0 1.0 1.0
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> Containers(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/SM_Container.SM_Container'"));
	if (Containers.Succeeded())
	{
		FRandomObstacleData ContainersData;
		ContainersData.ObsObstacleMesh = Containers.Object;
		ContainersData.ObsObstacleLocation = FVector(0, 0, 232);
		ContainersData.ObsObstacleRotation = FRotator(0, 0, 0);
		ContainersData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		ThreeLaneObstacle.Add(ContainersData);
	}

	//돌 무더기
	//3레인 슬라이드 장애물
	//스케일 1.0 1.0 1.0
	//회전 0, 2, 4
	ConstructorHelpers::FObjectFinder<UStaticMesh> Stones(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/SM_Stones.SM_Stones'"));
	if (Stones.Succeeded())
	{
		FRandomObstacleData StonesData;
		StonesData.ObsObstacleMesh = Stones.Object;
		StonesData.ObsObstacleLocation = FVector(0, 0, 330);
		StonesData.ObsObstacleRotation = FRotator(2, 4, 0);
		StonesData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);

		ThreeLaneObstacle.Add(StonesData);
	}

	//쓰레기 컨테이너
	//3레인 로프 장애물
	//스케일 2.0 2.0 2.0
	//회전 12, 0, 12
	ConstructorHelpers::FObjectFinder<UStaticMesh> TrashContainer(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/AbandonedFactory/Environment/TrashContainer_01/sm_TrashContainer_01.sm_TrashContainer_01'"));
	if (TrashContainer.Succeeded())
	{
		FRandomObstacleData TrashContainerData;
		TrashContainerData.ObsObstacleMesh = TrashContainer.Object;
		TrashContainerData.ObsObstacleLocation = FVector(0, 0, 0);
		TrashContainerData.ObsObstacleRotation = FRotator(0, 12, 12);
		TrashContainerData.ObsObstacleScale = FVector(1.25f, 1.25f, 1.25f);
		TrashContainerData.ObsObstacleRope = true;

		ThreeLaneObstacle.Add(TrashContainerData);
	}

	//철조망 1
	//3레인 로프 장애물
	//스케일 0.0125 0.0025 0.008
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> SpikeIronFence(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Spiky_Iron_Fence/spiky_iron_fence.spiky_iron_fence'"));
	if (SpikeIronFence.Succeeded())
	{
		FRandomObstacleData SpikeIronFenceData;
		SpikeIronFenceData.ObsObstacleMesh = SpikeIronFence.Object;
		SpikeIronFenceData.ObsObstacleLocation = FVector(0, 0, 0);
		SpikeIronFenceData.ObsObstacleRotation = FRotator(0, 0, 0);
		SpikeIronFenceData.ObsObstacleScale = FVector(0.0125f, 0.0025f, 0.007f);
		SpikeIronFenceData.ObsObstacleRope = true;

		ThreeLaneObstacle.Add(SpikeIronFenceData);
	}

	//트럭 4
	//3레인 로프 장애물
	//스케일 1, 1, 1 
	//회전 180, 80, 90
	ConstructorHelpers::FObjectFinder<UStaticMesh> Truck_4(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Abandoned_Truck/truck_011.truck_011'"));
	if (Truck_4.Succeeded())
	{
		FRandomObstacleData Truck_4Data;
		Truck_4Data.ObsObstacleMesh = Truck_4.Object;
		Truck_4Data.ObsObstacleLocation = FVector(0, 0, 35);
		Truck_4Data.ObsObstacleRotation = FRotator(80, 90, 180);
		Truck_4Data.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);
		Truck_4Data.ObsObstacleRope = true;

		ThreeLaneObstacle.Add(Truck_4Data);
	}


	//스트릿카
	//3레인 로프 장애물
	//스케일 1, 1, 1 
	//회전 0, -15, 100
	ConstructorHelpers::FObjectFinder<UStaticMesh> StreetCar(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/Broken_old_streetcar__FREE_/broken_old_streetcar_free.broken_old_streetcar_free'"));
	if (StreetCar.Succeeded())
	{
		FRandomObstacleData StreetCarData;
		StreetCarData.ObsObstacleMesh = StreetCar.Object;
		StreetCarData.ObsObstacleLocation = FVector(0, 0, 0);
		StreetCarData.ObsObstacleRotation = FRotator(-15, 100, 0);
		StreetCarData.ObsObstacleScale = FVector(1.0f, 1.0f, 1.0f);
		StreetCarData.ObsObstacleRope = true;

		ThreeLaneObstacle.Add(StreetCarData);
	}


	//철골
	//3레인 로프 장애물
	//스케일 1.5, 1.5, 1.5
	//회전 0, 37, 90
	ConstructorHelpers::FObjectFinder<UStaticMesh> PipeBackground(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/AbandonedFactory/Environment/PipeBackground_01/sm_PipeBackground_02_01.sm_PipeBackground_02_01'"));
	if (PipeBackground.Succeeded())
	{
		FRandomObstacleData PipeBackgroundData;
		PipeBackgroundData.ObsObstacleMesh = PipeBackground.Object;
		PipeBackgroundData.ObsObstacleLocation = FVector(0, 0, 0);
		PipeBackgroundData.ObsObstacleRotation = FRotator(37, 90, 0);
		PipeBackgroundData.ObsObstacleScale = FVector(1.5f, 1.5f, 1.5f);

		ThreeLaneObstacle.Add(PipeBackgroundData);
	}


	//쓰레기 더미
	//3레인 점프 장애물
	//스케일 2.25 2.25 2.25
	//회전 0, 0, 0
	ConstructorHelpers::FObjectFinder<UStaticMesh> BigTrashBag(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Meshes/AbandonedFactory/Environment/TrashBag_01/sm_TrashBag_01_01.sm_TrashBag_01_01'"));
	if (BigTrashBag.Succeeded())
	{
		FRandomObstacleData BigTrashBagData;
		BigTrashBagData.ObsObstacleMesh = BigTrashBag.Object;
		BigTrashBagData.ObsObstacleLocation = FVector(0, 0, -15);
		BigTrashBagData.ObsObstacleRotation = FRotator(0, 12, 12);
		BigTrashBagData.ObsObstacleScale = FVector(2.25f, 2.25f, 2.25f);

		ThreeLaneObstacle.Add(BigTrashBagData);
	}
}


// Called when the game starts
void UOBS_ObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	ALast_Footsteps_GamemodeBase* GM = Cast<ALast_Footsteps_GamemodeBase>(UGameplayStatics::GetGameMode(this));

	// 장애물 생성 수
	if (GM->Time <= 30)
	{
		MaxIndex = 2;
		ObstacleRange = 1000;
	}
	else if (GM->Time <= 45 && GM->Time > 30)
	{
		MaxIndex = 3;
		ObstacleRange = 980;
	}
	else if (GM->Time <= 70 && GM->Time > 45)
	{
		MaxIndex = 4;
		ObstacleRange = 960;
	}
	else if (GM->Time <= 100 && GM->Time > 70)
	{
		MaxIndex = 5;
		ObstacleRange = 940;
	}
	else if (GM->Time <= 130 && GM->Time > 100)
	{
		MaxIndex = 6;
		ObstacleRange = 920;
	}
	else if (GM->Time <= 160 && GM->Time > 130)
	{
		MaxIndex = 7;
		ObstacleRange = 900;
	}
	else if (GM->Time <= 210 && GM->Time > 160)
	{
		MaxIndex = 8;
		RandIndex = 7;
		ObstacleRange = 880;
	}
	else if (GM->Time <= 230 && GM->Time > 210)
	{
		MaxIndex = 9;
		RandIndex = 8;
		ObstacleRange = 800;
	}
	else if (GM->Time <= 250 && GM->Time > 230)
	{
		MaxIndex = 10;
		RandIndex = 9;
		ObstacleRange = 700;
	}
	else if (GM->Time <= 270 && GM->Time > 250)
	{
		MaxIndex = 11;
		RandIndex = 10;
		ObstacleRange = 600;
	}
	else if (GM->Time > 270)
	{
		MaxIndex = 15;
		RandIndex = 14;
		ObstacleRange = 500;
	}
	
	// 중복 장애물 방지
	vector<int> NoDuplication;
	for (int i = 0; i < MaxIndex; i++)
	{
		int TempNum = 2 + RandomSelect(RandIndex);

		while (true)
		{
			auto it = find(NoDuplication.begin(), NoDuplication.end(), TempNum);
			if (it == NoDuplication.end())
			{
				NoDuplication.push_back(TempNum);
				break;
			}
			else
			{
				TempNum = 2 + RandomSelect(RandIndex);
			}
		}

		if (TempNum == 4)
		{
			// 생성 확률은 70%
			if (RandomSelect(9) <= 6)
			{
				SpawnThreeLaneRandomMesh(SpawnThreeLaneObstacle(TempNum * ObstacleRange));
			}
			else
			{
				SpawnOneLaneRandomMesh(SpawnOneLaneObstacle(TempNum * ObstacleRange));
			}
		}
		else
		{
			SpawnOneLaneRandomMesh(SpawnOneLaneObstacle(TempNum * ObstacleRange));
		}
	}
}


// Called every frame
void UOBS_ObstacleSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), randNum));
}


// 단일 라인 무작위 스태틱 메시 생성 부분
void UOBS_ObstacleSpawner::SpawnOneLaneRandomMesh(FVector ObstaclePosition)
{	
	// TArray에 등록된 메시의 수 판별 (예외처리)
	if (OneLaneObstacle.Num() == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OneLaneObstacle's Size : 0")));
		return;
	}

	// 소환할 단일 장애물 뽑기
	const FRandomObstacleData& Selected = OneLaneObstacle[RandomSelect(OneLaneObstacle.Num()-1)];

	// Load된 메시 판별 (예외처리)
	if (!Selected.ObsObstacleMesh)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Selected Mesh is Null !")));
		return;
	}

	// StaticMeshComponent 생성
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
	MeshComponent->RegisterComponent();
	MeshComponent->SetStaticMesh(Selected.ObsObstacleMesh);
	MeshComponent->SetWorldLocation(GetComponentLocation() + ObstaclePosition + Selected.ObsObstacleLocation);
	MeshComponent->SetWorldRotation(GetComponentRotation() + Selected.ObsObstacleRotation);
	MeshComponent->SetWorldScale3D(Selected.ObsObstacleScale);

	MeshComponent->SetCanEverAffectNavigation(false); // 네비게이션 무시
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_GameTraceChannel2); // Object Collision Type을 Obstacle로 지정 (해당 값은 프로젝트 폴더 config의 DefaultEngine.ini에 있음/프로젝트 설정 값)
}

// 3라인 무작위 스태틱 메시 생성 부분
void UOBS_ObstacleSpawner::SpawnThreeLaneRandomMesh(FVector ObstaclePosition)
{
	// TArray에 등록된 메시의 수 판별 (예외처리)
	if (ThreeLaneObstacle.Num() == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ThreeLaneObstacle's Size : 0")));
		return;
	}

	// 소환할 단일 장애물 뽑기
	const FRandomObstacleData& Selected = ThreeLaneObstacle[RandomSelect(ThreeLaneObstacle.Num() - 1)];

	// Load된 메시 판별 (예외처리)
	if (!Selected.ObsObstacleMesh)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Selected Mesh is Null !")));
		return;
	}

	// StaticMeshComponent 생성
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
	MeshComponent->RegisterComponent();
	MeshComponent->SetStaticMesh(Selected.ObsObstacleMesh);
	MeshComponent->SetWorldLocation(GetComponentLocation() + ObstaclePosition + Selected.ObsObstacleLocation);
	MeshComponent->SetWorldRotation(GetComponentRotation() + Selected.ObsObstacleRotation);
	MeshComponent->SetWorldScale3D(Selected.ObsObstacleScale);

	MeshComponent->SetCanEverAffectNavigation(false); // 네비게이션 무시
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_GameTraceChannel2); // Object Collision Type을 Obstacle로 지정 (해당 값은 프로젝트 폴더 config의 DefaultEngine.ini에 있음/프로젝트 설정 값)

	if (Selected.ObsObstacleRope == true)
	{
		if (RopeObstacleSwingPoint)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			GetWorld()->SpawnActor<AActor>(RopeObstacleSwingPoint, GetComponentLocation() + ObstaclePosition + FVector(0.f, 0.f, 600.f), GetComponentRotation(), SpawnParams);
		}
	}
}

// 랜덤 처리부분
int UOBS_ObstacleSpawner::RandomSelect(int RandSize)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, RandSize); // 무작위 범위
	auto RandResult = dist(mt); // 실제로 수를 뽑기

	return RandResult;
}

// 단일 장애물 생성 부분
FVector UOBS_ObstacleSpawner::SpawnOneLaneObstacle(int SpawnDistance)
{
	// 레인 뽑기
	switch (RandomSelect(MAX_LANE_SIZE - 1))
	{
	// 현재 플랫폼의 각 레인을 기준으로 생성
	case 0:
		InitOneLanePosition = -250;
		break;
	case 1:
		InitOneLanePosition = 0;
		break;
	case 2:
		InitOneLanePosition = 250;
		break;
	}

	// Scene Component가 붙어있는 Root Actor를 판별
	// 해당 Actor의 Rotation 값을 반환 받아서 장애물 위치 판별
	if (GetAttachmentRootActor())
	{
		if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, 0.f, 1.f))
		{
			InitOneLaneObstaclePosition = FVector(InitOneLanePosition, SpawnDistance, 0);
		}
		else if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, -90.f, 1.f))
		{
			InitOneLaneObstaclePosition = FVector(SpawnDistance, InitOneLanePosition, 0);
		}
		else if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, 90.f, 1.f))
		{
			InitOneLaneObstaclePosition = FVector(-SpawnDistance, -InitOneLanePosition, 0);
		}
	}

	return InitOneLaneObstaclePosition;
}

FVector UOBS_ObstacleSpawner::SpawnThreeLaneObstacle(int SpawnDistance)
{
	// 생성 위치
	InitThreeLanePosition = 0;

	// Scene Component가 붙어있는 Root Actor를 판별
	// 해당 Actor의 Rotation 값을 반환 받아서 장애물 위치 판별
	if (GetAttachmentRootActor())
	{
		if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, 0.f, 1.f))
		{
			InitThreeLaneObstaclePosition = FVector(InitThreeLanePosition, SpawnDistance, 0);
		}
		else if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, -90.f, 1.f))
		{
			InitThreeLaneObstaclePosition = FVector(SpawnDistance, InitThreeLanePosition, 0);
		}
		else if (FMath::IsNearlyEqual(GetAttachmentRootActor()->GetActorRotation().Yaw, 90.f, 1.f))
		{
			InitThreeLaneObstaclePosition = FVector(-SpawnDistance, -InitThreeLanePosition, 0);
		}
	}

	return InitThreeLaneObstaclePosition;
}
