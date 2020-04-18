# Arrange-Act-Assert(AAA) Pattern

잘 짜여진 테스트 코드는 그 자체로 요구사항을 보여주는 일종의 design doc이 될 수 있다. 그렇기 때문에 테스트 코드의 가독성 또한 굉장히 중요하다고 할 수 있겠다.

Arrange-Act-Assert Pattern, 일명 AAA 패턴은 하나의 유닛 테스트를 Arrange, Act, Assert를 하는 섹션으로 구분지어 배치하는 것을 말한다. 단어로부터 느낌적인 느낌은 받을 수 있겠지만 각각의 섹션에서 어떤 일을 해야 하는지 한 번 보자.

### Arrange

Arrange 섹션에선 테스트하고자 하는 객체를 설정한다. 이 섹션에서 state, dependency 등을 설정해준다. Arrange 섹션은 가장 큰 섹션이다. 

### Act

Act 섹션에선 해당 유닛 테스트에서 테스트하고자 하는 행위를 메소드를 호출함으로써 수행한다. 이 때 하나의 Act는 한 줄로 이루어지도록 하자.

### Assert

테스트 결과가 원하던 결과인지 확인한다.



위의 세 가지가 혼재되어 사용될 경우 독자는 이 테스트를 통해 무엇을 테스트하고자 하는지 파악하기 어려워진다. 아래와 같이 순서를 잘 지켜서 테스트 코드의 가독성을 높여 보자~

```
// Arrange
...
...
...

// Act
...

// Assert
...
```





참고: https://freecontent.manning.com/making-better-unit-tests-part-1-the-aaa-pattern/