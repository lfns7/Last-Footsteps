# [Unreal] Last Footsteps

## __1. 소개__
|<img src="https://github.com/user-attachments/assets/2993e646-5e91-45d6-819a-c8458e7b0a41" />|<img src="https://github.com/user-attachments/assets/fc13797c-77a6-4d2d-a6a6-c441b7f789d3" /> |
| ---------- | ---------------------------------------------- |
<img src="https://github.com/user-attachments/assets/7c0eb007-1810-440c-8a2b-626bfa9c243c" /> | <img src="https://github.com/user-attachments/assets/e53b7abf-f655-493e-8159-f12b9c88a1e9" />|
<br>

- Unreal Engine 5를 활용하여 만든 캐주얼 레이싱 게임입니다.
- 3인 1개 팀으로 구성하여 각자가 희망하는 파트를 지목하여 담당하고 구현하였습니다.
- 개발기간 : 2025년 3월 31일 ~ 2025년 6월 9일 (10주)

<br>

## 2. 개발 환경
- Engine : Unreal Engine 5.4.4
- Lang : C++
- OS : Windows 10
- VCS : Perforce

<br>

## 3. 담당 기능
- 장애물과 아이템 충돌 및 캐릭터 회전
  > [Source/Last_Footsteps/CHR/CHR_Interaction.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/CHR/CHR_Interaction.cpp)<br>
  - 플레이어의 캐릭터를 기준으로 좌, 우, 정면으로 보이지 않는 LineTrace 및 SphereTrace를 발사하고, 해당 트레이서에 Custom TraceChannel이 감지되면 장애물의 위치로 이동 불가 또는 피격 판정을 처리함
  - 아이템은 캐릭터의 Collision 충돌로 감지하며 충돌한 대상에 대해 GetName으로 판단하여 획득한 아이템에 대한 효과 발생 <br><br>
  
  > [Source/Last_Footsteps/OBS/OBS_Turn_Platform.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/OBS/OBS_Turn_Platform.cpp)
  - 플레이어 캐릭터 및 보스 오브젝트를 캐스트하고, 해당 오브젝트들이 특정 위치의 Collision에 충돌 시 Platform의 위치를 읽어 각 해당하는 방향에 대해 Rotation 처리 및 RInterpTo를 사용한 보간 처리
    
- 아이템 생성 및 적용
  > [Source/Last_Footsteps/ITEM](https://github.com/lfns7/Last-Footsteps/tree/main/Source/Last_Footsteps/ITEM)
  - ClassFinder로 게임 실행 중 각 아이템들에 대한 정보를 Load하고 Platform의 Transform을 불러와 아이템 Actor Spawn
  - 아이템은 random_device를 사용하여 시드를 적용하고 무작위 결과에 의해 Platform마다 34회의 생성을 시도하며 아이템 0.75%, 무기 0.75% 확률로 생성을 시도
  - 생성은 중앙 레인부터 생성을 시도하며 매 생성마다 33%의 확률로 생성되는 위치가 바뀌며 생성되는 위치가 자연스레 흘러가도록 배치

- 플랫폼 및 장애물
  > [Source/Last_Footsteps/OBS/OBS_ObstaclePlatformSpawner.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/OBS/OBS_ObstaclePlatformSpawner.cpp)
  - 부모가 되는 Base Platform을 우선 구현하고, 자식이 되는 Platform을 BP로 두어 지정하며, 이를 플레이어가 Platform의 Collision과 Overlap 되었을 때 무작위 플랫폼중 1개를 생성함
  - 각 플랫폼은 RandRange에 의해 확률적으로 플랫폼의 회전 여부가 결정됨
    
- UI
  > [Source/Last_Footsteps/UI](https://github.com/lfns7/Last-Footsteps/tree/main/Source/Last_Footsteps/UI)
  - UMG를 활용하여 각 기능별로 구분하여 UI를 작성하였음
  - UI간 전환은 OnClick 이벤트로 처리하였으며 SetVisibility를 적용하여 구현
  - 메뉴 호출 시 Time Dilation을 조절하여 일시정지
  
- 게임 인스턴스
  > [Source/Last_Footsteps/private/GI_GameInstance.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/private/GI_GameInstance.cpp) <br>
  > [Source/Last_Footsteps/private/Last_Footsteps_GamemodeBase.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/private/Last_Footsteps_GamemodeBase.cpp)
  - 게임 전체의 생명 주기를 함께하는 Game Instance를 두어 게임 실행부터 종료까지 필요한 요소들을 Load 및 Save
  - UGameInstance 클래스를 상속하여 구현하였으며 이전 데이터의 세이브/로드는 SaveGame 클래스를 상속하여 구현
      
- 메뉴 및 설정
  > [Source/Last_Footsteps/UI/UI_GameSettingsWidget.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/UI/UI_GameSettingsWidget.cpp) <br>
  > [Source/Last_Footsteps/UI/UI_TitleWidget.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/UI/UI_TitleWidget.cpp)
  - 별개의 Level 구성 및 UMG Animation를 적용하여 동적인 연출
  - Game Instance와 연계하여 게임 전역의 음량, 퀄리티, 튜토리얼 및 인트로 영상 여부 설정
  - 설정에서 적용된 내용은 SaveGame 클래스에 의해 저장되며 저장과 동시에 Game Instance에 반영
    
- 사운드
  > [Source/Last_Footsteps/Sound/Sound_MasterVolume.cpp](https://github.com/lfns7/Last-Footsteps/blob/main/Source/Last_Footsteps/Sound/Sound_MasterVolume.cpp)
  - Master Sound Class 및 자식 FX Sound Class와 BGM Sound Class로 게임 전역의 음량 조절

<br>

## 4. 관련 자료
- 프로젝트 기획서 : https://drive.google.com/file/d/1kExSw-WWpIl2gIiop9J6kKENHOhCIKJs/view?usp=drive_link
- 기능 정의서 : https://docs.google.com/spreadsheets/d/1dEmpesbT5DwarkZ6pZBuvRZrTbGhgJjb-iSt_F6_pgg/edit?usp=sharing

<br>

## 5. 플레이영상 및 설치 방법
- 플레이 영상 : https://youtu.be/85dpYSLdrgY (유튜브)
- 설치 방법 : https://play.google.com/store/apps/details?id=com.Footstep.LastFootsteps (구글 플레이스토어)

  
