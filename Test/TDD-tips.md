# TDD tips

* Assert First

  인간은 top-down으로 생각하는 걸 편하게 생각해서 어렵게 느껴질 수 있으나 익숙해지면 좋은 방법. 테스트 코드를 작성할 때 assert부터 거꾸로 작성한다. 이렇게 테스트 코드를 작성하게 되면 필연적으로 compile error, exception error를 만나게 된다. 이런 에러들을 없애나가는 방식으로, 에러를 이용해서 코드를 작성하자. 이 방법을 이용하면 항상 테스트로부터 코드를 작성할 수 있다.

* Triangular (삼각측량법)

  하나의 테스트 만으로는 테스트를 테스트가 fake 테스트인지, 정말 동작하는 테스트인지 알기 어려울 때가 있다. 2개 이상의 테스트를 이용해 문제와 해결 방법을 좀 더 명확하게 만드는 기법이다. 삼각측량법이 2개 이상의 지점의 위치를 이용하여 측량을 하는 것과 유사하게 2개 이상의 테스트를 작성하면 보다 정확한 테스트를 작성할 수 있다. 

* Transformation List
  
  Transformation은 주목할만한 구조의 변경 없이 행위를 변경하는 것을 말한다. 이를 이용해서 실패하는 테스트를 성공하게 만든다.
  
  1. Null
  2. Null to Constant
  3. Constant to Variable
  4. Add Computation
  5. Split Flow
  6. Variable to Array
  7. Array to Container
  8. If to While
  9. Recurse
  10. Iterate
  11. Assign
  12. Add Case
