# 3. Function

## 원칙

* 함수는 한 가지 일만 해야 한다.

* 함수의 크기는 작은 것이 좋다. 4줄 정도가 적당하다고 할 수 있겠다.

* nested if, while 등은 없어야 한다.

* 잘 지어진 서술적인 긴 이름을 갖는 많고 작은 함수들로 유지해야 한다. (small many functions)



## The First Rule of Functions

* 더 이상 작아질 수 없을 만큼 작아야 한다.
* 큰 함수를 보면 클래스로 추출할 생각을 해야 한다. -> extract method object

* 클래스는 일련의 변수들에 동작하는 기능의 집합이다. 큰 함수는 로컬 변수와 기능들을 가지고 있다. 이것은 클래스로 추출할 수 있다.



## Example

1. Extract method object
2. Extract field
3. Extract method - includePage
4. Extract method - includeInherited
5. Extract method - includeSetups, includeTearDowns
6. Merge if statement
7. Change StringBuffer to String
8. Extract method - surroundPageWithSetUpsAndTearDowns
9. Rename - SetUpTearDownSurrounder



## 개선

읽기 쉬워지고, 이해하기 쉬워지고, 함수가 자신의 의도를 잘 전달할 수 있게 된다.



## 개선의 원인

* Small - 함수의 첫 번째 규칙. 함수는 작아질 수 있는 한 최대한 작아야 한다.
* 블록이 적어야 한다. - if, else, while 문장 등의 내부 블록은 한 줄이어야 한다.
* Indenting이 적어야 한다.



## Functions should do one thing

* 리팩토링 전: 하나의 함수가 하나 이상의 일을 한다.
* 리팩토링 후: 함수는 한 가지의 단순한 일만 한다. 
* 함수가 하나 이상의 일을 한다고 말할 수 있는 경우 (자기보다 한 단계 낮은 추상화 수준을 가진 함수들을 가지는 경우 한 가지 일을 한다고 볼 수 있다.) 더 이상 추출할 수 없을 때까지 extract를 한다.
* Reading code from top to bottom: 함수를 구성하는 원칙. 



## Where do classes go to hide?

* 큰 함수는 실제로는 클래스가 숨어 있는 곳이다.
* 변수와 인자들이 존재, 들여쓰기가 존재하며 변수들을 사용해서 통신하는 기능의 집합이 함수다. 이는 항상 하나 이상의 클래스로 분리할 수 있다.