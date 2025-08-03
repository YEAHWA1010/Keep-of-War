# 🎮 Keep of War - Soulslike Action RPG

## 📌 프로젝트 개요

- 프로젝트 명: Keep of War  
- 장르: 소울라이크 스타일 3인칭 액션 RPG  
- 개발 언어/엔진: Unreal Engine 5, C++  
- 참여 형태: 개인 개발  
- 링크:  
  - 🔗 [📹 플레이 영상](https://youtu.be/GogdZGq0ry8)  
  - 📄 [🧾 개발 명세서](https://melted-part-f0c.notion.site/Keep-Of-War-21f924ed314980c1a296d35f9729d9ea?source=copy_link)  
  - 💻 [🧠 GitHub](https://github.com/YEAHWA1010/Keep-of-War)

---

## 🎮 게임 소개

Keep of War는 보스 전투를 중심으로 구성된 소울라이크 스타일의 액션 RPG입니다.  
플레이어는 다양한 무기(칼, 망치, 활)를 장착하고, 장애물을 넘으며 전략적으로 전투에 임하게 됩니다.  
실시간 AI 전투, 조준 시스템, 순간이동 패턴, 잔상 이펙트 등 몰입도 높은 전투 시스템이 구현되어 있습니다.

---

## 🚀 주요 기능

### 🗡 무기 시스템
- 무기(칼, 망치, 활)를 데이터 에셋 기반으로 설계
- 공통 장착 및 전투 동작을 구조화하여 확장성과 유지보수성 향상

### 🧗 이동 시스템
- 장애물의 거리/높이 판별을 통해 파쿠르 이동 구현
  (점프, 슬라이딩, 벽 점프 등)

### 🤖 적 AI 및 전투 전략
- EQS + AIService + Behavior Tree 조합
- 정찰, 추적, 전투, 순간이동 등 상태 기반 전투 패턴
- 근거리/원거리 타입에 따라 행동 방식 분기 처리

### 🏹 활 전투 및 조준 시스템
- Additive + AimOffset + Animation Layer로 자연스러운 조준 연출
- 활줄 휘어짐까지 표현된 정밀한 조준/공격 애니메이션

### 💥 전투 연출 요소
- 타겟팅 시스템
- 피격 순간 정지(Hit Stop), 카메라 흔들림(Camera Shake)
- UPoseableMeshComponent 기반 잔상 시스템

---

## 🏁 게임 결과 화면

- 보스 처치 시 전투 종료 UI 출력
- 클리어 연출 및 엔딩 분기 구조 적용 가능

---

## 🔄 회고 및 배운 점

- 데이터 에셋 기반 설계로 무기/공격 정보를 쉽게 관리 및 확장 가능
- EQS 쿼리를 직접 구성하여 전략적 위치 선정을 구현
- 다양한 애니메이션 기법(Additive, AimOffset 등) 직접 활용
- 잔상 연출을 통해 메시 계열 컴포넌트의 구조와 본 제어 방식 학습

