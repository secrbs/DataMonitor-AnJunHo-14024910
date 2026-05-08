# DataMonitor

PoC: 콘솔 기반 데이터 모니터링 Tool

## 목적

`data/` 디렉토리의 JSON 파일들을 콘솔에서 실시간 조회할 수 있는 관리자 도구.  
메인 앱(SampleOrderSystem 등)과 동일한 JSON 파일을 공유하여 데이터 상태를 확인한다.

## 구조

```
src/
├── monitor/
│   ├── JsonFileMonitor.h
│   └── JsonFileMonitor.cpp  - JSON 파일 탐색, 파싱, 테이블 출력
└── main.cpp
include/
└── nlohmann/
    └── json.hpp
data/
└── (모니터링 대상 JSON 파일 위치)
```

## 기능

| 기능 | 설명 |
|------|------|
| 파일 목록 | data/ 디렉토리의 JSON 파일 자동 탐색 |
| 테이블 출력 | JSON 배열을 컬럼 정렬된 테이블로 출력 |
| 레코드 요약 | 파일명, 레코드 수, 필드 목록 표시 |
| 새로고침 | [R] 입력으로 파일 목록 갱신 |

## 빌드

**Visual Studio 2022**
- `DataMonitor.sln` 열기 → 빌드

**커맨드라인 (MSVC)**
```bat
cl /std:c++17 /EHsc /I include src/main.cpp src/monitor/JsonFileMonitor.cpp /Fe:DataMonitor.exe
```

## 실행

```
DataMonitor.exe [data_directory]
```

기본값: 실행 위치의 `data/` 디렉토리

```
========================================
  DataMonitor  [데이터 모니터링 Tool]
  경로: C:\...\data
========================================
  JSON 파일 목록
----------------------------------------
  [1] orders.json
  [2] samples.json
----------------------------------------
  [R] 새로고침  [Q] 종료
선택 > 1

  파일  : orders.json
  레코드: 5건
  필드  : [id] [sampleId] [customerName] [quantity] [status]

  +-----+---------+--------------+----------+----------+
  | id  | sampleId| customerName | quantity | status   |
  +-----+---------+--------------+----------+----------+
  | 1   | S-001   | 삼성전자     | 200      | RESERVED |
  ...
```
