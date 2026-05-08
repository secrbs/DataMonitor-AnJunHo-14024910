# DataMonitor

PoC: 콘솔 실시간 모니터링 UI 컴포넌트

## 목적

데이터를 **어떻게 콘솔에 보여줄 것인가**만을 검증하는 PoC.  
데이터를 읽거나 파싱하는 것은 이 PoC의 관심사가 아니다.  
`DataProvider` 콜백으로 데이터를 주입받아 화면에 출력하는 UI 레이어만 독립적으로 구현한다.

실제 앱에서는 Repository(PoC-2)와 조합하여 사용한다.

## 구조

```
src/
├── monitor/
│   ├── TableRenderer.h/.cpp   - 테이블 렌더링 (컬럼 정렬, 구분선)
│   ├── ConsoleMonitor.h/.cpp  - 화면 갱신 루프, 키입력 처리
└── main.cpp                   - 하드코딩 더미 데이터로 컴포넌트 검증
```

## 핵심 인터페이스

```cpp
// 데이터 공급 콜백 — 앱에서 실제 데이터로 구현
using DataProvider = std::function<std::vector<Section>()>;

// 실행: dataProvider를 주기적으로 호출하여 화면 갱신
monitor.run(dataProvider);
```

## 동작

- N초(기본 3초)마다 `dataProvider()`를 호출 → 화면 전체 갱신
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
