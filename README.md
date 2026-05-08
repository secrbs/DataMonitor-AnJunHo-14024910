# DataMonitor

PoC: 콘솔 기반 실시간 데이터 모니터링 Tool

## 목적

`data/` 디렉토리의 JSON 파일들을 주기적으로 자동 갱신하여 현재 데이터 상태를 실시간으로 보여주는 관리자 도구.

## 구조

```
src/
├── monitor/
│   ├── JsonFileMonitor.h
│   └── JsonFileMonitor.cpp  - 자동 갱신 대시보드 구현
└── main.cpp
include/
└── nlohmann/json.hpp
data/
└── (모니터링 대상 JSON 파일 위치)
```

## 동작 방식

- 지정된 주기(기본 3초)마다 화면을 지우고 전체 JSON 파일을 다시 읽어 출력
- 메인 앱(SampleOrderSystem)이 데이터를 변경하면 자동으로 반영
- `[R]` 즉시 갱신 / `[Q]` 종료

```
╔══════════════════════════════════════════════════╗
║          DataMonitor  [실시간 데이터 조회]        ║
╚══════════════════════════════════════════════════╝
  갱신 시각 : 2026-05-08 09:32:15
  갱신 주기 : 3초  |  [R] 즉시 갱신  [Q] 종료
  경로      : C:\...\data

┌─ orders.json ── 3건 ──────────────────────────────┐
  +----+----------+--------------+----------+----------+
  | id | sampleId | customerName | quantity | status   |
  +----+----------+--------------+----------+----------+
  | 1  | S-001    | 삼성전자     | 200      | RESERVED |
  | 2  | S-002    | SK하이닉스   | 150      | CONFIRMED|
  +----+----------+--------------+----------+----------+

┌─ samples.json ── 2건 ─────────────────────────────┐
  ...
```

## 빌드

**Visual Studio 2022**
- `DataMonitor.sln` 열기 → 빌드

**커맨드라인 (MSVC)**
```bat
cl /std:c++17 /EHsc /I include src/main.cpp src/monitor/JsonFileMonitor.cpp /Fe:DataMonitor.exe
```

## 실행

```
DataMonitor.exe [data_directory] [refresh_seconds]
```

| 인자 | 설명 | 기본값 |
|------|------|--------|
| data_directory | JSON 파일 디렉토리 경로 | `data` |
| refresh_seconds | 자동 갱신 주기 (초) | `3` |
