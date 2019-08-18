# View Programming Guide for iOS

> 뷰 프로그래밍 가이드
>
> 원문 : [View Programming Guide for iOS](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Introduction/Introduction.html)

## 윈도우와 뷰

iOS에서, 앱의 내용을 화면에 보여주기 위해서 윈도우와 뷰를 사용한다. 윈도우는 눈에 보이는 내용은 없지만 앱의 뷰들을 제공하는 기본 컨테이너를 제공한다. 뷰는 원하는 내용으로 채울 윈도우의 일부를 정의한다. 예를 들어 이미지, 텍스트, 모양, 또는 그 조합으로 이루어진 것들을 보여주기 위해 뷰를 사용할 수 있다. 또한 다른 뷰들을 구성하고 관리하기 위해서 뷰를 사용할 수도 있다.



### 훑어 보기

모든 앱은 내용을 보여주기 위해서 하나 이상의 윈도우와 뷰를 갖는다. UIKit과 그 외 시스템 프레임워크들은 내용을 보여주기 위해서 사용할 수 있는 미리 정의된 뷰를 제공한다. 간단한 버튼과 텍스트 레이블에서 시작해서 테이블 뷰, 피커(picker) 뷰, 스크롤 뷰 같은 복잡한 뷰까지 다양한 범위의 뷰를 제공한다. 미리 정의되어 있는 뷰가 아닌 다른 뷰가 필요하다면 커스텀 뷰를 만들고 드로잉과 이벤트 핸들링을 직접할 수 있다.



#### 뷰는 앱의 눈에 보이는 내용을 관리한다.

뷰는 `UIView` 클래스의 인스턴스 (또는 서브 클래스)로 앱의 윈도우 안에서 직사각형 영역을 관리한다. 뷰는 내용을 그리고(draw), 멀티 터치 이벤트를 핸들링하고 서브뷰의 레이아웃을 관리하는 책임이 있다. 드로잉은 Core Graphics, OpenGL ES, 또는 UIKit을 사용하여 모양, 이미지, 그리고 텍스트를 뷰의 사각형 영역에 그리는 것을 포함한다. 뷰는 gesture recognizer를 이용하거나 터치 이벤트를 직접 핸들링해서 뷰의 사각형 영역 안의 터치 이벤트에 반응한다. 뷰 계층에서, 부모 뷰는 그들의 자식 뷰의 위치와 사이즈를 조절하는 책임이 있고, 이를 동적으로 수행할 수 있다. 자식 뷰를 동적으로 변경하는 기능은 뷰가 인터페이스 회전이나 애니메이션과 같은 변경되는 조건들에 대응할 수 있게 해준다.

뷰를 건물의 벽돌처럼 생각해도 좋다. 개발자는 유저 인터페이스를 건설하기 위해서 뷰를 사용한다. 하나의 뷰를 이용해서 모든 내용을 보여주려고 하기 보다, 종종 여러 개의 뷰를 이용해서 뷰 계층을 만든다. 계층에 있는 각각의 뷰는 유저 인터페이스이 특정 부분을 보여주고, 일반적으로 특정 타입의 내용에 최적화된다. 예를 들어  UIKit은 특별히 이미지, 텍스트, 그 외 타입의 내용을 보여주는 뷰를 가지고 있다.



#### 윈도우는 뷰의 표현(display)를 조정한다.

윈도우는 `UIWindow` 클래스의 인스턴스로 앱 유저 인터페이스의 전체 보여짐을 관리한다. 윈도우는 뷰와 함께 작동해서 (그들을 소유하고 있는 뷰 컨트롤러와 함께) 보여지는 뷰 계층과의 상호 작용과 변경 사항을 관리한다. 대부분의 경우 앱의 윈도우는 변하지 않는다. 윈도우가 생성된 후 윈도우는 그대로 유지되고 보여지는 뷰만 변경된다. 모든 앱은 디바이스의 메인 화면에서 앱의 유저 인터페이스를 보여주는 최소 한 개 이상의 윈도우를 가진다. 외부 디스플레이가 디바이스에 연결되면, 앱은 두 번째 윈도우를 만들어서 화면에 내용을 보여준다.



#### 애니메이션은 유저에게 인터페이스 변경에 대한 눈에 보이는 피드백을 제공한다.

애니메이션은 유저에게 뷰 계층의 변경 사항에 대한 눈에 보이는 피드백을 제공한다. 시스템은 모달 뷰를 present하는 것과 다른 뷰 그룹 간에 transition하는 경우에 대해 표준 애니메이션을 정의한다. 그러나 많은 뷰의 요소들이 직접적으로 움직여질 수 있다. 예를 들어 애니메이션을 통해 뷰의 투명도나 화면에서의 위치, 크기, 배경 색상, 그 외 다른 요소들을 조절할 수 있다. 그리고 뷰의 기본 CoreAnimation 레이어 객로 작업하면, 더 많은 애니메이션을 수행할 수 있다. 



#### 인터페이스 빌더의 역할

인터페이스 빌더는 앱의 윈도우와 뷰를 그래픽적으로 구성하고 설정하려고 사용하는 앱이다. 인터페이스 빌더를 사용해서 뷰를 조합하고 그것을 nib 파일에 둔다.  nib 파일은 뷰와 다른 객체의 동결 건조 버전을 저장하는 리소스 파일이다. nib 파일을 런타임에 로드하면, 내부에 있는 객체들은 실제 객체로 재구성되어 코드에서 프로그래밍을 통해 조작할 수 있다.

인터페이스 빌더는 앱의 유저 인터페이스를 생성할 때 해야하는 작업들을 간단하게 만든다. 인터페이스 빌더 및 nib 파일에 대한 지원은 iOS 전체에 통합되어 있어서 nib 파일을 앱의 설계에 통합시키는 데는 많은 노력이 필요하지 않다. 

인터페이스 빌더를 쓰는 방법에 대한 더 많은 정보를 알고 싶다면, [Interface Builder User Guide](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/IB_UserGuide/Introduction/Introduction.html#//apple_ref/doc/uid/TP40005344)를 봐라. 뷰 컨트롤러가 어떻게 뷰를 가지고 있는 nib 파일을 관리하는 지 보고 싶다면 [View Controller Programming Guide for iOS](https://developer.apple.com/library/archive/featuredarticles/ViewControllerPGforiPhoneOS/index.html#//apple_ref/doc/uid/TP40007457)의 Creating Custom Content View Controllers를 확인해라.



### 그 외

뷰는 아주 정교하고 유연한 객체이기 때문에, 이 문서에서 뷰의 모든 행동(behavior)를 다루는 것은 불가능한 일일 것이다. 그러나, 다른 문서들이 뷰를 다루는 다른 방법들과 유저 인터페이스에 관해서 배우는 데 도움을 주는 것이 가능하다.

* 뷰 컨트롤러는 앱의 뷰를 관리하는 데 중요한 요소이다. 뷰 컨트롤러는 단일 뷰 계층 구조에서 모든 뷰를 제어하고, 화면에서 뷰를 표현한다. 뷰 컨트롤러와 뷰 컨트롤러의 역할에 대해 더 알고 싶다면 [View Controller Programming Guide for iOS](https://developer.apple.com/library/archive/featuredarticles/ViewControllerPGforiPhoneOS/index.html#//apple_ref/doc/uid/TP40007457)를 확인해라.
* 뷰는 앱에서 제스쳐와 터치 이벤트의 주요한 수신자이다. gesture recognizer와 터치 이벤트 핸들링을 직접 사용하는 것에 대한 더 많은 정보는 ~~Event Handling Guide for iOS~~를 확인해라.
* 커스텀 뷰는 반드시 사용 가능한 드로잉 기법을 이용해서 내용을 렌더링한다. 뷰 내부를 그리는 기술을 알고 싶다면 [Drawing and Printing Guide for iOS](https://developer.apple.com/library/archive/documentation/2DDrawing/Conceptual/DrawingPrintingiOS/Introduction/Introduction.html#//apple_ref/doc/uid/TP40010156)를 봐라.
* 표준 뷰 애니메이션이 충분하지 않은 경우에, Core Animation을 이용할 수 있다. Core Animation을 이용해서 애니메이션을 구현하는 것에 대해 더 궁금하다면 [Core Animation Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/CoreAnimation_guide/Introduction/Introduction.html#//apple_ref/doc/uid/TP40004514)를 참고해라.
