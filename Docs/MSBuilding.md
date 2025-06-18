# MSBuilding 시스템 개요 및 사용법

## 개요
MSBuilding 시스템은 구조 해석 및 설계를 위한 3차원 건물 모델링 시스템입니다. MSBaseBuilding을 기반으로 하여 철근 콘크리트 설계(MSRebarBuilding)와 거푸집 설계(MSFormWorkBuilding)를 지원하는 계층적 구조로 구성되어 있습니다.

## 1. 클래스 상속 구조

### 1.1 기본 상속 계층
```
MSObject → MSElement → MSContainer → MSBaseBuilding
                                         ├─ MSRebarBuilding
                                         └─ MSFormWorkBuilding
```

### 1.2 주요 파생 클래스
- **MSRebarBuilding**: 철근 콘크리트 구조 설계를 위한 건물 클래스
- **MSFormWorkBuilding**: 거푸집 설계를 위한 건물 클래스

## 2. MSBaseBuilding 핵심 구성 요소

### 2.1 건물 정보 관리
- **MSBuildingInfo**: 건물의 기본 정보 (동명, 연면적, 용도 등)
- **MSBuildingInfoManager**: 건물 정보들을 관리하는 매니저 클래스
- **MSBuildingClassificationData**: 건물 분류 정보 (아파트, 지하주차장, 부속동 등)
- **MSBuildingUCS**: 건물의 사용자 좌표계 (UCS) 정보

### 2.2 공간 구조
- **MSFloor**: 층 정보를 관리하는 클래스
  - 층별 부재 관리
  - 높이 정보
  - 슬래브 레벨 관리
  - QuadTree를 이용한 공간 인덱싱 (MSFloorQT)

### 2.3 구조 부재 (MSCompoM/MSCompoG)
#### 2.3.1 부재 마스터 정보 (MSCompoM)
- **MSCompoM**: 부재의 단면치수, 재료 등 핵심 정보를 담는 마스터 정보 클래스
- **MSLineMemberM**: 선형 부재 마스터 정보
- **MSCustomM**: 사용자 정의 부재 마스터 정보
- **MSCustomBeamM**: 사용자 정의 보 마스터 정보
- **MSCustomColumnM**: 사용자 정의 기둥 마스터 정보
- **MSCustomWallColumnM**: 사용자 정의 벽기둥 마스터 정보

#### 2.3.2 부재 글로벌 인스턴스 (MSCompoG)
- **MSCompoG**: 마스터 정보를 참조하여 실제 배치된 글로벌 인스턴스 클래스
- **MSBeamG**: 보 글로벌 인스턴스
- **MSColumnG**: 기둥 글로벌 인스턴스  
- **MSWallMemberG**: 벽체 글로벌 인스턴스
- **MSSlabG**: 슬래브 글로벌 인스턴스
- **MSIsoFootG**: 독립기초 글로벌 인스턴스
- **MSStairG**: 계단 글로벌 인스턴스
- **MSRampG**: 램프 글로벌 인스턴스
- **MSPitG**: 피트 글로벌 인스턴스

### 2.4 재료 시스템
- **MSMaterial**: 재료 기본 클래스
  - 콘크리트, 철근, 강재 등의 재료 속성
- **MSReinforceBar**: 철근 정보
- **MSReinforceBarSetting**: 철근 설정 정보
- **MSStoryMaterial**: 층별 재료 설정
- **MSSteelUtility**: 강재 유틸리티

### 2.5 단면 및 프로파일
- **MSCustomSection**: 사용자 정의 단면
- **MSCustomBeamSection**: 사용자 정의 보 단면
- **MSCustomColumnSection**: 사용자 정의 기둥 단면
- **MSCustomLineSection**: 사용자 정의 선형 단면
- **MSCustomWallColumnSection**: 사용자 정의 벽기둥 단면

### 2.6 보조 요소
- **MSGridLine**: 그리드 라인
- **MSMarkM/MSMarkG**: 마킹 시스템
- **MSBlock**: 블록 시스템
- **MSAssembly**: 조립체 관리
- **MSGroup**: 그룹 관리

### 2.7 레벨 및 구역 관리
- **MSLevelZone**: 레벨 구역
- **MSConstructionZone**: 시공 구역
- **MSLineLevelDifG**: 선형 레벨차
- **MSMemberLevelDif**: 부재 레벨차

### 2.8 개구부 및 특수 요소
- **MSOpening**: 일반 개구부
- **MSWallOpeningG**: 벽체 개구부
- **MSOpeningReinforceM**: 개구부 보강재
- **MSDoorG/MSDoorM**: 문 요소
- **MSWell**: 우물
- **MSTrench**: 트렌치
- **MSPile**: 말뚝

## 3. MSRebarBuilding 특화 기능

### 3.1 철근 관리
- 철근 데이터베이스 관리
- 철근 설정 및 등급 관리 (MSSteelGradeManager)
- 배근 템플릿 시스템 (BaseRebarTemplate)
- 철근 물량 관리 (RebarQuantityInfo)

### 3.2 배근 템플릿
- **BeamRebarTemplate**: 보 배근 템플릿
- **ColumnRebarTemplate**: 기둥 배근 템플릿
- **BWallRebarTemplate**: 벽체 배근 템플릿
- **IsoFootRebarTemplate**: 독립기초 배근 템플릿

### 3.3 철근 라인 및 영역
- **MSCustomReBarLine**: 사용자 정의 철근 라인
- **MSWallEndRebarLine**: 벽체 끝단 철근 라인
- **MSSlabRebar**: 슬래브 배근

### 3.4 물량 및 옵션
- **MSQuantityOption**: 물량 산출 옵션
- **MSCheckModelOption**: 모델 검토 옵션
- **RebarGlobalOptionManager**: 철근 전역 옵션 관리

## 4. MSFormWorkBuilding 특화 기능

### 4.1 거푸집 관리
- **MSFormWorkTypeManager**: 거푸집 타입 관리
- **MSFormWorkType**: 거푸집 타입 정의
- **MSFormWorkMaterial**: 거푸집 재료
- **MSFormWorkDesignUnit**: 거푸집 설계 단위

### 4.2 거푸집 템플릿
- **FormWorkTemplate**: 거푸집 템플릿
- **FormWorkTemplateManager**: 거푸집 템플릿 관리

### 4.3 형태 템플릿
- 거푸집 형태 템플릿 관리
- 사용자 정의 거푸집 형상

## 5. 템플릿 및 설정 시스템

### 5.1 영역 템플릿
- **MSAreaTemplate**: 영역 템플릿
- **MSAreaTemplateManager**: 영역 템플릿 관리

### 5.2 내역(BOQ) 시스템
- **NayukTemplate**: 내역(BOQ) 템플릿
- **NayukTemplateManager**: 내역(BOQ) 템플릿 관리
- **NayukFormworkSetting**: 내역(BOQ) 거푸집 설정
- **NayukRebarSetting**: 내역(BOQ) 철근 설정
- **NayukConcreteSetting**: 내역(BOQ) 콘크리트 설정

## 6. 데이터 관리 및 I/O

### 6.1 파일 관리
- 바이너리 파일 읽기/쓰기
- 직렬화 (Serialization) 지원
- 템플릿 파일 관리

### 6.2 ID 관리
- **TypeUIDFactory**: 타입별 고유 ID 생성
- 객체별 고유 식별자 관리

### 6.3 로깅 및 분류
- **Logger**: 로깅 시스템
- **ClassificationNameManager**: 분류명 관리

## 7. 사용법 가이드

### 7.1 기본 건물 생성
```cpp
// 철근 콘크리트 건물 생성
MSRebarBuilding* building = new MSRebarBuilding();
building->Init();

// 기본 설정 적용
building->SetGroundLevel(0.0);
building->SetLicenseVersion(최신버전);
```

### 7.2 층 추가 및 관리
```cpp
// 새 층 추가
MSFloor* floor = building->AddNewFloor(1); // 1층
floor->SetHeight(3000.0); // 높이 3m
floor->SetSlabLevel(100.0); // 슬래브 레벨

// 층 정보 조회
double floorLevel = building->GetFloorLevel(1);
```

### 7.3 부재 추가
```cpp
// 기둥 마스터 정보 생성
MSCustomColumnM* columnM = new MSCustomColumnM();
columnM->SetName("C1");

// 기둥 글로벌 인스턴스 생성
MSColumnG* columnG = new MSColumnG();
columnG->SetCompoM(columnM);

// 층에 추가
floor->Add(columnG);
```

### 7.4 재료 관리
```cpp
// 콘크리트 재료 생성
MSMaterial* concrete = building->CreateNewMaterial(MSMaterial::msRConcrete);
concrete->SetName("C24");

// 건물에 추가
building->Add(concrete);
```

### 7.5 템플릿 적용
```cpp
// 배근 템플릿 설정
building->SetRebarTemplateName("표준배근");
BaseRebarTemplate* rebarTemplate = building->GetRebarTemplate(RT_COLUMN);

// 내역(BOQ) 템플릿 설정
building->SetNayukTemplateByName("표준내역");
int nayukTemplateID = building->GetNayukTemplateID();
```

## 8. 주요 특징

### 8.1 모듈화된 설계
- 기능별 클래스 분리
- 명확한 책임 분할
- 확장 가능한 구조

### 8.2 템플릿 시스템
- 재사용 가능한 설계 패턴
- 표준화된 설계 프로세스
- 사용자 정의 템플릿 지원

### 8.3 물량 산출 및 내역(BOQ)
- 자동 물량 계산
- 상세한 물량 분석
- 내역서(BOQ) 생성 및 관리
- 다양한 출력 형식 지원

### 8.4 모델 검증
- 지오메트리 검증
- 설계 기준 검토
- 오류 탐지 및 수정

## 9. 확장성

MSBuilding 시스템은 다음과 같은 확장이 가능합니다:
- 새로운 부재 타입 추가
- 사용자 정의 템플릿 개발
- 추가 재료 모델 지원
- 새로운 설계 기준 적용
- 내역(BOQ) 양식 및 규격 확장

이 시스템은 건축 구조 설계의 전 과정을 지원하는 포괄적인 플랫폼으로, 효율적이고 정확한 구조 설계를 가능하게 합니다.

**용어 정의:**
- **MSCompoM (Master Information)**: 부재의 단면치수, 단면이름, 재료 등 핵심 정보를 담는 마스터 정보
- **MSCompoG (Global Instance)**: 마스터 정보를 참조하여 실제 건물에 배치된 글로벌 인스턴스
- **Nayuk**: 한국어 "내역"의 의미로, 영어의 BOQ(Bill of Quantity)에 해당하는 물량 내역서 시스템

**동작 원리:**
MSCompoM은 부재의 타입 정의 역할을 하며(예: C1 기둥 마스터 정보 - 600x600, C24 콘크리트), 여러 개의 MSCompoG 인스턴스들이 이 마스터 정보를 참조하여 실제 건물의 다양한 위치에 배치됩니다(예: 1층 A-1, 1층 B-1, 2층 A-1에 각각 배치된 C1 기둥들). 