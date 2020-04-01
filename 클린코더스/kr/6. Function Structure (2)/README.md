# 6. Function Structure (2)

## Temporal Coupling

* 함수들이 순서를 지켜 호출되는 경우에는 어떻게하면 안전할까? 
* Passing a Block으로 해결하자.



## CQS (Command Query Separation)

* CQS는 side effect를 관리하는 좋은 방법이다.
* Command
  * 객체의 상태 변경 가능
  * side effect를 가진다.
  * 아무것도 반환하지 않는다.
* Query
  * side effect가 없다.
  * 계산값이나 시스템의 상태를 반환한다.
* CQS의 정의
  * 상태를 변경하는 함수는 값을 반환하면 안 된다.
  * 값을 반환하는 함수는 상태를 변경하면 안 된다.
* 코드를 읽는 독자들을 혼란스럽게 하지 말자. 함수가 독자가 기대하는 행동을 하도록 하자. CQS는 신뢰에  대한 이야기이기도 하다.



## Tell Don't Ask

* Tell other object what to do
* But not to ask object what the state iis
* CQS의 강한 버전. Tell don't ask를 잘 지키면 Command와 Query를 함께 사용하지 않게 된다.



## Law of Demeter

* 하나의 함수가 전체 시스템 객체들 간의 네비게이션을 아는 것은 잘못된 설계이다. 이럴 경우에는 함수가 시스템에 너무 많은 의존성을 갖는 것고 있다는 것이다.
* 함수는 시스템의 전체를 알 필요가 없다. 개별 함수는 아주 제한된 지식만을 알아야 한다.
* 아래 규칙들을 통해 Tell, Don't ask를 형식화 한다.
  * 인자로 전달된 객체, locally 생성한 객체, 필드로 선언된 객체, 전역 객체들의 메서드만 호출할 수 있다.
  * 이전 메서드의 호출 결과로 얻을 수 있는 객체의 메서드를 호출하면 안 된다.
* ask 대신 tell을 해서 decouple 시켜야 한다.



## Early return

* early return, guard return은 허용된다.
* 하지만 루프 중간에서 리턴하게 하지 말자. 코드를 이해하기 어려워진다.



## Error handling

* Stack example
  * capacity가 0일 때 pop을 하면 null을 반환해야 하는가?
  * 이 보다는 exception을 발생시키는 것이 좋다.
  * 이 경우 exception 이름은 최대한 구체적이어야 한다.
* Checked exception
  * reverse dependency를 유발한다. 하위 클래스에서 어떤 exception이 유발되면 상위 클래스를 변경해야 하고, 이를 사용하는 모든 코드가 변경되어야 한다.
  * 가장 좋은 것은 checked exception을 사용하지 않는 것이다. (unchecked exception을 사용하자.)



## Null is not an error

* empty stack이 있다고 가정했을 때 top을 호출하면 null을 기대할까?
* 이럴 경우에는 error를 반환하도록 하자.



## Null is a value

* 발견되지 않을 때 반환할 수 있는 값이 필요하다. 그럴 때는 null을 리턴하자.



## try도 하나의 역할/기능이다.

* 함수는 하나의 일만 해야 하는데 error handling도 하나이 일이라는 것을 염두에 두고 코드를 작성해야 한다
* 함수 내에서 try 문장이 있다면 변수 선언을 제외하고 첫 번째 문장이어야 한다.

