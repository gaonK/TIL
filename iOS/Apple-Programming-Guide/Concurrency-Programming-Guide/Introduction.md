# Concurrency Programming Guide

> 동시성 프로그래밍 가이드
>
> 원문: [Concurrency Programming Guide](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/Introduction/Introduction.html)

## Introduction

동시성(Concurrency)은 여러 가지 일들이 동시에 일어남을 의미한다. 멀티 코어 CPU의 확산과 각 프로세서의 코어 수가 증가할 것이라는 인식으로 개발자들은 이를 활용해야 할 필요가 생겼다. OS X나 iOS 같은 운영 체제는 동시에 여러 프로그램을 병렬적으로 실행할 수 있지만 대부분의 프로그램은 백그라운드에서 실행되고 짧은 프로세서 시간을 요구하는 작업들을 수행한다. 유저의 관심을 끌고, 컴퓨터를 바쁘게 작동하게 하는 것은 포그라운드 앱이다. 만약 앱이 수행해야 할 일이 많지만 사용 가능한 코어 중 일부만 사용한다면 남은 프로세싱 자원들은 낭비되는 것이다.

과거에는 동시성 프로그래밍을 도입하려면 추가적인 스레드를 생성해야 했다. 불행하게도 스레드와 관련된 코드를 작성하는 일은 쉽지 않다. 스레드는 반드시 수동으로 다루어져야 하는 로우 레벨 툴이다. 최적의 스레드 수를 할당하는 것은 현재 시스템 로드와 하드웨어에 따라 달라지기 때문에 적절한 스레딩 솔루션을 구현하는 것은 불가능하지는 않지만 아주 어렵다. 게다가 스레드 동기화 메커니즘은 성능 향상을 보장하지 않으며 소프트웨어 설계에 복잡도와 리스크를 추가한다.

OS X와 iOS는 기존의 스레드 기반의 시스템과 앱들에서 사용한 방법들보다 더 비동기적인 방법을 채택해서 동시성 작업들을 수행한다. 스레드를 직접적으로 생성하는 작업 대신 앱은 특정 작업들을 정의하고 시스템이 이를 수행하도록 한다. 시스템이 스레드를 관리하게 하기 때문에 앱은 스레드를 직접 이용할 때는 불가능한 수준의 확장성을 얻는다. 또한 앱 개발자들은 더 단순하고 더 효과적인 프로그래밍 모델을 얻을 수 있다.

이 문서는 동시성을 당신의 앱에 구현할 때 사용해야할 테크닉과 기술을 다룬다. 이 문서에 적혀있는 기술은 OS X와 iOS에 모두 적용된다.



### 구성

* [**동시성과 앱 설계**](Concurrency-and-Application-Design.md): 비동기 앱 설계의 기초와 커스텀 작업들을 비동기적으로 수행하는 기술
* [**Operation Queues**](Operation-Queues.md): Objective-C 객체들을 이용하여 작업을 캡슐화하고 수행하는 방법
* **Dispatch Queues**: C-기반의 앱에서 작업을 동시적으로 수행하는 방법
* **Dispatch Sources**: 시스템 이벤트를 비동기적으로 다루는 방법
* **Migrating Away from Threads**: 스레드 기반의 코드를 새로운 기술을 이용하게 바꾸는 팁과 테크닉 



### 용어

동시성에 대해 얘기하기 전에 혼란을 막기 위해 몇 가지 관련 용어들을 정의할 필요가 있다. UNIX 시스템이나 이전 OS X 기술에 익숙한 개발자들은 아마 "작업(task)", "프로세스(process)", "스레드(thread)"가 다소 다르게 쓰임을 알게 될 것이다. 이 문서에서는 이 용어들을 다음과 같이 쓴다.

* 스레드: 코드의 독립된 실행 경로. OS X 스레드의 기본 구현은 POSIX 스레드를 기반으로 한다.
* 프로세스: 여러 개의 스레드를 포함하는 실행 중인 실행 파일
* 작업: 실행되어야 하는 추상적인 개념의 작업



### 그 외

이 문서는 당신의 앱에 동시성을 구현하기 위해 선호되는 기술에 중점을 두고 있으며 스레드 사용에 대해서는 다루지 않고 있다. 스레드 또는 기타 스레드 관련 기술 사용에 대한 정보가 필요하다면 [스레딩 프로그래밍 가이드(Threading Programming Guide)](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/Multithreading/Introduction/Introduction.html#//apple_ref/doc/uid/10000057i)를 참조해라.
