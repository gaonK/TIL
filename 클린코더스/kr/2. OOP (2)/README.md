# 2. OOP (2)

## Composition over Inheritance

OOP에서 재사용을 말할 때 super class에 기능을 구현해두고 이를 상속 받아서 사용하는 것을 말하는 사람들이 많다.  하지만 상속을 통한 재사용은 진정한 의미의 재사용이라 할 수 없다. 상속을 구현하기란 굉장히 쉽다. 하지만 이는 변경의 유연함 측면에서 치명적인 단점이 있다. 



### 상속의 단점

####Super class의 변경이 다수의 sub class에 영향을 미친다.

Super class에서 구현된 메서드를 상속 받아 사용한다는 것은 마치 라이브러리를 사용하는 것과 같다. super class에서의 변경은 이를 상속 받는 다수의 sub class들의 기능 변경을 초래한다.

#### 유사한 기능의 확장에서 클래스의 갯수가 불필요하게 증가할 수 있다.

상속을 받는 경우, 다중 상속이 불가능하기 때문에 2개 이상의 수퍼 클래스의 기능이 필요하다면 1개를 상속 받고 다른 한 개는 따로 구현해야 한다. 

#### 상속 자체를 잘못 사용할 수도 있다.

ArrayList를 상속 받는 객체가 있다고 한다면, ArrayList 자체의 메서드를 호출해서 sub class에서는 의도하지 않았던 변화를 만들어내서 비즈니스 로직이 깨지는 경우가 발생할 수도 있다. 이런 문제를 해결하기 위해서는 ArrayList는 field variable로 가지고 있으면서 비즈니스 로직에서는 이 field variable의 상태를 업데이트 해주는 방식을 적용할 수도 있겠다.

OOP에서 진정한 재사용이란 상속이 아닌 **composition을 이용한 재사용**을 말한다.



## Composition(delegation)

```java
public class Calculator {
  private PriceStrategy strategy;
  
  public Calculator(PriceStrategy strategy) {
    this.strategy = strategy;
  }
  
  public void calculate( ... ) {
    this.strategy.apply(price);
  }
}
```

위 코드는 Strategy 패턴을 이용했다. `PriceStrategy`는 인터페이스로 존재하고 이 인터페이스의 구현체를 이용하여 `calculate()`를 구현하게 된다. 이렇게 하면 Polymorphism과 composition으로 인해 유연성이 증가한다. 즉 변경이 용이해진다. 이런 식으로 코드를 작성하게 된다면 mock을 이용한 unit test가 용이해진다. 이는 즉 TDD가 쉬워짐을 의미한다. OOP에서는 이렇게 인터페이스를 잘 사용하는 것이 정말 중요하다고 할 수 있다. 



## 추가: 상속이 항상 나쁘다고 할 수 있는가?

Base class, 즉 super class에서 구현을 하고 sub class는 그 구현을 사용하는 경우에는 base class의 사용이 나쁘다고만은 할 수 없다. 변경이 일어날 확률이 희박하고, 변경이 일어나더라도 sub class들에 적용이 되어야 하는 경우라면 상속이 더 좋은 방법일 수도 있다.



### 참고 자료

[Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)

