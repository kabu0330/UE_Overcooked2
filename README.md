# Overcooked! 2 언리얼 엔진 모작 프로젝트

![Image](https://github.com/user-attachments/assets/22a88ae9-27e3-47ae-b79f-86288e5b93b3)

## 플레이 영상

![bandicam 2025-07-24 20-29-53-831 (4)](https://github.com/user-attachments/assets/d293ccb3-4f28-499f-b0ea-0a6c2d93cfa4)

____


## 📌 팀원 및 역할  

| 👤 팀원   | 🎭 역할       | 🔗 GitHub 링크 |
|----------|------------|--------------|
| 🧑‍🎨 박석원 | 캐릭터      | [Seokwon-Park](https://github.com/Seokwon-Park) |
| 🏗️ 류성민  | 레벨        | [kabu0330](https://github.com/kabu0330) |
| 🏗️ 이수환  | 레벨        | [dfgh2727](https://github.com/dfgh2727) |
| 🎨 강민정  | UI         | [minjeong0917](https://github.com/minjeong0917) |
| 🌍 전혜주  | 월드        | [JeonHyeJu](https://github.com/JeonHyeJu) |
| 🌐 김용일  | 글로벌      | [yongilkim01](https://github.com/yongilkim01) |


## 📑 공용 문서  
- [Notion](https://www.notion.so/19ea97d70b81808f9b0ed999d1a88230?pvs=4)


  
## 인게임 스크린샷
![Image](https://github.com/user-attachments/assets/18202e74-6fd8-4af3-a086-afa436a0ca45)


## 컨텐츠 클래스 구조도
![Image](https://github.com/user-attachments/assets/bdf0a910-126d-4fd8-9e2e-468cec602ddb)




## 주요 코드
### 1. 데이터 테이블 설정
재료, 요리 조합, 주문을 구현하기 위하여 데이터 테이블을 활용했습니다. 컨텐츠 확장성 고려 및 패키징 시 리소스 누락을 방지했습니다.
![Image](https://github.com/user-attachments/assets/74f843ae-52c3-4303-b75f-575836b661d1)


![Image](https://github.com/user-attachments/assets/1e31fc00-8b09-4d1a-8ba7-a240afd1fcd5)


재료는 종류(Type)와 상태(State)로 정의했습니다.


ex). 물고기(Type)를 손질하다(State). / 밥(Type)을 짓다(State).



'FIngredient Data Row'에서 재료와 관련된 데이터로 다음과 같이 설정했습니다.


|타입|목적|
|--|--|
|`UTexture2D*`|아이콘 텍스처|
|`EIngredientType`|재료의 종류|
|`UStatticMesh*`|재료 메시 리소스|
|`Location`|위치 조정|
|`Rotation`|회전 조정|
|`Scale`|크기 조정|
|`TArray<FIngredientCookDataRow>`|재료가 손질되어 바뀔 수 있는 모든 상태들|

![Image](https://github.com/user-attachments/assets/78cff9ec-8c5f-4485-b08f-91b6394fd21d)




### 2. 

