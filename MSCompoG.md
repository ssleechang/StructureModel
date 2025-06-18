# MSCompoG 클래스 상속 구조 조사 보고서

## 1. 개요

MSCompoG는 구조 모델링 시스템의 핵심 클래스로, MSElement를 상속받아 구조 부재(Component Geometry)를 나타내는 기본 클래스입니다. 이 보고서는 MSCompoG의 상속 구조와 파생 클래스들의 역할 및 의미에 대해 상세히 분석한 결과를 제시합니다.

## 2. MSCompoG 기본 구조

### 2.1 상속 관계
```
MSObject
    └── MSElement  
        └── MSCompoG (기본 구조 부재 클래스)
```

### 2.2 주요 특징
- **역할**: 구조물의 기하학적 형상과 속성을 관리하는 기본 클래스
- **위치**: `MSCompoG.h`, `MSCompoG.cpp`
- **주요 멤버**: 
  - `MSCompoM* mp_CompoM`: 재료 및 단면 정보
  - `MSContainer* mp_Owner`: 소유자 컨테이너
  - `vector<GM2DVector*> ma_Vec`: 기하학적 점 정보
  - `GM2DPolyline* mp_Boundary`: 경계선 정보

## 3. 주요 파생 클래스 계층 구조

### 3.1 직접 파생 클래스들

#### 3.1.1 MSPartCompoG
- **상속**: `MSCompoG` → `MSPartCompoG`
- **역할**: 부분 구조 부재의 기본 클래스
- **파생 클래스**: `MSSolidCompoG`

#### 3.1.2 MSAssemblyG
- **상속**: `MSCompoG` → `MSAssemblyG`
- **역할**: 조립체 구조 부재
- **특징**: 조립체 정보와 기준점, 회전 정보 관리

#### 3.1.3 MSLinkMemberG (링크 부재)
- **상속**: `MSCompoG` → `MSLinkMemberG`
- **역할**: 선형 부재들의 기본 클래스
- **주요 파생 클래스**:
  - `MSBeamG` (보)
  - `MSBraceG` (브레이스)
  - `MSWallMemberG` (벽체)
  - `MSWallFootG` (벽체 기초)
  - `MSHandRailG` (저벽/난간)
  - `MSHaunchG` (헌치)

#### 3.1.4 MSPointMemberG (점 부재)
- **상속**: `MSCompoG` → `MSPointMemberG`
- **역할**: 점형 부재들의 기본 클래스
- **파생 클래스**: `MSVerticalLineMemberG`

#### 3.1.5 MSSlabG (슬래브)
- **상속**: `MSCompoG` → `MSSlabG`
- **역할**: 슬래브 부재
- **특징**: 다각형 형태의 바닥판 구조 부재

#### 3.1.6 MSWallOpeningG (벽체 개구부)
- **상속**: `MSCompoG` → `MSWallOpeningG`
- **역할**: 벽체 개구부의 기본 클래스
- **파생 클래스**:
  - `MSWindowG` (창)
  - `MSDoorG` (문)
  - `MSWallOpeningExtraG` (추가 개구부)

#### 3.1.7 MSOpeningG (일반 개구부)
- **상속**: `MSCompoG` → `MSOpeningG`
- **역할**: 일반적인 개구부 부재

#### 3.1.8 FormWork 관련 클래스들
- `MSFWSoundAbsorptionG` (방음재)
- `MSFWShadeG` (차양)
- `MSFWSwitchBoxG` (스위치박스)

### 3.2 다단계 상속 구조

#### 3.2.1 수직 선형 부재 계층
```
MSCompoG → MSPointMemberG → MSVerticalLineMemberG
                               ├── MSColumnG (기둥)
                               ├── MSWallColumnG (벽체기둥)
                               └── MSFWSurfaceFoldingG (표면 접이)
```

#### 3.2.2 링크 부재 계층 (선형 부재)
```
MSCompoG → MSLinkMemberG
           ├── MSBeamG (보)
           ├── MSBraceG (브레이스)  
           ├── MSWallMemberG (벽체)
           ├── MSWallFootG (벽체기초)
           ├── MSHandRailG (저벽/난간)
           └── MSHaunchG (헌치)
```

#### 3.2.3 선형 부재 기본 클래스
```
MSCompoG → MSLinkMemberG → MSLineMemberG
                          ├── MSBeamG (보)
                          ├── MSBraceG (브레이스)
                          ├── MSWallFootG (벽체기초)
                          ├── MSHandRailG (저벽/난간)
                          └── MSHaunchG (헌치)
```

## 4. 각 파생 클래스의 의미와 역할

### 4.1 구조 부재별 분류

#### 4.1.1 수직 부재 (Vertical Members)
- **MSColumnG**: 기둥 부재
  - 수직 하중 지지
  - 원형, 사각형 등 다양한 단면 형태
  - 경계 조건 설정 가능

- **MSWallColumnG**: 벽체 내 기둥
  - 벽체와 통합된 기둥 형태
  - 커스텀 단면 지원

#### 4.1.2 수평 부재 (Horizontal Members)
- **MSBeamG**: 보 부재
  - 수평 하중 전달
  - PC보, 일반보 등 다양한 타입
  - Built-up 보 지원

- **MSBraceG**: 브레이스 부재
  - 수평력 저항
  - 가새 역할

#### 4.1.3 벽체 부재 (Wall Members)
- **MSWallMemberG**: 벽체 부재
  - 내력벽, 옹벽 구분
  - 개구부 포함 가능
  - 차양(Shade) 부착 가능

- **MSHandRailG**: 저벽/난간
  - 낮은 높이의 벽체
  - 안전 기능

#### 4.1.4 기초 부재 (Foundation Members)
- **MSIsoFootG**: 독립 기초
  - 기둥 하부 기초
  - 개별 기초 형태

- **MSWallFootG**: 벽체 기초
  - 연속 기초 형태
  - 벽체 하부 지지

#### 4.1.5 슬래브 부재 (Slab Members)
- **MSSlabG**: 슬래브
  - 바닥판, 지붕판
  - 다양한 슬래브 타입 (일반, 데크, PC 등)
  - 경사 슬래브 지원
  - 레벨차 처리

### 4.2 특수 부재

#### 4.2.1 계단 부재
- **MSGenStairG**: 계단
  - 계단 형상 모델링
  - 단차 정보 관리

#### 4.2.2 피트/램프 부재
- **MSPitG**: 피트
  - 지하 구조물
  - 우물통 형태

- **MSRampG**: 램프
  - 경사로
  - 차량 진입로

#### 4.2.3 개구부 부재
- **MSWindowG**: 창문
  - 벽체 내 창문 개구부

- **MSDoorG**: 문
  - 벽체 내 문 개구부

- **MSOpeningG**: 일반 개구부
  - 슬래브, 벽체 등의 일반적인 구멍

#### 4.2.4 FormWork 전용 부재
- **MSFWSoundAbsorptionG**: 방음재
  - 거푸집 방음 기능
  
- **MSFWShadeG**: 차양
  - 거푸집 차양 기능

- **MSFWSurfaceFoldingG**: 표면 접이
  - 거푸집 표면 처리

## 5. Type_MSCompoG Enum 값들

```cpp
enum Type_MSCompoG {
    msNULL = 0, 
    msBuilding = 1, 
    msFloor, 
    msBeamG,           // 보
    msBraceG,          // 브레이스  
    msWallG,           // 벽체
    msColumnG,         // 기둥
    msIsoFootG,        // 독립기초
    msRampG,           // 램프
    msSlabG,           // 슬래브
    msStairG,          // 계단
    msPitG,            // 피트
    msWallFootG,       // 벽체기초
    msHandRailG,       // 저벽/난간
    msWindowG,         // 창문
    msDoorG,           // 문
    msStairPart = 26,  // 계단 부품
    msOpeningG = 30,   // 일반 개구부
    msWallOpeningExtraG, // 추가 벽체 개구부
    msBuiltUpBeam = 40, // Built-up 보
    msWallColumnG,     // 벽체 기둥
    msHaunchG = 80,    // 헌치
    msFWSurfaceFoldingG = 3000, // FormWork 표면 접이
    msFWShadeG,        // FormWork 차양
    msFWSwitchBoxG,    // FormWork 스위치박스
    msFWSoundAbsorptionG, // FormWork 방음재
};
```

## 6. 설계 패턴 및 특징

### 6.1 주요 설계 패턴
1. **Template Method Pattern**: 각 파생 클래스에서 공통 인터페이스 구현
2. **Factory Pattern**: `MSContainer::CreateNewCompoG()` 통한 객체 생성
3. **Composite Pattern**: 부재들의 계층적 구조 관리

### 6.2 핵심 기능
1. **기하학적 정보 관리**: 경계선, 점 정보
2. **재료 정보 연결**: MSCompoM과의 연결
3. **레벨 정보 처리**: Z-레벨 계산 및 관리
4. **트림 기능**: 부재 간 교차 처리
5. **물량 계산**: 콘크리트, 거푸집, 철근 물량

### 6.3 중요한 가상 함수들
- `GetType()`: 부재 타입 반환
- `MakeBoundary()`: 경계선 생성
- `GetZLevels()`: 레벨 정보 계산
- `Copy()`, `CopyFromMe()`: 객체 복사
- `Serialize()`: 직렬화

## 7. 결론

MSCompoG는 구조 모델링 시스템의 핵심 클래스로서 다음과 같은 특징을 갖습니다:

1. **확장성**: 다양한 구조 부재를 위한 유연한 상속 구조
2. **재사용성**: 공통 기능의 기본 클래스 제공
3. **모듈성**: 각 부재 타입별 전문화된 클래스 구조
4. **통합성**: 통일된 인터페이스를 통한 일관성 있는 처리

이러한 설계를 통해 건축/토목 구조물의 다양한 부재들을 효과적으로 모델링하고 관리할 수 있는 견고한 아키텍처를 제공합니다.