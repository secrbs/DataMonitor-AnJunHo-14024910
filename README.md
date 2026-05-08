# DataMonitor

현재 저장된 데이터 상태를 콘솔에서 실시간 조회하는 관리자 도구

## 구조

```
src/
├── monitor/
│   ├── TableRenderer.h/.cpp   - 테이블 렌더링
│   └── ConsoleMonitor.h/.cpp  - 화면 갱신 루프, 키입력 처리
└── main.cpp                   - data/ 디렉토리 JSON 파일 읽기 → ConsoleMonitor 연결
include/
└── nlohmann/json.hpp
data/
└── (모니터링 대상 JSON 파일 위치)
```

## 동작

- `data/` 디렉토리의 JSON 파일을 N초마다 다시 읽어 화면 갱신
- 파일이 변경되면 다음 갱신 주기에 자동 반영
- `[R]` 즉시 갱신 / `[Q]` 종료

```
╔══════════════════════════════════════════╗
║       Console Monitor [실시간 조회]       ║
╚══════════════════════════════════════════╝
  갱신 시각 : 2026-05-08 09:32:15
  갱신 주기 : 3초  [R] 즉시갱신  [Q] 종료

[ orders.json ]
  +--------------------+----------+------------------+----------+----------+
  | orderId            | sampleId | customerName     | quantity | status   |
  +--------------------+----------+------------------+----------+----------+
  | ORD-20260508-0001  | S-001    | 삼성전자 파운드리 | 200      | RESERVED |
  ...
  총 3건
```

## 빌드

**Visual Studio 2022**
`DataMonitor.sln` 열기 → 빌드

**커맨드라인 (MSVC)**
```bat
cl /std:c++17 /EHsc /I include src/main.cpp src/monitor/ConsoleMonitor.cpp src/monitor/TableRenderer.cpp /Fe:DataMonitor.exe
```

## 실행

```
DataMonitor.exe [data_directory] [refresh_seconds]
```

| 인자 | 기본값 |
|------|--------|
| data_directory | `data` |
| refresh_seconds | `3` |
