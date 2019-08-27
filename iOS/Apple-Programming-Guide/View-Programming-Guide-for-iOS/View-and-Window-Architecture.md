# View Programming Guide for iOS

> 뷰 프로그래밍 가이드
>
> 원문: [View Programming Guide for iOS](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Introduction/Introduction.html)



## 뷰와 윈도우의 아키텍처

뷰와 윈도우는 앱의 유저 인터페이스를 보여주고, 인터페이스와의 상호작용을 제어한다. UIKit과 다른 시스템 프레임워크는 수정하지 않거나 약간만 수정해서 그대로 사용할 수 있는 많은 뷰들을 제공하고 있다. 또한 표준 뷰에서 제공하는 것과는 다른 컨텐트를 보여줘야 할 때 커스텀 뷰를 정의해서 사용할 수 있다. 

시스템 뷰를 사용하던, 커스텀 뷰를 제작하던 `UIView`와 `UIWindow` 클래스에서 제공하는 기반 구조에 대해 이해해야 한다. 이 클래스들은 뷰의 레이아웃과 보여짐을 관리하기 위한 정교한 기능을 제공한다. 어떻게 이 기능이 작동하는가를 이해하는 것은 앱에 변화가 생겼을 때 뷰가 올바로 작동하도록 하기 때문에 중요하다.



### 뷰 아키텍처 기초

시각적으로 할 수 있는 대부분의 작업은 `UIView` 클래스의 인스턴스인 뷰 객체에 이미 작업이 완료되었다. 뷰 객체는 사각형 영역을 화면에 정의하고 그 영역에서의 드로잉과 터치 이벤트를 제어한다. 뷰는 또한 다른 뷰의 부모 역할을 수행할 수 있고 그 뷰들의 위치와 사이즈를 조정한다. `UIView` 클래스는 뷰들 사이의 관계를 관리하는 대부분의 작업을 수행하지만 이 기본 행위를 필요하다면 커스텀할 수 있다.

코어 애니메이션 레이어와 함께 뷰는 뷰 컨텐트의 렌더링과 애니메이션을 제어한다. UIKit의 모든 뷰는 레이어 객체를 이용해 구성되었다. (보통 `CALayer` 클래스의 인스턴스다.) 레이어는 뷰의 backing store를 관리하고, 뷰와 관련된 애니메이션들을 제어한다. 대부분의 작업들은 `UIView` 인터페이스를 이용해 수행된다. 그러나 뷰의 렌더링이나 애니메이션 동작까지 제어해야 할 경우에는 레이어를 이용해서 작업을 수행할 수 있다. 

뷰와 레이어의 관계를 이해하기 위해서 하나의 예제를 살펴보는 것이 도움이 된다. 아래 그림은 `ViewTransition` 샘플 앱 기저의 코어 애니메이션 레이어와의 관계를 표현한 뷰 아키텍처를 보여준다. 앱의 뷰는 윈도우 (뷰일수도 있다.), 컨테이너 뷰 역할을 하는 일반적인 `UIView` 객체, 이미지 뷰, 컨트롤을 보여주기 위한 툴바, 바 버튼 아이템 (그 자체로 뷰는 아니지만 내부적으로 뷰를 관리한다.)  (실제 `ViewTransitions` 샘플 앱은 트랜지션을 구현하기 위한 추가적인 이미지 뷰를 가지고 있다. 해당 뷰는 보통 숨겨져 있어서, 아래 그림에는 보여지지 않는다.) 모든 뷰는 연관된 레이어 오브젝트를 가지고 있고 뷰의 `layer` 프로퍼티를 이용해서 접근 가능하다. (바 버튼 아이템은 뷰가 아니기 때문에 레이어에 직접적으로 접근할 수 없다.) 레이어 객체의 뒤에는 코어 애니메이션 렌더링 객체가 있고, 하드웨어 버퍼가 최종적으로 화면의 실제 bit들을 관리한다.

![img](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/view-layer-store.jpg)

코어 애니메이션 레이어 객체의 사용은 성능에 중요한 영향을 준다. 뷰 객체의 실제 드로잉 코드는 가능한 적게 호출되고, 코드가 호출되면 그 결과는 코어 애니메이션에 의해 캐싱되고 가능한만큼 후에 재사용된다. 이미 렌더링된 컨텐트를 재사용하는 것은 뷰를 업데이트하는 데 필요한 비싼 드로잉 사이클을 제거한다. 컨텐트를 재사용하는 것은 특히 이미 존재하는 컨텐트가 변할 수 있는 애니메이션에서 중요하다. 재사용은 새로운 컨텐트를 그리는 것보다 훨씬 더 적은 비용이 든다.



#### 뷰 계층 구조와 서브 뷰 관리

자체 컨텐트를 제공하는 것 외에도 뷰는 다른 뷰의 컨테이너 역할도 한다. 한 뷰가 다른 뷰를 포함할 때, 두 뷰 사이에는 부모-자식 관계가 성립된다. 자식 뷰는 관계 속에서 *subview*가 되고, 부모 뷰는 *superview*가 된다. 이 타입의 관계를 생성하는 것은 앱의 보여지는 모습과 앱의 행위 모두에 영향을 준다.

시각적으로, 서브 뷰의 컨텐트는 부모 뷰의 컨텐트 전체나 일부를 가린다. 만약 서브 뷰가 완전히 불투명한 경우 서브 뷰가 차지하는 영역이 부모 뷰의 해당 영역을 완전히 가린다. 서브 뷰가 부분적으로 투명한 경우 두 뷰의 컨텐트는 합쳐져서 보여진다. 각 슈퍼 뷰는 서브 뷰들을 순서가 있는 배열에 저장하고 배열의 순서는 각각의 서브 뷰의 가시성에 영향을 준다. 만약 두 자식 뷰가 영역이 겹친다면 마지막에 더해진 뷰(또는 서브 뷰 배열의 마지막으로 이동 된 뷰)가 다른 뷰의 위에 보여지게 된다.

슈퍼 뷰-서브 뷰 관계는 또한 몇몇 뷰의 행위에 영향을 준다. 부모 뷰의 사이즈를 변경하는 것은 서브 뷰의 사이즈와 위치를 변경시키는 연쇄 작용을 일으킨다. 부모 뷰의 사이즈를 변경할 때, 뷰를 적절히 구성해서 각각의 서브 뷰 의 리사이징을 제어할 수 있다. 이 외에도 서브 뷰의 변형을 일으키는 것에는 슈퍼 뷰를 숨기는 것(hide), 슈퍼 뷰의 투명도(alpha)를 조정 또는 슈퍼 뷰의 좌표계 변형이 있다.

뷰 계층 구조에서 뷰의 배열은 어떻게 앱이 이벤트에 반응하는지도 결정한다.  특정 뷰에서 터치 이벤트가 발생했을 때, 시스템은 터치 정보를 가진 이벤트 객체를 제어하기 위해 바로 그 뷰로 전달한다. 만약 그 뷰가 특정 터치 이벤트를 처리하지 않으면 그 이벤트 객체는 슈퍼 뷰로 보내진다. 슈퍼 뷰가 이벤트를 제어하지 않으면 그 슈퍼 뷰의 슈퍼 뷰로 전달 되는 방식의 responder chain으로 위로 보내진다. 특정 뷰는 또한 이벤트 객체를 뷰 컨트롤러와 같은 중간 응답자 객체로 전달할 수도 있다. 어떤 객체도 이벤트를 처리하지 않으면 최종적으로 application 객체에 보내지게 되고, 보통 이 이벤트는 무시된다.

뷰 계층 구조를 생성하는 것과 관련해 더 많은 정보가 필요하다면, [Creating and Managing a View Hierarchy](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/CreatingViews/CreatingViews.html#//apple_ref/doc/uid/TP40009503-CH5-SW47)를 봐라.



#### 뷰 드로잉 사이클

`UIView` 사이클은 컨텐트를 표시하기 위해서 수요에 의한(on-demand) 드로잉 모델을 사용한다. 화면에 처음 뷰가 나타날 때, 시스템은 뷰에게 컨텐트를 그릴 것을 요구한다. 시스템은 이 컨텐트에 대한 스냅샷을 캡쳐하고 이 스냅샷을 뷰의 시각적인 표현으로 사용한다. 뷰의 컨텐트를 변경하지 않으면, 뷰의 드로잉 코드는 다시 호출되지 않을 것이다. 컨텐트를 변경시키면, 시스템에 뷰가 변경됐음을 알리게 된다. 뷰는 새로운 결과에 대해 뷰를 그리고, 스냅샷을 캡처하는 프로세스를 반복하게 된다.

뷰의 컨텐트가 변경될 때, 변경 사항을 바로 다시 그리지 않는다. 대신 뷰를 `setNeedsDisplay` 나 `setNeedsDisplayInRect:`를 이용해서 뷰를 무효화 시킨다. 이 메서드들은 시스템에게 뷰의 컨텐트가 변경되었고, 다음 기회에 새로 그려야 할 필요가 있음을 알린다.  시스템은 드로잉 작업을 시작하기 전, 현재 런 루프가 종료되기를 기다린다. 이 지연은 여러 뷰를 무효화하고 뷰 계층 구조의 뷰를 추가하거나 제거할 수 있게 하고, 뷰를 가리거나, 리사이징, 재위치 시키는 것을 한 번에 변경할 수 있게 한다. 만든 변화들은 그래서 한 번에 반영된다. 

> 뷰의 geometry를 변경하는 것은 자동적으로 시스템이 뷰의 컨텐트를 다시 그리게 하지 않는다. 뷰의 `contentMode` 프로퍼티가 geometry 변경 사항을 어떻게 처리할 지 정한다. 대부분의 컨텐트 모드는 뷰의 경계 내에서 이미 존재하는 스냅샷을 늘리거나 위치를 재설정하고, 새로운 스냅샷을 생성하지 않는다. 컨텐트 모드가 드로잉 사이클에 어떤 영향을 주는 지 알고 싶다면, [Content Modes](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/WindowsandViews/WindowsandViews.html#//apple_ref/doc/uid/TP40009503-CH2-SW2)를 봐라.

뷰의 컨텐트를 렌더링할 시간에, 실제 드로잉 프로세스는 뷰와 뷰의 설정에 따라 달라진다. 시스템 뷰는 보통 private 드로잉 메서드를 이용해서 컨텐트를 렌더링한다. 동일한 시스템 뷰는 종종 뷰의 실제 모양을 구성할 수 있는 인터페이스를 노출한다.  커스텀 `UIView` 서브 클래스는 보통 `drawRect:` 메서드를 override 하고 이 메서드를 이용해서 뷰의 컨텐트를 그린다. 뷰의 컨텐트를 제공하는 다른 방법도 있는데, 뷰의 내부 레이어의 컨텐트를 직접 설정하는 것이다. 하지만 `drawRect:` 메서드를 override 하는 것이 일반적인 방법이다.



#### 컨텐트 모드

각각의 뷰는 뷰의 geometry 변경에 따라 뷰가 컨텐트를 재사용하는 방법과 컨텐트의 재사용 여부를 제어하는 컨텐트 모드를 가진다. 뷰가 처음 보여질 때, 뷰는 보통 컨텐트를 렌더링하고, 그 결과는 비트맵으로 캡처된다. 이후에 뷰의 geometry에 생긴 변화는 항상 비트맵이 재생성되게 하지는 않는다. 대신, `contentMode` 프로퍼티의 값이 비트맵이 비트맵이 새로운 바운드에 맞게 크기를 재설정해야 하는지, 뷰의 한 모서리나 가장자리에 고정해야 할 지를 정한다.

뷰의 컨텐트 모드는 다음과 같은 것들을 할 때 적용된다.

* 뷰의 `frame` 또는 `bounds` 사각형의 width, height을 변경했을 때
* 뷰의 `transform` 프로퍼티의 크기 설정 요소를 변경했을 때

기본적으로 대부분의 뷰의 `contentMode` 프로퍼티는 `UIViewContentModeScaleToFill`로 설정되어 있다. 이는 뷰의 컨텐츠가 새로운 프레임 사이즈에 맞게 크기를 변경 시킨다. 아래 그림은 가능한 몇 개의 컨텐트 모드들이 적용된 결과를 보여준다. 그림에서 볼 수 있듯이, 모든 컨텐트 모드가 뷰의 바운즈에 맞게 전체를 가득 채우지는 않고, 뷰의 내용을 왜곡시킬수도 있다.

![Content mode comparisons](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/scale_aspect.jpg)

컨텐트 모드는 뷰의 컨텐츠를 재사용하기에 좋은 방법이지만, 컨텐트 모드를 `UIViewContentModeRedraw` 로 설정하면 커스텀 뷰가 크기를 크기를 변경하거나 리사이징 작업을 할 때 다시 그리도록 할 수 있다. 뷰의 컨텐트 모드를 이 값으로 바꾸는 것은 시스템이 geometry가 변경될 때 `drawRect:` 메서드를 부르도록 강제한다. 보통 가능하다면 이 값을 사용하는 것을 피해야 하고, 표준 시스템 뷰들에서 이 값을 사용하지 말아야 한다.

가능한 컨텐트 모드에 대해 더 알고 싶다면, [UIView Class Reference](https://developer.apple.com/documentation/uikit/uiview)를 확인해라.



#### 신축성이 있는 뷰

뷰의 부분을 신축성이 있게 지정하면 뷰의 사이즈가 변경될 때 신축성 있는 부분의 컨텐트가 영향을 받게 할 수 있다. 보통 버튼이나 반복적인 패턴을 정의하고 있는 다른 뷰를 위해 신축성이 있는 영역을 사용한다. 정의된 신축성이 있는 영역 뷰의 하나, 또는 두 개의 축을 늘릴 수 있다. 두 축을 이용해서 뷰를 늘릴 경우 뷰의 모서리는 왜곡을 막기 위해서 반복되는 패턴으로 정의되어 있어야 한다. 아래 그림은 어떻게 왜곡이 뷰에 드러나는 지 보여준다. 뷰의 원본 픽셀들의 색은 커진 뷰의 일치하는 영역에 복제된다. 

![img](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/button_scale.jpg)

신축성이 있는 영역은 `contentStretch` 프로퍼티를 이용해서 지정된다. 이 프로퍼티는 0.0에서 1.0 사이의 값으로 정규화 된 사각형을 가집니다. 뷰를 늘릴 때, 시스템은 이 정규화 된 값을 현재 bounds와 scale 요소를 곱해서 어떤 픽셀을 늘려야 하는지 정한다. 정규화 된 값의 사용은 뷰의 bounds가 변할 때마다 `contentStretch` 프로퍼티를 업데이트 하지 않게 도와준다.

뷰의 컨텐트 모드는 또한 뷰의 신축성 있는 영역이 어떻게 사용되는 지를 결정하기도 한다. 신축성 있는 영역은 컨텐트 모드로 인해 뷰의 컨텐트가 조정될 때만 사용된다. 이것은 늘어날 수 있는 뷰는 `UIViewConotentModeScaleToFill`, `UIViewContentModeScaleAspectFit`, `UIVewCoontentModeScaleAspectFill`를 이용해서만 지원된다는 것을 의미한다. 만약 컨텐트를 모서리나 코너에 고정하는 컨텐트 모드를 지정했다면 뷰는 신축성 있는 영역을 무시한다.


> 뷰의 배경을 지정할 때 `contentStretch` 프로퍼티를 사용하는 것이 신축성 있는 `UIImage` 객체를 생성하는 것보다 좋다. 신축성 있는 뷰는 Core Animatioin 레이어에서 전적으로 처리되기 때문에 더 좋은 성능을 보여준다.



#### 빌트인 애니메이션 지원

모든 뷰의 뒤에 레이어 객체를 가지고 있는 것의 장점 중 하나는 뷰와 관련된 변화들을 쉽게 애니메이션을 적용할 수 있다는 것이다. 애니메이션은 유저와 커뮤니케이션하기에 유용한 방법이며, 이는 앱의 설계 단계에서 고려되어야 한다. 많은 `UIView` 클래스의 프로퍼티는 애니메이션이 가능하다(animatable), 즉 어떤 값을 다른 것으로 애니메이션을 적용할 때 부분적으로 자동(semiautomatic)인 지원이 존재한다. 이 애니메이션 가능한 프로퍼티들의 애니메이션을 수행하기 위해서 해야 할 것은 이것이 전부다.

1. UIKit에게 애니메이션을 수행할 것임을 알린다.
2. 프로퍼티의 값을 변경시킨다.



UIView 객체에서 animation을 적용할 수 있는 프로퍼티는 다음과 같다.

[frame](https://developer.apple.com/documentation/uikit/uiview/1622621-frame) - 뷰의 위치와 크기 변경에 애니메이션을 적용할 때 사용한다.

[bounds](https://developer.apple.com/documentation/uikit/uiview/1622580-bounds) - 뷰의 크기에 대한 변경에 애니메이션을 적용할 때 사용한다.

[center](https://developer.apple.com/documentation/uikit/uiview/1622627-center) - 뷰의 위치에 애니메이션을 적용할 때 사용한다.

[transform](https://developer.apple.com/documentation/uikit/uiview/1622459-transform) - 뷰를 회전시키거나 크기를 조절할 때 사용한다.

[alpha](https://developer.apple.com/documentation/uikit/uiview/1622417-alpha) - 뷰의 투명도를 변화시킬 때 사용한다.

[backgroundColor](https://developer.apple.com/documentation/uikit/uiview/1622591-backgroundcolor) - 뷰의 배경 색상을 변경시킬 때 사용한다.

[contentStretch](https://developer.apple.com/documentation/uikit/uiview/1622511-contentstretch) - 뷰의 컨텐츠를 어떻게 늘리는 지를 변경시킬 때 사용한다.

애니메이션이 아주 중요한 곳 중 하나는 하나의 뷰 세트에서 다른 뷰 세트로 트랜지션이 이루어질 때다. 일반적으로, 뷰 컨트롤러가 유저 인터페이스와 관련된 애니메이션의 주요 변화들을 관리한다. 예를 들어, 상위 레벨에서 하위 레벨 정보로 이동하는 인터페이스의 경우, 보통 navigation controller를 사용해서 연속하는 레벨의 데이터를 보여주는 뷰의 트랜지션을 관리한다. 그러나, 뷰 컨트롤러를 이용하지 않고 두 뷰 세트 사이의 트랜지션을 애니메이션을 이용해서 만들 수 있다. 뷰 컨트롤러의 표준 애니메이션이 원하는 애니메이션이 아닐 때, 이를 이용할 수 있다.

UIKit 클래스를 이용해서 생성한 애니메이션 외에도 Core Animation layer를 이용해서 애니메이션을 생성할 수도 있다. 레이어 수준으로 내려가면 애니메이션의 타이밍과 프로퍼티를 더 잘 제어할 수 있다.

뷰 기반에 대한 디테일한 내용은 이 문서의 Animations를 참고하고, Core Animation에 대한 더 많은 정보를 원하면 [Core Animation Programming Guide](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/CoreAnimation_guide/Introduction/Introduction.html#//apple_ref/doc/uid/TP40004514)와 [Core Animation Cookbook](https://developer.apple.com/library/archive/documentation/GraphicsImaging/Conceptual/CoreAnimation_Cookbook/Introduction/Introduction.html#//apple_ref/doc/uid/TP40005406)을 참고해라.



### 뷰 지오메트리와 좌표 시스템

UIKit의 기본 좌표 시스템은 왼쪽 상단 코너를 origin으로 하고, origin 점에서부터 아래와 오른쪽으로 확장하는 축을 가진다. 좌표 값은 floating-point 수로 표현되고, 이를 이용해 정확한 레이아웃과 컨텐트를 화면 해상도와 상관 없이

위치시킬 수 있게 된다. 아래 그림은 화면과 관련된 좌표 시스템을 보여준다. 화면의 좌표 시스템 외에도 윈도우와 뷰는 그들 자신의 원점을 기준으로 하는 고유의 로컬 좌표 시스템을 가진다.

![View coordinate system](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/native_coordinate_system.jpg)

모든 뷰와 윈도우가 스스로의 로컬 좌표 시스템을 가지고 있기 때문에, 어떤 좌표 시스템이 영향을 받고 있는지를 항상 알아야 할 필요가 있다. 뷰를 그리거나 지오메트리를 변경시킬 때, 좌표 시스템과 함께 작업하게 된다. 뷰를 그릴 때, 뷰의 좌표 시스템에 맞는 좌표를 명시해야 한다. 지오메트리가 변경될 때는, 슈퍼뷰의 좌표 시스템과 관련된 좌표를 명시해야 한다. `UIWindow`와 `UIView` 클래스는 어떤 좌표 시스템에서 다른 좌표 시스템으로 변환하는 것을 도와주는 메서드를 포함하고 있다.

> 일부 iOS 기술은 기본 좌표 시스템을 UIKit에서 사용하는 것과는 다르게 정의한다. 예를 들어 Core Graphics나 OpenGL ES는 뷰나 윈도우의 왼쪽 아래를 origin으로 사용하고 y축은 화면의 위 방향으로 존재한다. 컨텐트를 그리거나 생성할 때, 좌표 시스템의 값을 조정할 때 이런 점들을 염두에 두어야 한다.



#### Frame, Bounds, Center 프로퍼티의 관계

뷰 객체는 크기와 위치를 `frame`, `bounds`, `center` 프로퍼티를 이용해서 추적한다. 

* [frame](https://developer.apple.com/documentation/uikit/uiview/1622621-frame) 프로퍼티는 *frame 사각형*을 가진다. 이는 슈퍼 뷰의 좌표 시스템에 맞는 뷰의 크기와 위치를 표현한다.
* [bounds](https://developer.apple.com/documentation/uikit/uiview/1622580-bounds) 프로퍼티는 *bounds 사각형*을 가진다. 이는 뷰의 사이즈(그리고 컨텐트의 origin)을 자기 자신의 로컬 좌표 시스템에서 표현한다.
* [center](https://developer.apple.com/documentation/uikit/uiview/1622627-center) 프로퍼티는 슈퍼 뷰의 좌표 시스템에서 뷰의 알려진 center point를 가진다.

`center`와 `frame` 프로퍼티는 주로 현재 뷰의 지오메트리를 조작할 때 쓰인다. 예를 들면, 이 프로퍼티들은 뷰 계층을 만들 때나 뷰의 위치나 크기를 런타임에 변경시킬 때 사용된다. 크기는 변경시키지 않고 오직 뷰의 위치만 변경시킬 때는 `center` 프로퍼티를 사용하는 것이 선호된다. `center` 프로퍼티의 값은 transform이 추가되어 크기를 조정하거나 회전될 때를 포함해서 항상 유효하다. 이것은 `frame` 프로퍼티에는 적용되지 않는 사항이다. 뷰의 transform이 초기의 trnasform에서 달라지더라면 `frame` 프로퍼티는 유효하지 않다.

`bounds` 프로퍼티는 주로 드로잉에 많이 쓰인다. bounds 사각형은 뷰의 로컬 좌표 시스템에서 표현된다. 이 사각형의 기본 origin은 (0, 0)이고 크기는 frame 사각형의 크기와 같다. 이 사각형 내부에 그리는 것들은 어떤 것이든 뷰의 보여지는 먼텐트가 된다. bounds 사각형의 origin을 변경시키면, 새로운 사각형에 보이는 부분이 뷰의 보여지는 컨텐트가 된다.

아래 그림은 한 이미지 뷰에서의 frame과 bounds 사각형의 관계를 보여준다. 그림에서 이미지 뷰의 왼쪽 상단 코너는 슈퍼 뷰의 좌표 시스템에서 (40, 40)에 위치하고 크기는 240 * 380이다. bounds 사각형에서 origin 점은 (0, 0) 그리고 사각형의 크기는 동일하게 240 * 380이다.



![Relationship between a view's frame and bounds](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/frame_bounds_rects.jpg)

`frame`, `bounds`, `center` 프로퍼티를 각각 독립적으로 변경시킬 수 있지만, 한 프로퍼티를 변경시키는 것은 다른 프로퍼티들에 다음과 같은 상황에서 영향을 줄 수 있다.

* `frame` 프로퍼티를 설정했을 때, `bounds` 프로퍼티의 크기 값이 새로운 frame 사각형에 맞게 변경된다. `center` 프로퍼티의 값도 동일하게 frame 사각형의 새로운 center 점으로 변경된다.
* `center` 프로퍼티를 설정했을 때, `frame`의 origin 값도 알맞게 변경된다.
* `bounds` 프로퍼티를 설정했을 때, `frame` 프로퍼티의 크기 값이 bounds 사각형의 새로운 크기에 맞게 변경된다.

기본적으로 뷰의 frame은 슈퍼 뷰의 프레임에 잘리지(clip) 않는다. 서브 뷰가 슈퍼 뷰의 프레임 밖에 있더라도 전체가 렌더링된다. 그렇지만 이 행위를 슈퍼 뷰의 `clipsToBounds` 프로퍼티를 `YES`로 설정함으로써 변경시킬 수 있다. 서브 뷰가 시각적으로 잘리던, 그렇지 않던 터치 이벤트는 항상 타겟 뷰의 슈퍼 뷰의 bounds 사각형에 기반한다. 다시 말해 슈퍼 뷰의 bounds 사각형을 벗어난 뷰의 일부에서 발생하는 터치 이벤트는 해당 뷰로 전달되지 않는다.
