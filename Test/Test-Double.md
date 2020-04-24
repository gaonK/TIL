# Test Double

테스트를 목적으로 사용하는 객체. 

[SUT](http://xunitpatterns.com/SUT.html)는 [DOC](http://xunitpatterns.com/DOC.html)들에 의존적이기 때문에 테스트를 하기 어렵다. 발생할 수 있는 side effects 때문에 테스트하기 어려운 경우 이를 제어하고 내부 행위를 더 잘 보여줄 수 있는 테스트 전략이 필요하다. DOC를 Test Double로 교체하는 것이다. (SUT를 교체하는 것이 아님에 주의하자.) 이 때, Test Double은 완벽하게 DOC와 같은 행위를 할 필요는 없다. 



## Test Doubles

* Dummy
* Fake
* Stub
* Spy
* Mock



## State Verification vs. Behavior Verification

상태 검증과 행위 검증.

* 상태 검증: 메서드가 수행된 후에 SUT나 DOC의 `상태`를 확인해서 올바르게 동작했는지 판단한다.
* 행위 검증: SUT나 DOC의 특정 `행위`를 검사함으로써 올바르게 동작했는지 판단한다. 



## Reference

* [Test Double](https://martinfowler.com/bliki/TestDouble.html)
* [Test Double](http://xunitpatterns.com/Test%20Double.html)
* [Mocks Aren't Stubs](https://martinfowler.com/articles/mocksArentStubs.html)
* [Testing Basics: SUT and DOCs](http://kaczanowscy.pl/tomek/2011-01/testing-basics-sut-and-docs)