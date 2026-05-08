# DataMonitor

콘솔 실시간 모니터링 UI 컴포넌트

## 구조

```
src/
├── monitor/
│   ├── TableRenderer.h/.cpp   - 테이블 렌더링 (컬럼 정렬, 구분선)
│   └── ConsoleMonitor.h/.cpp  - 화면 갱신 루프, 키입력 처리
└── main.cpp
```

## 핵심 인터페이스

```cpp
// 데이터 공급 콜백
using DataProvider = std::function<std::vector<Section>()>;

// 실행: dataProvider를 주기적으로 호출하여 화면 갱신
monitor.run(dataProvider);
```

## 동작

- N초(기본 3초)마다 `dataProvider()`를 호출하여 화면 전체 갱신
- `[R]` 즉시 갱신 / `[Q]` 종료

```
╔══════════════════════════════════════════╗
║       Console Monitor [실시간 조회]       ║
╚══════════════════════════════════════════╝
  갱신 시각 : 2026-05-08 09:32:15
  갱신 주기 : 3초  [R] 즉시갱신  [Q] 종료

[ 시료 목록 ]
  +--------+----------------------+--------+--------+
  | ID     | 이름                 | 수율   | 재고   |
  +--------+----------------------+--------+--------+
  | S-001  | 실리콘 웨이퍼-8인치  | 0.92   | 480    |
  +--------+----------------------+--------+--------+
  총 3건
```

## 빌드

**Visual Studio 2022**
`DataMonitor.sln` 열기 → 빌드

**커맨드라인 (MSVC)**
```bat
cl /std:c++17 /EHsc src/main.cpp src/monitor/ConsoleMonitor.cpp src/monitor/TableRenderer.cpp /Fe:DataMonitor.exe
```

## 실행

```
DataMonitor.exe [refresh_seconds]
```
