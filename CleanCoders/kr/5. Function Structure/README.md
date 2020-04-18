# 5. Function Structure

## Arguments

* 인자가 많아지면 복잡도가 증가한다.
* 3개를 최대로 생각하자.
* 생성자에 많은 수의 인자를 넘겨야 한다면 좋은 이름을 갖는 setter (Java Bean Pattern)이 나을 수 있다.
* Builder 패턴을 적용하는 게 더 좋은 방법일 수도!
* Boolean 인자는 사용하지 말자. 이것은 2가지 이상의 일을 한다는 것이다. true일 때, false일 때를 구분하지 말자. 이런 함수는 2개의 함수로 분리하자.
* Innies not Outies: Argument는 함수로 전달되는 것이지 output 인자로 사용하지 말아야 한다. output argument 대신 return value로 처리해야 한다.
* The null defense: null을 전달하는 함수는 마치 boolean처럼 사용하는 것과 같다. 이 경우에는 2개의 함수를 만드는 것이 맞다. null을 pseudo boolean으로 쓰지 말아야 하고, defensive programming을 지양해야 한다. null 체크, 에러 체클로 코드를 더럽히는 것은 잘못되었다는 증거고, 팀원이나 단위 테스트를 믿지 못한다는 것이다. null 여부를 지속적으로 조사하지 말고 단위 테스트에서 검증해야 한다.



## The stepdown rule

* 모든 public은 위에, 모든 private은 아래에!
* 왜? reader가 public만 읽고도 이해할 수 있도록 코드를 작성해라.
* 중요한 부분은 위로, 상세한 부분은 밑으로 옮겨서 마치 잡지를 읽는 것처럼 독자가 코드를 읽을 수 있게 해라. 잡지는 헤드라인, 시놉시스, 아웃라인 등 위에서 아래로 읽어나갈 수 있다.
* Backward reference 없이 top에서 bottom으로 읽을 수 있게 하자.



### Switches and Cases

* Switch 문 사용은 왜 꺼려질까? - 강한 의존성을 만들기 때문에! 이를 polymorphism을 이용해 해결하자.
* OO의 가장 큰 이점 중 하나는 의존성 관리 능력이다. 소스 코드 의존성과 런타임 의존성이 같은 방향일 경우에는 독립적으로 배포/컴파일/개발이 불가능하다.
* 객체지향이 가능하게 하는 것 중 하나는 런타임 의존성은 그대로 둔 채 소스 코드 의존성을 역전시키는 것이다. (Dependency Inversion Principle) 
  * 본래의 의존성을 제거하고
  * polymorphic interface를 삽입
  * 모듈 A는 인터페이스에 의존하고, 모듈 B는 인터페이스로부터 derive 한다.
  * 이로 인해 B의 소스코드 의존성은 런타임 의존성과 반대가 된다.
* Switch 문장은 어떻게 의존성을 만드나?
  * 각 case 문장은 외부 모듈에 의존성을 가지게 되고 다수의 다른 모듈에 대한 의존성을 가질 수 있다.
  * 이를 fan-out problem이라고 한다.
  * switch 문장에서 소스 코드 의존성은 flow of control(런타임 의존성)과 방향이 같다. 이는 외부 모듈에 영향을 받는다는 것이고 이렇게 되면 외부 모듈 중 하나라도 변경이 일어나면 switch 문이 영향을 받는다.
* Switch 제거 절차
  * polymorphic interface 호출로 변환
  * case에 있는 문장들을 별도의 클래스로 추출해서 변경 영향이 발생하지 않도록 한다.
* Application / Main partition
  * Application partition: 대개의 어플리케이션 코드가 존재하는 곳
  * Main partition: 하위 레벨 (factory, configuration data, main program)
  * 항상 App 파티션과 Main 파티션 사이에는 선을 그을 수 있도록 프로그램을 설계해야 한다.
  * DI를 활용해서 구현 가능
