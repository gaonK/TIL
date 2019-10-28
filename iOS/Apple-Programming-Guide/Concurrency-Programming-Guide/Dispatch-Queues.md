# Concurrency Programming Guide

동시성 프로그래밍 가이드

원문: [Concurrency Programming Guide](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/Introduction/Introduction.html)



## Dispatch Queues

Grand Central Dispatch (GCD) dispatch queue는 작업을 수행하기에 적합한 강력한 도구다. Dispatch queue는 호출자와 관련된 임의의 코드 block을 비동기적 또는 동기적으로 실행시킬 수 있게 한다. 독립된 thread에서 수행해야 하는 거의 모든 작업을 dispatch queue를 이용해서 수행할 수 있다. dispatch queue의 장점은 사용하기 간단하고 thread를 이용한 코드보다 훨씬 더 효율적으로 작업을 실행할 수 있다는 것이다.

이 장에서는 dispatch queue를 어떻게 사용하고 앱에서 일반적인 작업들을 실행시킬 수 있을 지 소개한다. 만약 이미 존재하는 thread 코드를 dispatch queue를 이용하도록 전환하고 싶다면 [Migrating Away from Threads](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/ThreadMigration/ThreadMigration.html#//apple_ref/doc/uid/TP40008091-CH105-SW1)를 참고해라.



### Dispatch Queue란

Dispatch queue는 앱에서 작업을 비동기적이고 동시적으로 쉽게 수행할 수 있게 해 주는 방법이다. 작업(task)은 단순히 앱에서 수행되어야 할 필요가 있는 일(work)를 말한다. 예를 들어 어떤 계산을 수행하는 작업을 정의한다고 하면 자료 구조를 생성하거나 수정하고, 파일 또는 어떤 수로부터 데이터를 읽어 오는 것으로 정의할 수 있다. 하나의 함수나 block 객체에 연관된 코드를 넣고 그것을 dispatch queue에 추가해서 작업을 정의한다.

Dispatch queue는 마치 객체와 같은 자료구조로 추가된 작업들을 관리한다. 모든 dispatch queue는 first-in, first-out (선입선출) 방식을 따른다. 그러므로 큐에 추가된 작업은 항상 추가된 순서대로 시작된다. GCD는 몇몇 dispatch queue를 자동으로 제공하지만 특정 목적에 맞게 생성해서 사용할 수도 있다. 아래 테이블은 앱에서 사용 가능한 dispatch queue의 타입과 어떻게 사용할 수 있는지를 보여준다.

