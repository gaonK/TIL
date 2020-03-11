# Concurrency Programming Guide

동시성 프로그래밍 가이드

원문: [Concurrency Programming Guide](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/Introduction/Introduction.html)



## Dispatch Queues

Grand Central Dispatch (GCD) dispatch queue는 작업을 수행하기에 적합한 강력한 도구다. Dispatch queue는 호출자와 관련된 임의의 코드 block을 비동기적 또는 동기적으로 실행시킬 수 있게 한다. 독립된 thread에서 수행해야 하는 거의 모든 작업을 dispatch queue를 이용해서 수행할 수 있다. dispatch queue의 장점은 사용하기 간단하고 thread를 이용한 코드보다 훨씬 더 효율적으로 작업을 실행할 수 있다는 것이다.

이 장에서는 dispatch queue를 어떻게 사용하고 앱에서 일반적인 작업들을 실행시킬 수 있을 지 소개한다. 만약 이미 존재하는 thread 코드를 dispatch queue를 이용하도록 전환하고 싶다면 [Migrating Away from Threads](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/ThreadMigration/ThreadMigration.html#//apple_ref/doc/uid/TP40008091-CH105-SW1)를 참고해라.



### Dispatch Queue란

Dispatch queue는 앱에서 작업을 비동기적이고 동시적으로 쉽게 수행할 수 있게 해 주는 방법이다. 작업(task)은 단순히 앱에서 수행되어야 할 필요가 있는 일(work)를 말한다. 예를 들어 어떤 계산을 수행하는 작업을 정의한다고 하면 자료 구조를 생성하거나 수정하고, 파일 또는 어떤 수로부터 데이터를 읽어 오는 것으로 정의할 수 있다. 하나의 함수나 block 객체에 연관된 코드를 넣고 그것을 dispatch queue에 추가해서 작업을 정의한다.

Dispatch queue는 마치 객체와 같은 자료구조로 추가된 작업들을 관리한다. 모든 dispatch queue는 first-in, first-out (선입선출) 방식을 따른다. 그러므로 큐에 추가된 작업은 항상 추가된 순서대로 시작된다. GCD는 몇몇 dispatch queue를 자동으로 제공하지만 특정 목적에 맞게 생성해서 사용할 수도 있다. 아래 테이블은 앱에서 사용 가능한 dispatch queue의 타입과 어떻게 사용할 수 있는지를 보여준다.

| Type                | Description                                                  |
| ------------------- | ------------------------------------------------------------ |
| Serial              | *private dispatch queue*로 알려지기도 한 serial queue는한 번에 하나의 작업을 큐에 추가된 순서에 맞게 실행한다. 현재 실행되는 작업은 dispatch queue에 의해서 관리되는 다른 스레드에서 동작한다. (스레드는 작업에 따라 달라질 수도 있다.) 개발자는 필요한 만큼 serial queue를 많이 만들 수 있고, 각각의 큐는 다른 큐들에 동시적으로 작동한다. 다른 말로 하자면, 네 개의 serial queue를 생성했다고 할 때 각각의 큐는 한 번에 오직 하나의 작업을 수행하짐반 최대 4개의 작업이 한 큐에 하나씩 동시에 실행될 수 있다. |
| Concurrent          | *global dispatch queue* 로 알려지기도 한 concurrent queue는 한 개 이상의 작업을 동시에 실행한다. 하지만 작업은 큐에 추가된 순서대로 실행이 시작되게 된다. 실행 중인 작업들은 dispatch queue에 의해 관리되는 구분된 스레드들에서 동작한다. 어떤 시점에서 실행되고 있는 작업의 정확한 수는 때마다 다르고, 작동하는 시스템 상태에 따라 달라진다. iOS 5와 그 이후에서 concurrent dispatch queue를 `DISPATCH_QUEUE_CONCURRENT` 를 큐의 타입으로 명시함으로써 concurrent dispatch queue를 생성할 수 있다. 추가로, 이미 정의된 네 개의 global concurrent dispatch queue를 앱에서 사용할 수 있다. |
| Main dispatch queue | main dispatch queue는 전역적으로 사용이 가능한 serial queue로 작업들을 앱의 메인 스레드에서 실행시킨다. 이 큐는 앱의 (만약 run loop이 있다면) run loop과 함께 동작한다. 이는 큐에 추가된 작업들과 run loop에 추가된 다른 이벤트 소스들의 실행을 배치하기 위해서다. 메인 스레드에서 실행되기 때문에 main queue는 종종 앱의 주요 동기화 지점으로 사용된다. main dispatch queue를 생성할 필요는 없지만 앱에서 적절하게 비워야 한다. |

동시성을 앱에 추가하려고 하는 관점에서 dispatch queue는 스레드보다 몇 가지 이점을 가진다. 가장 두드러지는 이점은 작업-큐 프로그래밍 모델의 간결성이다. 스레드를 이용하면 개발자들은 실행하기 원하는 작업과 스레드의 생성과 관리 코드를 모두 작성해야 한다. dispatch queue는 스레드의 생성과 관리에 대한 부담 없이 실행하고자 하는 작업에만 집중할 수 있게 해준다. 대신에 시스템이 스레드의 생성과 관리를 제어한다. 장점은 시스템은 하나의 앱이 할 수 있는 것보다 훨씬 더 효율적으로 스레드를 관리할 수 있다는 것이다. 시스템은 현재 시스템 조건에 맞게 스레드의 수를 동적으로 조절할 수 있다. 게다가 시스템은 보통 직접 생성한 스레드에서보다 작업의 실행을 더 빨리 시작시킬 수 있다.

dispatch queue를 이용하도록 코드를 재작성하는 것이 어렵게 느껴질 수도 있으나 종종 dispatch queue를 이용해서 코드를 작성하는 것이 스레드를 이용해서 작성하는 것보다 쉽다. 코드를 작성할 때의 핵심은 독립적이고 비동기적으로 실행될 수 있는 작업을 설계하는 것이다. 만약 동시에 같은 공유 자원에 접근하지만 다른 스레드에서 실행되는 두 개의 작업이 있을 때, 어떤 스레드든 먼저 공유 자원을 수정할 수 있으므로 두 작업이 동시에 공유 자원을 수정하지 않게 하기 위해 lock을 사용해야 한다. dispatch queue를 사용하면, 두 작업을 모두 serial dispatch queue에 추가해서 어떤 시점에 오직 하나의 작업이 공유 자원을 수정하는 것을 보장할 수 있다. 이런 큐 기반 동기화는 lock을 사용할 때보다 더 효과적인데, lock은 경쟁 상태이던, 비 경쟁 상태이던 상관 없이 항상 고 비용의 커널 트랩을 요구하기 때문이다. 반면 dispatch queue는 절대적으로 필요한 상황에만 커널 레벨로 내려가고 주로 앱의 프로세스 공간에서 작동한다.

serial queue에서 실행되는 두 개의 작업이 동시적이지 않다는 점을 지적할 수도 있겠지만, 기억해야 할 점은 두 스레드가 lock을 동시에 가진다면, 동시성은 의미가 없다. 더 중요한 것은 스레드 모델은 커널과 유저 공간의 메모리를 차지하는 두 개의 스레드의 생성을 요구한다는 점이다. dispatch queue는 사용하는 스레드에 대해 같은 메모리 패널티를 가지지 않고, 바쁘게 그리고 블락되지 않게 그들의 스레드를 사용한다.

dispatch queue에 대해 기억해야 할 몇 가지 주요 사항은 다음과 같다.

* dispatch queue는 그들의 작업을 다른 dispatch queue와 동시에 실행한다. 작업의 직렬화는 오직 해당 dispatch queue에만 적용된다. 
* 시스템이 동시에 실행될 작업의 전체 수를 결정한다. 그러므로 한 앱이 100개의 큐에서 100개의 작업을 실행시킬 때 모두 동시에 실행되지 않을 수 있다. (100개 이상의 효율적인 코어를 가지고 있지 않다면)
* 시스템이 어떤 작업을 새로 실행시킬 지 결정할 때 큐의 우선순위가 고려된다.
* 큐의 작업은 큐에 추가되는 시점에 실행될 준비가 되어야 한다. (cocoa operation을 사용해봤다면, 이 동작은 그 모델과는 다르게 동작한다는 것을 주의해라.)
* private dispatch queue는 reference-counted 객체이다. 게다가 코드에서 dispatch queue를 가지면(retain), dispatch source가 큐에 추가되고 retain count가 증가한다. 그러므로 모든 dispatch source가 취소되고 모든 retain call이 release call과 균형이 맞는 것을 보장해야 한다. 



### 큐와 관련된 기술들

dispatch queue에 추가적으로 Grand Central Dispatch는 큐를 사용한 몇 가지 기술들을 제공한다. 

| Technology          | Description                                                  |
| ------------------- | ------------------------------------------------------------ |
| Dispatch Groups     | dispatch group은 block 객체의 완료를 모니터링할 수 있는 방법이다. (동기적이거든 비동기적이든 상관 없이 필요에 따라.) 그룹은 다른 작업들의 완료에 기반한 유용한 동기화 메커니즘을 제공한다. |
| Dispatch Semaphores | dispatch semaphore는 전통적인 세마포어와 유사하지만 일반적으로 더 효율적이다. dispatch semaphore는 커널을 오직 세마포어가 사용 불가능(unavailable)할 때 스레드가 블락되어야 함을 알리기 위해 호출한다. 만약 세마포어가 사용 가능(available)하다면 커널 호출은 일어나지 않는다. |
| Dispatch sources    | dispatch source는 시스템 이벤트에 대한 알림(notification)을 생성한다. dispatch source는 프로세스 알림, signal, descriptor 이벤트 같은 이벤트들을 모니터링하기 위해 사용될 수 있다. 어떤 이벤트가 발생하면, dispatch source는 작업 코드를 비동기적으로 특정 dispatch queue에 실행시키기 위해 제출한다. |



### Block을 이용한 작업 구성

Block 객체는 C-기반 언어 특징으로 C, Objective-C, C++ 코드에서 사용할 수 있다. block을 이용하면 독립적인 작업 단위를 설정이 쉽다. function pointer와 비슷해보일 수 있지만, block은 사실 객체와 유사한 자료구조에 의해 표현되고 컴파일러가 이를 생성하고 관리한다. 컴파일러는 작성된 코드와 관련된 데이터를 포장하고 힙 영역에서 존재할 수 있고 앱 주변에 전달할 수 있는 형태로 캡슐화한다. 

block을 이용할 때의 주요 장점 중 하나는 block이 자기 자신의 lexical scope을 벗어난 영역의 변숟 사용할 수 있다는 것이다. block을 함수나 메서드에서 정의했을 때, block은 전통적인 코드 block의 역할과 유사한 역할을 한다. 예를  들자면 block은 부모 영역에서 정의된 변수의 값들을 읽을 수 있다. block에서 접근된 변수들은 힙 영역에 있는 block 자료 구조 내로 복사되고 그렇기 때문에 block은 후에 이 변수들에 접근할 수 있다. block들이 dispatch queue에 추가될 때, 이 값들은 읽기 전용 포맷으로 남아야 한다. 그러나 동기적으로 실행되는 block은 부모의 호출 영역에서 데이터를 리턴하는 것처럼 작동하는  `__block` 키워드를 포함한 변수를 이용할 수 있다.

function pointer를 이용할 때와 유사하게 block을 inline으로 선언할 수 있다. 두드러지는 차이는 block name은 caret(^)을 asterisk(*) 대신 이용한다는 것이다. function pointer와 같이 block에 인자(argument)를 전달할 수 있고, 리턴 값을 받을 수 있다. 아래 코드는 어떻게 block을 동기적으로 선언하고 실행시킬 수 있는지를 다룬다. `aBlock` 변수는 하나의 정수 값을 받아 아무 것도 리턴하지 않는 block으로 선언된다. 그 후 해당 프로토타입과 매칭되는 실제 block이 `aBlock`에 할당되고 inline으로 선언된다. 마지막 줄은 block을 즉시 실행하고, 명시된 정수를 표준 출력한다.

```objective-c
int x = 123;
int y = 456;

// Block declaration and assignment
void (^aBlock)(int) = ^(int z) {
  print("%d %d %d\n", x, y, z);
};

// Execute the block
aBlock(789); // prints: 123 456 789
```

다음은 block을 설계할 때 고려해야 할 주요 가이드라인에 대한 요약이다.

* dispatch queue를 이용해서 비동기적으로 수행하기를 원하는 block은 스칼라 값을 부모 함수나 메서드에서 캡쳐해서 block에서 사용하는 것이 안전하다. 그러나 큰 구조나 호출 context에서 할당되고 해제되는 포인터 기반 변수를 캡쳐하는 것은 피해라. block이 실행될 때, 포인터에 의해 참조된 메모리가 없을 수 있다. 물론, 메모리나 객체를 직접 할당하고 해당 메모리의 소유권을 명시적으로 block에 전달하는 것은 안전하다.
* dispatch queue는 추가된 block을 복사하고 block의 실행이 끝났을 때 block을 해제한다. 그러므로 큐에 추가하기 전에 명시적으로 block을 복사할 필요는 없다.
* 작은 작업을 수행하기에 스레드를 이용하는 것보다 큐가 효율적이지만 큐에서 block을 생성하고 실행시키는 데서 발생하는 오버헤드가 존재한다. 만약 block이 아주 작은 작업을 한다면 inline으로 실행시키는 것이 큐에 전달하는 것보다 저렴할 수 있다. 블록이 아주 작은 작업을 하는 지 확인하는 방법은 성능 도구를 사용해서 각 경로에 대한 metrics를 수집해서 비교하는 것이다.
* 기본 스레드와 관련된 데이터를 cache하지 말고, 다른 block에서 해당 데이터를 접근할 수 있을 것을 예상해라. 만약 같은 큐에 있는 작업들이 데이터를 공유할 필요가 있다면 dispatch queue의 context pointer를 이용해서 데이터를 저장해라. 어떻게 dispatch queue의 context data에 접근하는 지 알고 싶다면, [Storing Custom Context Information with a Queue](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/OperationQueues/OperationQueues.html#//apple_ref/doc/uid/TP40008091-CH102-SW13) 를 참고해라. 
* block이 Objective-C 객체를 여러 개 생성하는 경우, block 코드의 일부를 해당 객체들의 메모리 관리를 위해 @autorelease block에 추가시키고 싶을 수 있다. GCD dispatch queue가 자기 자신의 autorelease pool을 가지고 있지만, 이는 풀이 언제 비워지는 지 보장하지 않는다. 만약 앱이 메모리에 제약이 있다면, autorelease pool을 직접 생성해서 일정한 간격으로 autoreleased object의 메모리를 해제할 수 있게 해라.

block에 대해 더 궁금하면 [Blocks Programming Topics](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Blocks/Articles/00_Introduction.html#//apple_ref/doc/uid/TP40007502) 를 참고해라. 



### Dispatch Queue의 생성과 관리

작업을 큐에 추가하기 전에 어떤 타입의 큐를 사용할 지와 어떻게 사용할 지를 결정해야 한다. dispatch queue는 작업을 직렬적으로 또는 동시적으로 실행시킨다. 추가적으로 큐를 사용하는 특별한 사용 목적이 있는 경우 큐의 특성을 설정할 수 있다. 아래 섹션들은 어떻게 dispatch queue를 생성하고 설정하는 방법을 보여준다.



#### Global Concurrent Dispatch Queue를 이용하기

concurrent dispatch queue는 병렬적으로 실행시킬 수 있는 작업들을 가지고 있는 경우 유용하다. concurrent queue는 여전히 first-in, first-out 순서로 작업을 실행시키는 큐다. 그렇지만 concurrent queue는 이전 작업이 끝나기 전에 추가적인 작업을 시작한다. 앱의 상태가 변함에 따라 동적으로 어떤 시점에 concurrent queue에 의해 실행되는 전체 작업의 수는 달라진다. 많은 요소들이 concurrent queue에서 실행되는 작업의 수에 영향을 미친다. 사용 가능한 코어의 수, 다른 프로세스에 의한 작업의 수, 다른 serial dispatch queue의 작업들의 수와 우선 순위 등이 영향을 미치는 요소이다.

