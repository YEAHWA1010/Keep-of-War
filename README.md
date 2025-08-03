# 🎮 Keep of War - Soulslike Action RPG

## 📌 프로젝트 개요

| 항목 | 내용 |
| --- | --- |
| 📅 **작업 기간** | 2023.08.28 ~ 2023.12.12 (약 4개월) |
| 🎮 **장르** | 소울라이크 3인칭 액션 RPG |
| 💻 **개발 언어** | C++ |
| 🛠️ **개발 엔진** | Unreal Engine 5 |
| 👤 **참여 형태** | 개인 프로젝트 |
| 🧩 **담당 역할** | 무기 시스템, AI, 파쿠르, 활 구현, 전투 연출 등 전반 |

---

## 🧠 게임 소개

> **전략적인 전투와 몰입감 있는 연출을 중심으로 설계된 소울라이크 스타일 액션 RPG**  
플레이어는 칼, 망치, 활을 이용해 강력한 보스를 처치하며, 순간이동하는 적, 파쿠르 요소, 조준 시스템 등을 통해  
높은 몰입감과 도전 욕구를 유도합니다.

---

## 🧩 주요 기능

### 🗡 무기 시스템 (Sword / Hammer / Bow)
- 데이터 에셋 기반으로 무기별 장착/공격 로직 분리 및 통합
- 몽타주, 사운드, 히트박스, 이펙트까지 포함한 데이터 중심 설계

### 🧗 파쿠르 시스템
- 장애물 높이 및 거리 계산 후 점프, 슬라이딩, 벽 점프 등 동작 분기
- 커스터마이징 가능한 이동 애니메이션 연결

### 🤖 적 AI 및 EQS 전략
- EQS + AIService + Behavior Tree 조합
- 순간이동, 추적, 전투, 히트 반응 등 다양한 AI 상태 및 전투 패턴 구현

### 🏹 활 전투 및 조준 시스템
- Additive + AimOffset + Animation Layer를 활용한 방향 조준
- 활줄 휘어짐, 발사 에니메이션, 커서 기반 회전 처리 포함

### 💥 전투 연출
- 타겟 락온 시스템
- 피격 시 순간 정지(Hit Stop), 카메라 쉐이크 등 전투 피드백 강화
- UPoseableMeshComponent 기반 잔상 시스템 구현

---

## 🖼️ 게임 결과 화면

<table>
  <tr>
    <th>파쿠르 실행</th>
    <th>활 공격</th>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/63eaacd9-0e1b-4809-b6a7-d649764e09b7" width="350px" height="200px"></td>
    <td><img src="https://github.com/user-attachments/assets/5f8a33c7-9a56-4613-a444-7b8af64518ba" width="350px" height="200px"></td>
  </tr>
  <tr>
    <th>플레이어 콤보</th>
    <th>적 공격 잔상 시스템</th>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/88446d96-a79c-4078-a07f-8b3fb88f05eb" width="350px" height="200px"></td>
    <td><img src="https://github.com/user-attachments/assets/2873cbc1-d7a1-45b3-b474-2ff85273c305" width="350px" height="200px"></td>
  </tr>
</table>

---

## 🔗 외부 링크

- 📹 [플레이 영상 보기](https://youtu.be/GogdZGq0ry8)  
- 📄 [개발 명세서 (Notion)](https://melted-part-f0c.notion.site/Keep-Of-War-21f924ed314980c1a296d35f9729d9ea?source=copy_link)  

---

## 📧 이메일

yea2979@naver.com
