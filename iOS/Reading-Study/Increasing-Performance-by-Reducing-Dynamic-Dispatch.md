# Dynamic Dispatch를 줄여서 성능 향상시키기

원문: [Increasing Performance by Reducing Dynamic Dispatch](https://developer.apple.com/swift/blog/?id=27)

다른 언어들처럼 Swift에서도 슈퍼 클래스에 있는 메서드나 프로퍼티를 override할 수 있다. 이는 프로그램이 런타임에 어떤 메서드나 프로퍼티를 부를지나, 간접적인 호출이나 접근을 수행해야 하는 지 결정해야 함을 말한다. 이 테크닉은 `dynamic dispatch`라고 불린다. `dynamic dispatch`는 일정한 양의 간접적인 사용에 대한 런타임 오버헤드로 언어의 표현성을 증가시킨다. 성능이 중요한 코드에서 이러한 오버헤드는 종종 바람직하지 않다. 이 블로그 포스트는 `final`, ` private`, Whole Module Optimization을 이용해 dynamism을 줄이고 성능을 향상 시키는 세 가지 방법을 보여준다.



아래 예시를 보자.

```swift
class ParticleModel {
	var point = ( 0.0, 0.0 )
	var velocity = 100.0

	func updatePoint(newPoint: (Double, Double), newVelocity: Double) {
		point = newPoint
		velocity = newVelocity
	}

	func update(newP: (Double, Double), newV: Double) {
		updatePoint(newP, newVelocity: newV)
	}
}

var p = ParticleModel()
for i in stride(from: 0.0, through: 360, by: 1.0) {
	p.update((i * sin(i), i), newV:i*1000)
}
```

작성된 대로, 컴파일러는 동적으로 디스패치된 호출을 발행한다.

1. Call `update` on `p`
2. Call `updatePoint` on `p`
3. Get the property `point` tuple of p
4. Get the property `velocity` of p


Swift에서 dynamic dispatch 호출은 메서드 테이블에서 함수를 찾고, 간접 호출을 수행하면서 구현된다. 이러한 방식은 직접 호출을 수행하는 것보다 느리다. 간접 호출은 또한 많은 compiler optimization(컴파일러 최적화)를 방해하기 때문에 비용이 더 비싸다. 성능이 중요한 코드에서는 성능 향상에 필요하지 않은 dynamic behavior를 제한적으로 사용할 수 있게 하는 기술이 있다.



## override가 필요하지 않은 경우에는 `final` 키워드를 사용하라

`final` 키워드는 클래스, 메서드, 프로퍼티의 정의가 override되지 않을 것이라고 제한한다. 이것은 컴파일러가 안전하게 간접적인 방법의 dynamic dispatch를 제거할 수 있게 해 준다. 예를 들어 아래의 `point`와 `velocity`는 객체의 저장 프로퍼티로부터 직접적으로 접근될 것이고, `updatePoint()` 는 직접적인 함수 호출(direct function call)로 호출될 것이다. 반면 `update()`는 게속 dynamic dispatch를 통해 호출되기 때문에 subclass가 `update()`를 override해서 커스텀하는 것이 가능하다. 

```swift
class ParticleModel {
	final var point = ( x: 0.0, y: 0.0 )
	final var velocity = 100.0

	final func updatePoint(newPoint: (Double, Double), newVelocity: Double) {
		point = newPoint
		velocity = newVelocity
	}

	func update(newP: (Double, Double), newV: Double) {
		updatePoint(newP, newVelocity: newV)
	}
}
```

클래스 전체를 `final`로 표시하는 것도 가능하다. 이렇게 하면 클래스의 상속을 금지하고, 클래스 안의 모든 함수와 프로퍼티들은 `final` 이 된다.

```swift
 final class ParticleModel {
	var point = ( x: 0.0, y: 0.0 )
	var velocity = 100.0
	// ...
}
```



## `private` 키워드를 이용해서 한 파일 내에서 참조 된 `final`을 추론하라

 `private` 키워드를 붙이면 선언의 가시성(visibility)가 현재 파일로 제한된다. 이것은 컴파일러가 override의 가능성이 있는 모든 선언을 찾을 수 있게 한다. override 선언이 없으면 컴파일러는 `final` 키워드를 자동으로 추론해서 메서드나 프로퍼티 접근에 대한 간접 호출을 제거한다.

현재 파일에서 `ParticleModel` 에 대한 클래스 override가 없다고 가정하면, 컴파일러는 모든 dynamic dispatch된 호출을 직접 호출하는 private 선언으로 바꿀 수 있다.



```swift
class ParticleModel {
	private var point = ( x: 0.0, y: 0.0 )
	private var velocity = 100.0

	private func updatePoint(newPoint: (Double, Double), newVelocity: Double) {
		point = newPoint
		velocity = newVelocity
	}

	func update(newP: (Double, Double), newV: Double) {
		updatePoint(newP, newVelocity: newV)
	}
}
```

이전 예제에서와 같이, `point` 와 `velocity` 는 직접 접근, `updatePoint()` 는 직접 호출된다. `update()`는 `private`이 아니기 때문에 또 다시 간접적으로 불려진다.

`final`과 같이, `private` 을 클래스 선언에 적용해서 클래스를 `private`으로 만들 수 있고, 이는 모든 프로퍼티와 메서드에 적용된다.

```swift
private class ParticleModel {
	var point = ( x: 0.0, y: 0.0 )
	var velocity = 100.0
	// ...
}
```



> 글이 작성된 시기와 내용으로 짐작해보건데 private이 fileprivate의 의미를 가질 때 쓰여진 글인 것 같다. :)



## Whole Module Optimization을 이용해서 `internal` 선언을  ` final`로 추론하라

`internal` 선언은 선언된 모듈 안에서만 접근이 가능하다. (`internal`은 기본 접근 제어자이다.) Swift는 보통 파일을 모듈을 개별적으로 컴파일하기 때문에 컴파일러는 `internal` 선언이 다른 파일에 override 되었는지 확인하지 못한다. 그러나, Whole Module Optimization이 가능하면, 모든 모듈은 동시에 함께 컴파일 된다. 이것은 컴파일러는 전체 모듈에 대해 추론하게 만들고, 눈에 보이는 override가 없는 경우 `internal`을 `final`로 추론할 수 있게 한다.

원본 코드로 돌아가보자. 이번에는 `public` 키워드를 `ParticleModel`에 추가해보겠다.

```swift
public class ParticleModel {
	var point = ( x: 0.0, y: 0.0 )
	var velocity = 100.0

	func updatePoint(newPoint: (Double, Double), newVelocity: Double) {
		point = newPoint
		velocity = newVelocity
	}

	public func update(newP: (Double, Double), newV: Double) {
		updatePoint(newP, newVelocity: newV)
	}
}

var p = ParticleModel()
for i in stride(from: 0.0, through: times, by: 1.0) {
	p.update((i * sin(i), i), newV:i*1000)
}
```

이 코드를 Whole Module Optimization을 이용해 컴파일하면, 컴파일러는 `point`, `velocity` 프로퍼티와 `updatePoint()` 메서드 호출에 대해 `final`로 추론이 가능하다. 반대로, `update()`는 `public`으로 되어있기 때문에 `final`로 추론이 불가능하다.
