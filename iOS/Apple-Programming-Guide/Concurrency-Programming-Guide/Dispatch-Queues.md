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

