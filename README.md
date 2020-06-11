# Snake-Game
Team project: Snake Game - 2020-1 C++ Programming

## 보고서

### 1 단계

* NCurses Library 함수들을 사용하여 2 차원 배열로 된 Snake Map을 Game 화면으로 표시하는 프로그램을 완성한다. 그림의 세부사항은 각자 정한다.
    * Map은 21x21 을 최소 크기로 한다.
* 주의 : Wall 과 Immune Wall 을 잘 구분할 것

#### 1단계 구현 내용

### 2 단계

* 1단계의 맵 위에 Snake 를 표시하고, 화살표를 입력받아 Snake 가 움직이도록 프로그램을 완성한다.
* Snake는 규칙 #1을 준수해야 한다.

#### 2단계 구현 내용

### 3 단계

* 2단계 프로그램에서, Map 위에 Growth Item과 Poison Item이 출현하도록 수정한다.
* 게임 규칙 #2를 준수해야 한다.
* Growth Item과 Poison Item을 Map 배열에 표현할 때 값을 정한다
    * 화면상에 표현시, 색이나 기호를 달리하여 구분할 수 있도록 한다
    * Map Data에서 Growth Item은 5, Poison Item은 6 과 같이 구분할 수 있도록 한다

#### 3단계 구현 내용

### 4 단계

* 3단계 프로그램에서, Map의 Wall의 임의의 위치에 한 쌍의 Gate가 출현할 수 있도록 변경하고, 각 Gate에 Snake가 통과할 수 있도록 수정한다.
* 게임 규칙 #3, #4, #5를 준수해야한다
* Wall(Immune Wall 포함)과 Gate를 Map배열에 표현할 때 값을 결정한다
    * 화면상에 표현시, Gate는 Wall과 구분될 수 있도록한다
    * Map Data에서 Gate는 7과 같이 하여, 다른 요소와 구분할 수 있도록 한다

#### 4단계 구현 내용

### 5 단계

* 4단계 프로그램에서, 우측에 게임 점수를 표시하는 화면을 구성한다.
* 게임 점수는 게임 규칙 #6을 준수한다
* Mission
    * 구성한 Map의 정의에 고정 값을 주거나
    * 매 게임마다 임의의 값을 주는 방식으로 처리한다
* Mission을 달성하면 다음 Map으로 진행하도록 프로그램을 완성한다
    * Stage는 최소 4개로 구성하고, 각 Stage의 Map은 서로 달라야 한다

#### 5단계 구현 내용

