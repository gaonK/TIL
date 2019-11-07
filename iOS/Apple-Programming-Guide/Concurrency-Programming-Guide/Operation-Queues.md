# Concurrency Programming Guide

동시성 프로그래밍 가이드

원문: [Concurrency Programming Guide](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/Introduction/Introduction.html)

## Operation Queues

코코아 operation은 비동기적으로 수행하고 싶은 작업을 캡슐화하는 객체지향적 방법이다. operation은 operation queue와의 결합 또는 그 자체로 이용되도록 설계되었다. Objective-C 기반이기 때문에, operation은 Cocoa 기반(OS X, iOS)의 앱에서 많이 쓰인다.

이번 장에서는 어떻게 operation을 정의하고 사용하는 지를 보여주겠다.



### Operation 객체

operatioin 객체는 `NSOperation` (Foundation 프레임워크) 클래스의 인스턴스이고 앱에서 수행되기를 원하는 작업을 캡슐화하는 데 쓰인다. `NSOperation` 클래스는 추상 클래스라 사용하기 위해서는 서브 클래스로 받아서 사용해야 한다. 추상 클래스 말고도, 상당한 양의 기반 구조를 통해 커스텀 서브 클래스를 만들 때 드는 작업을 줄여준다. 게다가, Foundation 프레임워크에서 두 개의 구체적인 서브 클래스를 제공하고 있어서 기존 코드와 함께 사용할 수 있다. 아래 테이블은 이 클래스들에 대한 요약과 어떻게 사용하는 지를 보여준다.



| Class                   | Description                                                  |
| ----------------------- | ------------------------------------------------------------ |
| `NSInvocationOperation` | 객체와 앱의 selector를 기반으로 하는 operation 객체를 만들기 위해 사용하는 클래스. 필요한 작업을 수행하는 메서드가 이미 있는 경우 이 클래스를 사용할 수 있다. 서브클래싱을 요구하지 않기 때문에, 이 클래스를 operation 객체를 동적으로 생성할 때 사용할 수 있다. |
| `NSBlockOperation`      | 여러 block 객체를 동시에 실행시키고 싶을 때 사용하는 클래스. 하나 이상의 block을 실행시킬 수 있기 때문에, block 객체는 그룹 시맨틱으로 작동한다. 관련되어 있는 모든 block이 완료되어야 해당 operation은 완료되었다고 여겨진다. block에 대해 더 알고 싶으면 [Blocks Programming Topics](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Blocks/Articles/00_Introduction.html#//apple_ref/doc/uid/TP40007502) 를 참고해라. |
| `NSOperation`           | 커스텀 operation 객체를 정의하는 기반 클래스이다. `NSOperation` 을 서브클래싱하는 것은 operation이 실행되는 기본 방식을 변경하는 능력과 상태를 보고하는 것을 포함해서 직접 만드는 operation 구현에 있어서 완전한 제어를 할 수 있게 한다. |

모든 operation 객체는 다음의 주요 기능을 지원한다.

* operation 객체 간의 그래프 기반 종속성 설정을 지원한다. 이 종속성은 주어진 operation이 종속된 operation들의 실행이 끝날 때까지 실행되지 않도록 한다. 
* operation의 메인 작업이 끝나고 실행되는 옵셔널 completion block을 지원한다. 
* KVO notification을 이용해 operation의 실행 상태 변화를 모니터링하는 것을 지원한다. KVO notification을 어떻게 관찰하는 지 알고 싶다면 [Key-Value Observing Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html#//apple_ref/doc/uid/10000177i) 를  봐라.
* operation들의 우선순위를 지정해서 관련된 실행 순서들에 영향을 준다. 
* operation이 실행 중일 때 정지 시킬 수 있도록 하는 취소 시맨틱을 지원한다. 



Operation은 앱의 동시성 수준을 향상시키기 위해 설계되었다. operation은 또한 앱의 행동을 간단한 별개의 덩어리(chunk)로 구조화하고, 캡슐화시키기 좋은 방법이다. 앱의 메인 스레드에서 약간의 코드를 실행시키기 보다 하나 이상의 operation 객체를 큐에 제출하고 관련된 작업들을 하나 이상의 독립된 스레드에서 비동기적으로 수행시켜라.



### Concurrent vs. Non-concurrent Operations

보통 operation을 operation queue에 추가함으로써 실행시키지만 그렇게 하는 것이 필수적인 것은 아니다. operation 객체를 `start` 메서드를 호출해 수동으로 실행시키는 것 또한 가능하다. 하지만 이렇게 하는 것은 operation이 나머지 코드에서 동시적으로 실행됨을 보장할 수 없다.  `NSObject` 클래스의 `isConcurrent` 메서드는 operation이 동기적으로 실행되어야 할 지, 비동기적으로 실행되어야 할 지 `start` 메서드가 스레드에서 호출되었느냐에 따라 알려 준다. 기본적으로 이 메서드는 `NO`를 반환하며 이는 이 operation이 동기적으로 호출 스레드에서 실행됨을 의미한다.

만약 호출 스레드와 관련해서 비동기적으로 작동되는 동시적인 operation을 구현하고 싶다면,  operation을 비동기적으로 시작시키기 위한 추가적인 코드를 작성해야 한다. 예를 들어, 작업이 끝나기 전에, 구분된 스레드를 생성하거나, 비동기적 시스템 함수를 호출하거나, `start` 메서드가 작업을 시작하고 즉각적으로 반환하게 하는 것을 보장하게 하는 어떠한 것들을 하는 것이 있다.

대부분의 개발자들은 동시적인 operation 객체를 구현할 필요가 없다. 항상 operation을 operation queue에 추가한다면, 동시적인 operation을 구현할 필요가 없다. 동시적이지 않은 operation을 operation queue에 제출한다면, 큐는 스스로 operation을 실행시킬 스레드를 생성한다. 그러므로, 동시적이지 않은 operation을 operation queue에 추가하는 것은 여전히 operation 객체 코드를 비동기적으로 실행시킴을 의미한다. 동시적 operation을 정의할 수 있는 능력은 오직 직접 operation을 operation queue에 추가하지 않고 비동기적으로 실행시키기를 원할 때만 신경 쓰면 된다. 



### NSInvocationOperation 객체 생성

`NSInvocationOperation` 클래스는 `NSOperation`의 구체적인 서브클래스로, 실행시킬 때 객체를 명시할 때 같이 명시한 selector를 부른다. 앱에서 작업에 대한 custom 객체들을 많이 정의하는 것을 피하기 위해 이 클래스를 사용해라. 특히 필요한 작업을 수행하기 위한 객체와 메서드를 가지고 있는 앱을 수정하고 싶다면 이 클래스를 사용해라. 또한 호출하기 원하는 메서드가 환경에 따라 변할 수 있을 때 이용하라. 예를 들어, 유저 입력에 따라 동적으로 변하는 selector를 수행하기 위해 invocation operation을 사용할 수 있다.

invocation operation을 만드는 프로세스는 간단하다. 클래스의 새로운 인스턴스를 생성하고, 원하는 객체와 실행할 selector를 초기화 메서드에 전달해서 초기화한다. 아래 코드는 생성 프로세스를 보여주는 커스텀 클래스의 두 메서드를 보여준다. `taskWithData:` 메서드는 새로운 invocation object를 생성하고 작업 실행을 포함하는 다른 메서드의 이름을 제공받는다.

```objective-c
@implementation MyCustomClass
- (NSOperation*)taskWithData:(id)data {
    NSInvocationOperation* theOp = [[NSInvocationOperation alloc] initWithTarget:self
                    selector:@selector(myTaskMethod:) object:data];
 
   return theOp;
}
 
// This is the method that does the actual work of the task.
- (void)myTaskMethod:(id)data {
    // Perform the task.
}
@end

```



### NSBlockOperation 객체 생성

`NSBlockOperation` 객체는 `NSOperation`의 구체적인 서브클래스로, 하나 이상의 block 객체의 wrapper 역할을 한다. 이 클래스는 이미 operation queue를 사용하고 있으면서 dispatch queue를 생성하고 싶지 않은 앱에게 객체지향적인 wrapper를 제공한다. 또한 operation 종속성, KVO notification, 기타 dispatch queue에서 가능하지 않은 다른 기능들의 이점을 취하기 위해 block operation을 사용할 수도 있다.  

block operation을 생성할 때, 일반적으로 한 개 이상의 block을 초기화 타이밍에 추가한다. 나중에 더 많은 block이 필요한 경우 추가할 수 있다. `NSBlockOperation` 객체를 실행할 때, 객체는 모든 block을 기본 우선순위(default-priority)로 concurrent dispatch queue에 제출한다. 객체는 모든 block의 실행이 완료될 때까지 기다린다. 마지막 block의 실행이 완료되면, operation 객체는 자기 자신을 완료되었다고 체크한다. 그렇기 때문에 block operation을 이용하면 스레드 조인을 이용해서 여러 스레드의 결과를 병합하는 것처럼, 실행되는 block들을 트랙킹할 수 있다. 차이점은 block operation은 별도의 스레드에서 실행되므로 앱의 다른 스레드들은 block operation이 완료되기를 기다리는 동안에도 작업을 수행할 수 있다.

아래의 간단한 에제는 어떻게 `NSBlcokOperation` 객체를 생성하는지 보여준다. block은 파라미터와 반환 값을 가지지 않는다.

```objective-c
NSBlockOperation* theOp = [NSBlockOperation blockOperationWithBlock: ^{
      NSLog(@"Beginning operation.\n");
      // Do some work.
   }];
```

block operation 객체를 생성한 후, `addExecutionBlock:` 을 이용해 더 많은 block을 추가할 수 있다. block을 직렬로 실행시키고 싶다면, block을 곧바로 dispatch queue에 제출해야 한다.



### Defining a Custom Operation Object

만약 block operation과 invocation operation 객체가 앱에 필요한 기능을 충족시키지 못할 때, `NSOperation`을 직접 서브클래싱해서 원하는 행위를 추가할 수 있다. `NSOperation` 클래스는 모든 operation 객체에 대한 일반적인 서브클래싱 지점을 제공한다. 또한 이 클래스는 종속성, KVO notification과 관련된 작업들을 제어할 수 있는 상당한 양의 기반 인프라를 제공한다. 그러나, 여전히 operation이 원하는 기능을 수행하게 하기 위해서 기존의 기반 인프라를 보완해야 할 필요가 있다. 추가적으로 해야 할 일의 양은 구현하려는 operation이 비동시적인지, 동시적인지에 따라 달라진다.

비동시적인 operation을 정의하는 것은 동시적인 operation을 정의하는 것에 비해 훨씬 간단하다. 비동시적인 operation에 있어서, 해야 할 것은 메인 작업을 수행하고, 취소 이벤트에 적절하게 반응하는 것이다. 존재하는 클래스의 기반 인프라는 다른 일들을 처리해 줄 것이다. 동시적인 operation의 경우, 기반 인프라를 커스텀 코드를 이용해서 변경할 필요가 있다. 아래 섹션들에서 어떻게 이 두 가지 타입의 객체를 구현하는지 보여주겠다.



#### 메인 작업 수행

최소한, 모든 operation은 다음 메서드를 구현해야 한다.

* 커스텀 초기화 메서드
* `main`

operation 객체를 알려진 상태에 두고, 커스텀 메인 메서드를 작업 수행을 위해 사용하기 위해서 커스텀 초기화 메서드가 필요하다. 물론 다음과 같이 필요한 다른 추가적인 메서드들도 추가할 수 있다.

* `main` 메서드의 수행에서 호출하고자 하는 커스텀 메서드
* 데이터 값을 설정하고, operation의 결과에 접근하는 접근 메서드 
* 아카이브와 언아카이브 객체의 저장과,  `NSCoding` 프로토콜의 메서드

아래 코드는 커스텀 `NSOperation` 서브클래스를 위한 시작 템플릿이다. (이 코드는 어떻게 취소를 핸들링하는지 보여주지 않지만 가져야 할 메서드를 보여준다.) 이 클래스의 초기화 메서드는 단일 객체를 데이터 파라미터로 사용하고 레퍼런스를 operation 객체의 내부에 저장한다. `main` 메서드는 결과를 다시 앱으로 반환하기 전에 해당 데이터 객체에서 작동한다.

```objective-c
@interface MyNonConcurrentOperation : NSOperation
@property id (strong) myData;
-(id)initWithData:(id)data;
@end
 
@implementation MyNonConcurrentOperation
- (id)initWithData:(id)data {
   if (self = [super init])
      myData = data;
   return self;
}
 
-(void)main {
   @try {
      // Do some work on myData and report the results.
   }
   @catch(...) {
      // Do not rethrow exceptions.
   }
}
@end
```

 `NSOperation` 서브클래스를 구현하는 더 자세한 예제를 보고 싶다면, [NSOperationSample](https://developer.apple.com/library/archive/samplecode/NSOperationSample/Introduction/Intro.html#//apple_ref/doc/uid/DTS10004184) 을 확인해라.



#### 취소 이벤트에 대한 응답

operation이 실행되고 난 이후에, operation은 계속 작업들이 완료되거나 operation을 취소하기 전까지 작업을 수행한다. 취소는 어느 때나 발생할 수 있고, 심지어 operation이 실행되기 전에도 일어날 수 있다. `NSOperation` 클래스가 클라이언트에게 operation을 취소할 수 있는 방법을 제공하지만, 취소 이벤트를 감지하는 것은 필요에 의해 자발적으로 이루어진다. 만약 operation이 완전히 종료되었다면, 할당된 리소스를 회수할 수 있는 방법이 없을 수 있다. 그 결과 operation 객체는 취소 이벤트 검사를 통해 operation 중간에 취소할 때 정상적으로 종료하기를 요구받는다.

operation 객체에서 취소를 지원하기 위해서, 해야 할 것은 객체의 `isCancelled` 메서드를 커스텀 코드를 주기적으로 호출하고, `YES` 를 반환한다면 즉각적으로 반환하는 것이다. operation의 지속 시간이나 구체적인 서브 클래스를 사용하던, 커스텀 서브 클래스를 사용하는지와 관계 없이 취소를 지원하는 것은 중요하다. `isCancelled` 메서드는 아주 가볍고, 성능상 패널티 없이 자주 호출하는 것이 가능하다. operation 객체를 설계할 때, 아래 상황에서 `isCancelled` 메서드를 호출하는 것을 고려해라.

* 실제 작업을 수행하기 바로 전
* 반복문 안에서 각 반복마다 한 번 이상, 만약 반복이 길다면 더 자주
* operation 중단이 비교적 쉬운 어디에서든

아래 코드는 `main` 메서드에서의 취소 이벤트에 어떻게 응답할 지에 대한 간단한 예제이다. 이 경우, `isCancelled` 메서드는 `while` 문의 각 반복 안에서 한 번씩 호출 되고, 작업이 시작되기 전 정기적으로 빠른 종료가 가능하다.

```objective-c
- (void)main {
   @try {
      BOOL isDone = NO;
 
      while (![self isCancelled] && !isDone) {
          // Do some work and set isDone to YES when finished
      }
   }
   @catch(...) {
      // Do not rethrow exceptions.
   }
}
```

위의 예제에서는 리소스를 해제하는 코드가 없지만, 직접 코드를 작성할 때는 할당한 모든 리소스를 해제해야 된다.



#### 동시적 실행을 위한 operation 설정

operation 객체는 기본적으로 동기적으로 실행된다. 그렇기 때문에 operation의 작업은 `start` 메서드를 호출하는 스레드에서 수행된다. operation queue가 비동시적인 operation을 위한 스레드를 제공하고 있지만, 대부분의 operation은 여전히 비동기적으로 실행된다. 그러나, 만약 operation들을 직접 비동기적으로 실행시키길 원한다면,  operation 객체를 동시적인 operation 객체로 정의하고자 할 때 적합한 행위(action)을 하도록 보장해야 한다.

##### 동시적인 operation을 위해 override 해야 하는 메서드

| Method                       | Description                                                  |
| ---------------------------- | ------------------------------------------------------------ |
| `start`                      | (필수) 모든 동시적인 operation은 이 메서드를 override하고, 기본 구현 대신 커스텀 구현으로 대체해야 한다. operation을 직접 실행시키기 위해서, `start` 메서드를 호출해야 한다. 그러므로, 이 메서드를 구현은 operation의 시작점이고, 스레드나 다른 작업을 실행시키기 위해 필요한 실행 환경을 구성해야할 곳이다.  커스텀 구현을 할 때에는 `super` 를 부르지 말아라. |
| `main`                       | (선택) 이 메서드는 보통 operation 객체와 관련된 작업을 실행시킬 때 사용된다. 작업을 `start` 메서드에서 수행할 수도 있지만, 이 메서드를 이용해서 구현하면 설정 코드와 작업 코드를 분리하는 결과를 얻을 수 있다. |
| `isExecuting` / `isFinished` | (필수) 동시적인 operation은 실행 환경을 설정하고, 환경의 상태를 외부의 클라이언트에게 보고할 책임이 있다. 그러므로, 동시적인 operation은 언제 작업을 실행할 지, 언제 작업이 끝날 지를 알기 위해서 반드시 상태 정보를 유지해야 한다. 그리고 상태를 이 메서드를 이용해서 반드시 보고해야 한다. 이 메서드를 구현하는 것은 다른 스레드에서의 동시 호출에 안전해야 한다. 또한 반드시 적합한 KVO notification을 생성해서 이 메서드들에 의해 보고되는 값을 확인해야 한다. |
| `isConcurrent`               | (필수) 해당 operation이 동시적인 operation인지 확인하기 위해서, 이 메서드를 override하고 `YES` 를 반환하라. |

남은 섹션에서는 동시적 operation을 구현하기 위해 필요한 기초적인 코드를 보여주는 `MyOperation` 클래스의 구현 예제를 볼 것이다. `MyOperation` 클래스는 단순히 생성한 각각의 스레드의 `main` 메서드를 이용해 실행한다. 실제 `main` 메서드가 수행하는 작업과는 다르다. 이 예제에서 중요한 포인트는, 동시적인 operation을 정의하기 위해 제공해야 하는 기반 구조를 보여주는 것이다. 

아래 코드는 `MyOperation` 클래스의 인터페이스와 구현의 일부분이다. `isConcurrent`, `isExecuting`, `isFinished` 메서드의 구현은 상대적으로 직접적이다. `isConcurrent` 메서드는 간단하게 이 operation이 동시적이라는 것을 알리기 위해 `YES` 를 반환한다. `isExecuting`과 `isFinished` 메서드는 단순히 클래스에 저장되어 있는 인스턴스 변수를 반환한다.

```objective-c
@interface MyOperation : NSOperation {
    BOOL        executing;
    BOOL        finished;
}
- (void)completeOperation;
@end
 
@implementation MyOperation
- (id)init {
    self = [super init];
    if (self) {
        executing = NO;
        finished = NO;
    }
    return self;
}
 
- (BOOL)isConcurrent {
    return YES;
}
 
- (BOOL)isExecuting {
    return executing;
}
 
- (BOOL)isFinished {
    return finished;
}
@end
```



아래 코드는 `MyOperation`의 `start` 메서드를 보여준다. 이 메서드의 구현은 꼭 수행해야 하는 작업들을 보여주기 위해 최소화되어 있다. 이 경우, 메서드는 단순히 새로운 스레드를 실행하고, `main` 메서드에서 호출하도록 설정한다. 또한 메서드는 `executing` 멤버 변수를 업데이트하고, `isExecuting` 키패스에 대한 KVO notification을 생성해서 값이 변함을 나타낸다. 이 작업이 끝나면, 메서드는 단순히 반환되어 새롭게 분리된 스레드가 실제 작업을 수행하게 한다.

```objective-c
- (void)start {
   // Always check for cancellation before launching the task.
   if ([self isCancelled])
   {
      // Must move the operation to the finished state if it is canceled.
      [self willChangeValueForKey:@"isFinished"];
      finished = YES;
      [self didChangeValueForKey:@"isFinished"];
      return;
   }
 
   // If the operation is not canceled, begin executing the task.
   [self willChangeValueForKey:@"isExecuting"];
   [NSThread detachNewThreadSelector:@selector(main) toTarget:self withObject:nil];
   executing = YES;
   [self didChangeValueForKey:@"isExecuting"];
}
```

아래 코드는 `MyOperation` 클래스의 남은 구현을 보여준다. 위에서 본 것처럼, `main` 메서드는 새로운 스레드의 시작 포인트이다. operation 객체와 관련된 작업들을 수행하고, 작업이 끝났을 때, 커스텀 `completeOperation` 메서드를 호출한다. `completeOperation` 메서드는 필요한 KVO notification을 `isExecuting`과 `isFinished` 키패스를 위해 생성하고, operation의 상태가 변하면 반영한다.

```objective-c
- (void)main {
   @try {
 
       // Do the main work of the operation here.
 
       [self completeOperation];
   }
   @catch(...) {
      // Do not rethrow exceptions.
   }
}
 
- (void)completeOperation {
    [self willChangeValueForKey:@"isFinished"];
    [self willChangeValueForKey:@"isExecuting"];
 
    executing = NO;
    finished = YES;
 
    [self didChangeValueForKey:@"isExecuting"];
    [self didChangeValueForKey:@"isFinished"];
}
```

만약 operation이 취소되더라도, KVO observer에게 operation이 작업을 끝냈음을 항상 알려야 한다. operation 객체가 다른 operation 객체의 완료에 종속성이 있는 경우, 그 객체의 `isFinished` 키를 모니터링한다. 모든 객체가 실행이 완료되었다고 알리는 경우에만 종속적인 객체가 실행될 준비가 되었다는 신호를 준다. 그러므로 완료 notification을 생성하는 것에 실패한다면 앱의 다른 operation의 실행이 막힐 수 있다.



#### KVO 준수를 유지하기

`NSOperation` 클래스는 다음 키패스를 준수하는 key-value observing(KVO)이다.

* `isCancelled`
* `isConcurrent`
* `isExecuting`
* `isFinished`
* `isReady`
* `dependencies`
* `queuePriority`
* `completionBlock`

`NSOperation`의 `start` 메서드를 override 했거나, 다른 주요한 커스텀을 했다면 반드시 커스텀 객체가 KVO를 이 키패스들에 대해서 KVO를 준수함을 보장해야 한다. `start` 메서드를 override 할 때, 가장 신경써야 할 키패스는 `isExecuting`과 `isFinished`이다. 이 키패스들은 다시 구현을 할 때 제일 영향을 많이 받는다. 

만약 종속성을 구현하거나, 다른 operation과 관련된 것을 구현한다면, `isReady` 메서드를 override할 수 있고, 커스텀 종속성이 만족될 때까지 강제로 `NO`를 반환하도록 할 수 있다. (만약 커스텀 종속성을 구현했을 때, 여전히 `NSOperation`의 기본 종속성 관리 시스템을 지원한다면 `isReady` 메서드에서 `super`를 호출해라.) 객체의 준비 상태가 변경된다면 `isReady` 키의 KVO notification을 생성해서 변경사항을 보고해라. `addDependency:`나 `removeDependency:`를 override하지 않았더라도 `depencencies` 키패스에 대한 KVO notification을 만드는 데 걱정할 필요는 없다.

또한 그렇게 할 필요는 없지만 `NSOperation`에 대한 다른 키패스를 위한 KVO notificiation도 생성할 수 있다. operation을 취소할 필요가 있다면, 이미 존재하는 `cancel` 메서드를 호출하면 된다. 비슷하게 operation 객체 안의 큐의 우선순위 정보를 변경할 필요는 거의 없다. 마지막으로, operation이 동적으로 동시성 상태를 변경해야 할 필요가 없다면, `isConcurrent` 키패스에 대해 KVO notification을 제공할 필요는 없다.

key-value observing에 대한 더 많은 정보가 필요하거나 어떻게 커스텀 객체에서 지원할 지를 알고 싶다면 [Key-Value Observing Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html#//apple_ref/doc/uid/10000177i) 를 봐라.



### Operation의 실행 행위 커스텀하기

operation 객체의 설정은 객체를 생성하고, 객체를 큐에 추가하기 전에 발생한다. 이 섹션에서 설명되는 설정 타입은 `NSOperation` 클래스를 직접 서브 클래싱했던, 이미 존재하는 서브 클래스를 사용하는지와 무관하게 모든 operation 객체에 적용될 수 있다. 

#### operation 간의 종속성 설정

종속성은 operation 객체를 직렬로 실행할 수 있게 하는 방법이다. 다른 operation들에 종속적인 operation은 종속된 다른 모든 operation들이 완료될 때까지 실행될 수 없다. 간단한 일대일 종속성을 만들 수도 있고, 복잡한 객체의 종속성 그래프를 만들 수도 있다. 

두 operation 객체 간의 종속성을 만들기 위해서는, `NSObject`의 `addDependency:` 메서드를 사용한다. 이 메서드는 현재 operation에서 파라미터로 넘긴 타겟 operation으로의 일방적인 종속성을 만든다. 이 종속성은 현재 operation은 타겟 객체의 실행이 완료되기 전까지 실행될 수 없음을 의미한다. 종속성은 같은 큐에 있는 다른 operation들의 영향을 받지 않는다. operation 객체는 자신의 종속성을 관리하기 때문에 operation 간에 종속성을 만들어서 서로 다른 큐에 넣는 것도 허용된다. 한 가지 허용되지 않는 것은 operation 간에 순환 종속성을 생성하는 것이다. 이것은 개발자가 만들어내는 에러로, operation이 영원히 실행되지 못하게 할 것이다.

operation의 모든 종속성이 실행이 완료되었을 경우, operation 객체는 일반적으로 실행 준비 상태가 된다. (`isReady` 메서드의 행위를 변경한다면 operation이 준비되었는지는 커스텀된 대로 결정된다.) operation 객체가 큐에 있으면, 큐는 operation을 언제든지 실행시킬 것이다. 직접 operation을 실행시키고자 한다면, operation의 `start` 메서드를 호출하는 것에 달려있다.

> 항상 operation을 실행시키기 전, operation을 operation queue에 넣기 전에 종속성을 설정해야 한다. 이후에 추가된 종속성은 해당 operation이 실행되지 않게 할 수도 있다.

각각의 operation 객체가 의존하는 종속성은 객체의 상태가 변할 때, 적절한 KVO notification을 보낸다. 만약 객체의 행위를 커스텀했다면, 종속성과 관련된 문제들을 일으키지 않기 위해 적절한 KVO notification을 생성해야 한다. 



#### Operation의 실행 우선순위 변경

큐에 추가된 operation의 경우, 실행 순서는 우선적으로 큐에 있는 operation이 준비가 됐는지에 달려있고, 그 다음 상대적인 우선순위에 달려있다. operation이 준비가 되었는 지는 다른 operation에 대한 종속성에 의해 결정되지만, 우선순위 레벨은 operation 객체 자기 자신의 요소이다. 새로 생성된 operation object는 기본적으로 "normal" 우선순위를 가지고, `setQueuePriority:` 메서드를 이용해 우선순위를 증가시키거나 감소시킬 수 있다.

우선순위 레벨은 같은 operation queue에 있는 operation들에 한해서 적용된다. 앱이 여러 개의 operation queue를 가지고 있다면, 각각의 operation queue는 자기 자신의 operation들의 우선순위를 독립적으로 매긴다. 그렇기 때문에 서로 다른 큐에 있는 낮은 우선순위의 operation이 높은 우선순위의 operation 보다 먼저 실행되는 것이 가능하다.

우선순위 레벨은 종속성의 대체제가 아니다. 우선순위는 이미 준비가 되어 있는 operation들에 대해서만 순서를 정한다. 예를 들어 어떤 큐가 높은 우선순위와 낮은 우선순위의 operation을 가지고 있고 두 operation 모두 준비가 되었을 때, 큐는 높은 우선순위의 operation을 먼저 실행시킨다. 하지만 높은 우선순위의 operation이 준비가 되어 있지 않았고, 낮은 우선순위의 operation은 준비가 되어 있다면, 큐는 낮은 우선순위의 operation을 먼저 실행시킨다. 다른 operation이 끝날 때까지 현재 operation을 실행시키지 않은 경우라면, 종속성을 이용해야 한다.



#### 기본 스레드 우선순위 변경

OS X v10.6 이후, operation의 기본 스레드의 작업 우선순위를 설정하는 것이 가능해졌다. 시스템의 스레드 정책은 커널에 의해 관리되지만, 일반적인 높은 우선순위의 스레드는 낮은 우선순위의 스레드보다 더 많이 실행될 수 있는 기회를 갖는다. operation 객체에서, 스레드 우선순위를 floating-point 값 0.0 ~ 1.0으로 명시할 수 있다. 스레드의 우선순위를 명시하지 않으면 operation은 우선순위가 0.5로 설정된 기본 스레드에서 실행된다. 

operation의 스레드 우선순위를 설정하기 위해서는 operation의 `setThreadPriority:` 메서드를 큐에 추가하기 전에 실행해야 한다. (또는 직접 실행시켜야 한다.) operation이 실행될 때, 기본 `start` 메서드는 명시된 값을 이용해 현재 스레드의 우선순위를 변경한다. 새로운 우선순위는 operation의 `main` 메서드가 유지되는 동안만 유지된다. 다른 코드들(operation의 completion block을 포함)는 기본 스레드 우선순위에서 실행된다. 동시적인 operation을 생성하고 `start` 메서드를 override하면, 반드시 스레드 우선순위를 설정해야 한다.



#### Completion Block 설정

OS X v10.6 이후, operation은 메인 작업이 완료된 후 completion block을 실행시킬 수 있게 되었다. 메인 작업이 아니라고 여겨지는 어떤 작업이든 completion block이 수행하게 할 수 있다. 예를 들어, 이 block을 사용해서 관심있는 클라이언트에게 operation이 완료되었음을 알릴 수 있다. 동시적인 operation 객체는 이 block을 최종적인 KVO notification을 만드는 데 사용할 수 있다. 

completion block을 설정하기 위해 `NSOperation`의 `setCompletionBlock:` 메서드를 이용해라. 이 메서드에 전달하는 block은 전달 인자나 반환 값을 가지지 않는다. 



### Operation 객체의 메모리 관리

이어지는 섹션에서는 operation 객체에서의 좋은 메모리 관리법에 대해 다룰 것이다. Objective-C 프로그램의 메모리 관리에 대한 일반적인 정보는 [Advanced Memory Management Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/MemoryMgmt/Articles/MemoryMgmt.html#//apple_ref/doc/uid/10000011i)에서 확인하라.



#### 스레드 저장소를 피하라

대부분의 operation이 스레드에서 실행되지만, 동시적이지 않은 operation의 경우 스레드는 operation queue에 의해 제공된다. 만약 operation queue가 스레드를 제공한다면 스레드가 operation이 아닌 operation queue에 의해 관리되도록 하는 것을 고려해야 한다. 특히, 직접 만들고 관리하지 않는 스레드와 어떤 데이터도 연관지어서는 안 된다. operation queue에 의해 관리되는 스레드는 시스템과 앱의 요구에 따라 왔다 갔다한다. 그러므로 operation 간 데이터 전송에 스레드 저장소를 이용하는 것은 신뢰할 수 없고 실패하기 쉽다. 

operation 객체의 경우 스레드 저장소를 이용할 어떤 이유도 없다. operation 객체를 초기화할 때, 작업을 하기 위해 필요한 모든 것을 제공해야 한다. 그러므로, operation 객체 그 자체로 필요한 문맥상의 저장소를 제공한다. 모든 나가고 들어오는 데이터는 앱에 통합되기 전까지 혹은 더 이상 필요하지 않을 때까지 그 곳에 저장된다.



#### 필요하다면 operation 객체를 참조하라

operation 객체가 비동기적으로 실행되기 때문에, operation 객체를 생성하고 잊어버려도 된다고 생각하면 안 된다. 여전히 operation은 객체일 뿐이며, 코드에 필요한 참조를 관리하는 것은 개발자에게 달려있다. operation이 완료된 후 결과 데이터를 얻어야 하는 경우 특히 더 중요하다.

operation에 대한 참조를 계속 유지해야 하는 이유는 큐에게 객체에 대해 물을 기회가 없을 수도 있기 때문이다. 큐는 operation을 디스패치하고, 실행시키는 일을 최대한 빨리 하는 데 모든 노력을 기울인다. 많은 경우, 큐는 operation들이 추가된 후 거의 즉시 operation들을 실행시킨다. 작성하는 코드에서 큐로부터 operation에 대한 참조를 얻으려고 할 때, operation이 이미 큐에서 완료되어 제거되었을 수 있다. 



### 에러와 예외 처리

operation은 기본적으로 앱의 개별적인 개체이기 때문에, 에러와 발생하는 예외를 처리해야 할 의무가 있다. OS X v.10.6 이후, `NSObject`의 기본 `start` 메서드는 예외를 catch하지 않는다. (이전 버전에서는 예외 처리를 했다.) 코드는 항상 예외를 직접 처리할 수 있어야 한다. 또한 에러 코드를 체크하고 필요하다면 앱의 적절한 부분에 알려줘야 한다. `start` 메서드를 바꿨다면, 예외를 커스텀 구현에서 잡아서 기본 스레드의 범위를 벗어나지 않게 해야 한다.

에러 처리를 하기 위해 미리 준비해야 하는 에러 상황은 다음과 같다.

* UNIX `errno`-style 에러 코드
* 메서드나 함수로부터 반환된 명확한 에러 코드
* 직접 작성한 코드나 다른 시스템 프레임워크에서 던져진 예외
* `NSOperation` 클래스에서 던져진 예외
  * operation이 준비되지 않았지만 `start` 메서드가 불려짐
  * operation이 실행중이거나 끝났는데 `start` 메서드가 다시 불려짐 (취소되었기 때문일 수 있다.)
  * 이미 실행중이거나 끝난 operation에 completion block을 추가함
  * 이미 취소된 `NSInvocationOperation` 객체의 결과를 얻으려 함

만약 커스텀 코드가 예외나 에러를 만났다면, 에러를 나머지 앱에 전달해야 한다. `NSOperation` 클래스는 에러 결과 코드나 예외를 앱의 다른 부분으로 전달하는 구체적인 메서드를 제공하고 있지 않다. 그러므로 중요한 정보일 경우 필요한 코드를 제공해야 한다.



### Operation 객체의 적절한 범위 설정

임의의 많은 수의 operation을 하나의 operation queue에 추가하는 것이 가능하지만 이는 실용적이지 못하다. 다른 객체들과 마찬가지로, `NSOperation` 클래스의 인스턴스는 메모리를 소모하고, 실행과 관련된 실제 비용이 든다. 각각의 operation 객체가 아주 작은 양의 작업을 하고 아주 많은 수의 operation을 생성하면, 실제 작업보다 더 많은 디스패치 시간을 소비하게 된다. 만약 앱이 이미 메모리 제약이 있다면, 메모리에 있는 아주 많은 수의 operation 객체는 성능을 저하시킬 것이다.

operation을 효율적으로 사용하기 위한 열쇠는 필요한 작업의 양과 컴퓨터를 바쁘게 작동시키는 것 사이에서 균형을 찾는 것이다. operation이 합리적인 양의 작업을 하도록 해라. 예를 들면, 앱이 100개의 operation 객체를 생성하고 다른 100개의 값을 이용해 작업을 수행한다고 하면, 10개의 operation을 만들어 10개의 값들을 처리하도록 해봐라.

또한 한 번에 큐에 너무 많은 수의 operation을 추가하는 것을 피하고 처리하는 것보다 더 빠른 속도로 operation 객체를 큐에 넣는 것을 피하라. 그러기보다는, 객체를 batch로 생성하라. 하나의 batch가 실행을 완료하면, completion block을 이용해서 앱에게 새로운 batch를 생성하라고 알린다. 해야 할 작업이 많아서 큐를 operation으로 가득 채워 컴퓨터를 바쁘게 작업하도록 하고 싶어도, 너무 많은 operation을 생성해서 앱의 메모리가 모자라는 현상을 바라지 않을 것이다.

당연히 생성하는 operation의 수와 수행하는 작업의 양은 전적으로 앱에 달려있다. Instruments 같은 툴을 이용해서 효율성과 속도의 적절한 균형을 찾아라. Instruments와 다른 성능 툴에 관해서 더 알고 싶다면 [Performance Overview](https://developer.apple.com/library/archive/documentation/Performance/Conceptual/PerformanceOverview/Introduction/Introduction.html#//apple_ref/doc/uid/TP40001410)를 읽어라.



### Operation의 실행

궁극적으로 앱은 operation을 작업을 처리하기 위해 실행시킬 필요가 있다. 이 섹션에서 operation을 실행시키는 몇 가지 방법과 런타임에 operation 실행을 조작하는 방법에 대해 설명한다.



#### Operation을 Operation Queue에 추가

operation을 실행시키는 가장 쉬운 방법은 `NSOperationQueue`의 인스턴스인 operation queue를 이용하는 것이다. 앱은 어떤 operation queue던지 사용하기 원하는대로 생성하고, 유지할 책임이 있다. 앱은 가지고 싶은 만큼 큐를 가질 수 있지만, 특정 시점에 실행될 수 있는 operation의 양에 있어서 실질적인 한계는 존재한다. operation queue는 시스템과 함께 동작하며 사용 가능한 코어와 시스템 로드에 적절하게 동시적인 operation의 수를 제한한다. 그러므로, 추가적인 큐를 생성하는 것이 추가적인 operation들을 실행시킬 수 있다는 뜻이 될 수 없다.

큐를 생성하기 위해서 다른 객체를 생성할 때 같이 앱에서 큐를 할당해줘야 한다.

```objective-c
NSOperationQueue* aQueue = [[NSOperationQueue alloc] init];
```

operation을 큐에 추가하기 위해서 `addOperation:` 메서드를 이용한다. OS X v10.6 이후, operation의 그룹을 `addOperations:waitUntilFinished:`를 이용하거나 추가하거나 `addOperationWithBlock` 메서드를 이용해 block object를 이용해(상응하는 operation 객체 없이) 바로 큐에 추가하는 것이 가능해졌다. 이 메서드들은 각각 operation을 큐에 넣고, operation을 처리해야 함을 큐에 알린다. 대부분의 경우 operation은 거의 큐에 추가되자마자 실행되지만 operation queue는 몇 가지 이유로 추가된 operation들을 지연시킬 수 있다. 특히, 아직 완료되지 않은 다른 operation에 종속성이 있는 경우 operation의 실행은 지연될 수 있다. 다른 경우로는 operation queue가 suspended 되었거나, 이미 실행할 수 있는 최대치의 동시적 operation을 실행하고 있는 경우가 있다. 아래 예제는 operation을 큐에 추가하는 간단한 문법을 보여준다.

```objective-c
[aQueue addOperation:anOp]; // Add a single operation
[aQueue addOperations:anArrayOfOps waitUntilFinished:NO]; // Add multiple operations
[aQueue addOperationWithBlock:^{
   /* Do something. */
}];
```

반드시 필요한 설정과 수정사항을 operation 객체가 큐에 추가되기 전에 만들어야 한다. 한 번 추가되고 나면, operation은 언제든지 실행될 수 있기 때문에 원하는 변화를 주려고 하는 때는 너무 늦은 때일 수 있기 때문이다.

`NSOperationQueue` 클래스가 operation의 동시적 실행을 위해 설계되었지만, 하나의 큐가 오직 하나의 operation만 동시에 실행되도록 하는 것도 가능하다. `setMaxConcurrentOperationCount:` 메서드는 operation queue의 최대 동시적 operation의 수를 설정할 수 있게 해준다. 이 메서드에 1을 넘기게 되면 큐는 한 번에 오직 하나의 operation 객체만을 실행시킨다. 비록 한 번에 하나의 operation만을 실행시키지만, 실행의 순서는 준비가 되었는지와 우선순위 같은 다른 요소들에 영향을 받는다. 그러므로 직렬화 된 operation queue는 GCD의 직렬화 된 dispatch queue와 동일한 동작을 제공하지 않는다. operation 객체의 실행 순서가 중요하다면 큐에 넣기 전에 종속성을 만들어라. 



#### 직접 Operation을 실행시키기

operation queue가 operation 객체를 실행시키기 가장 간편한 방법이지만, operation을 큐 없이 실행시키는 방법도 있다. operation을 직접 실행시키기로 했다면, 코드에서 미리 주의해야할 사항들이 있다. operation은 반드시 준비 상태여야 하고, 항상 `start` 메서드를 이용해 시작시켜야 한다. 

operation은 `isReady` 메서드가 `YES`를 반환하기 전까지 실행되지 않는다. `isReady` 메서드는  `NSOperation`의 종속성 관리 시스템에 통합되어 operation의 종속성 상태를 제공한다. 종속성이 지워진 이후에만 operation은 실행될 수 있다. 

operation을 직접 실행시킬 때는 실행의 처음 항상 `start` 메서드를 사용해야 한다. `main`이나 다른 메서드 대신 이 메서드를 사용하는 이유는 `start` 메서드는 커스텀 코드를 실행하기 전 몇 가지 안전성 체크를 하기 때문이다. 특히 기본 `start` 메서드는 operation의 종속성을 제대로 처리해야 한다는 KVO notification을 생성한다. 또한 이미 취소된 operation이 실행되는 것을 막고, operation이 실행될 준비가 안 되어 있다면 예외를 던진다.

앱이 동시적 operation 객체를 정의하는 경우, `isConcurrent` 메서드를 호출하는 것을 고려해보아야 한다. `NO`를 반환하는 경우, 로컬 코드는 현재 스레드에서 operation을 동기적으로 실행시킬 지, 별도의 스레드를 먼저 생성할 지 결정할 수 있다. 하지만 이런 검사를 구현하는 것은 전적으로 개발자에게 달려있다. 

아래 코드는 operation을 직접 실행시키기 전 해야하는 검사의 간단한 예제이다. 메서드가 `NO`를 반환할 경우, 타이머를 스케줄링해서 메서드를 후에 다시 호출할 수 있다. 메서드가 `YES`를 반환하기 전까지 계속 타이머를 다시 스케줄링한다. (operation이 취소되었기 때문에 발생할 수도 있다.)

```objective-c
- (BOOL)performOperation:(NSOperation*)anOp
{
   BOOL        ranIt = NO;
 
   if ([anOp isReady] && ![anOp isCancelled])
   {
      if (![anOp isConcurrent])
         [anOp start];
      else
         [NSThread detachNewThreadSelector:@selector(start)
                   toTarget:anOp withObject:nil];
      ranIt = YES;
   }
   else if ([anOp isCancelled])
   {
      // If it was canceled before it was started,
      //  move the operation to the finished state.
      [self willChangeValueForKey:@"isFinished"];
      [self willChangeValueForKey:@"isExecuting"];
      executing = NO;
      finished = YES;
      [self didChangeValueForKey:@"isExecuting"];
      [self didChangeValueForKey:@"isFinished"];
 
      // Set ranIt to YES to prevent the operation from
      // being passed to this method again in the future.
      ranIt = YES;
   }
   return ranIt;
}
```



#### Operation 취소

한 번 operation queue에 추가되면, operation 객체는 queue의 소유가 되고 지워지지 않는다. 큐에서 제거하는 유일한 방법은 operation을 취소하는 것이다. 단일 operation 객체를 `cancel` 메서드를 호출함으로써 취소할 수 있고, `cancelAllOperations` 메서드를 호출함으로써 opearation queue에 있는 모든 operation 객체를 취소할 수 있다.

operation이 정말로 필요하지 않을 때만 operation을 취소해라. 발행된 cancel 명령은 operation 객체를 "cancelled" 상태로 변경시키고, 이는 operation을 실행되지 못하게 한다. 취소된 operation은 "finished"로 여겨지기 때문에 적절한 KVO notification을 받은 종속된 객체는 해당 종속성을 지운다. 그러므로 모든 operation을 취소시키는 일은 앱을 끄거나 유저가 특별히 취소를 요청하는 중요한 몇가지 이벤트에 대해서 일반적으로 발생한다. 



#### Operation이 완료되기 까지 대기

최고의 성능을 위해서 operation을 가능한 비동기적으로 설계해서 operation이 실행되는 동안 앱이 자유롭게 다른 작업들을 할 수 있게 해라. operation을 생성하는 코드가 operation의 결과도 처리해야 한다면, `NSOperation`의  `waitUntilFinished` 메서드를 이용해서 operation이 완료되기 전까지 코드를 실행되지 않게 할 수 있다. 일반적으로, 이 메서드를 호출하지 않는 것이 좋다. 현재 스레드를 차단하는 것이 편리한 솔루션일 수 있으나 그것은 코드에 직렬성을 도입하고 전체 동시성 양을 제한한다. 

> 앱의 main 스레드에서 operation을 기다려서는 안 된다. 반드시 이차적인 스레드나 다른 operation에서 기다려야 한다. 메인 스레드를 막는 것은 앱이 유저 이벤트에 반응하지 못하게 막고, 이것은 앱이 응답성이 없다고 보여지게 만든다. 

단일 작업이 완료되기를 기다리는 것 외에도, `NSOperationQueue`의 `waitUntilAllOperationsAreFinished` 메서드를 이용해서 큐의 모든 작업이 끝나기를 기다릴 수 있다. 전체 큐가 완료되기를 기다릴 때, 다른 스레드가 operation을 큐에 추가하는 것이 가능하므로 대기 시간이 길어질 수 있음을 알아야 한다.



#### 큐의 정지, 재개

operation의 실행에 일시적인 중단을 발행하고 싶다면, 관련된 operation queue를 `setSuspended:` 메서드를 이용해 정지시킬 수 있다. 큐를 정지시키는 것은 이미 실행 중인 operation의 작업을 중단시키는 것은 아니다. 간단하게 새로운 operation이 실행되는 것을 막는다. 다시 작업을 재개하길 기대할 수도 있기 때문에 진행 중인 작업을 정지시키라는 유저 요청에 의해 큐를 중단할 수도 있다. 
