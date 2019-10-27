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



#### 좌표 시스템 transformation

좌표 시스템 transformaation은 뷰(또는 컨텐츠)를 빠르고 쉽게 바꿀 수 있는 방법을 제공한다. *affine* transformation은 한 좌표 시스템에 있는 점이 다른 좌표 시스템의 점으로 어떻게 매핑되는 지를 나타내는 수학적인 행렬이다. affine transformation을 슈퍼 뷰에 상대적으로 그 뷰의 크기, 위치, 방향을 변경하고 싶을 때 적용할 수 있다. 또한 affine transformation을 드로잉 코드에서 각각의 렌더링된 컨텐트에 대해 동일한 타입의 작업을 수행할 수 있다. 그러므로 어떻게 affine transformation을 적용시킬 지는 문맥에 달려있다.

* 전체 뷰를 변경시키고 싶을 때, 뷰의 [transform]( https://developer.apple.com/documentation/uikit/uiview/1622459-transform) 프로퍼티에서 affine transformation을 수정해라
* 뷰의 [drawRect:](https://developer.apple.com/documentation/uikit/uiview/1622529-draw) 메서드의 일부 컨텐트를 변경시키고 싶을 때, 활성화 된 그래픽스 문맥과 관련된 affine 변환을 수정해라

보통 애니메이션을 구현하고 싶을 때, 뷰의 `transform` 프로퍼티를 수정한다. 예를 들면, 이 프로퍼티를 이용해서 center 점을 중심으로 뷰를 회전시키는 애니메이션을 만들 수 있다. 이 프로퍼티를 이용해서 뷰의 위치나 크기를 슈퍼 뷰의 좌표 시스템 안에서 변경시키는 것과 같은 뷰의 영구적인 변화를 위해서 사용하지는 않는다. 그런 타입의 변경을 위해서는 대신 뷰의 frame 사각형을 변경해야 한다.

> 뷰의 `transform` 프로퍼티를 변경시킬 때는, 모든 transformation이 뷰의 center 점과 관련되어 수행된다.

뷰의 `drawRect:` 메서드에서, affine transformation을 그리고 싶은 대로 위치와 방향을 변경하고 싶어서 사용한다. 객체의 위치를 뷰의 어떤 점에 고정시키기 보다, 각각의 객체를 어떤 고정된 점에 대한 상대적인 객체로 만든다. 이는 보통 (0, 0)이다. 그리고 위치에 대한 transformation을 드로잉에 관해 우선하도록 만든다. 그러므로 뷰의 객체의 위치를 변경시킬 때, 해줘야 하는 일은 transform을 수정하는 것 뿐이다. 이것은 새로운 해당 객체의 새로운 위치를 생성하는 것보다 훨씬 빠르고 비용이 덜 드는 방법이다. [CGContextGetCTM](https://developer.apple.com/documentation/coregraphics/1454691-cgcontextgetctm) 함수를 이용해서 그래픽스 문맥과 관련된 affine transformation을 검색할 수 있고 관련 코어 그래픽 함수를 이용해서 드로잉하는 중에 이 변환을 설정하거나 수정하는 것이 가능하다.

current transformation matrix (CTM)은 언제나 쓰이는 affine transformation이다. 전체 뷰에 대한 지오메트리를 변경시킬 때, CTM은 뷰의 `transform` 프로퍼티에 저장된 affine transformation이다. `drawRect:` 메서드에서 CTM은 활성화 된 그래픽스 문맥과 관련있는 affine transformation이다.

각각의 서브 뷰의 좌표 시스템은 부모의 좌표 시스템 위에 만들어진다. 그렇기 때문에 뷰의 `transform` 프로퍼티를 변경할 때, 그것이 뷰와 그 뷰의 모든 서브 뷰에 영향을 미친다. 그러나 이러한 변경은 화면에 있는 뷰의 마지막 렌더링에만 영향을 미친다. 각각의 뷰는 각각의 bounds에 맞게 컨텐트를 그리고, 서브 뷰의 레이아웃을 결정하기 때문에, 슈퍼 뷰의 드로잉과 렌더링 단계에서의 transform은 무시할 수 있다.

아래 그림은 서로 다른 두 개의 회전 요소를 어떻게 렌더링할 때 시각적으로 결합할 수 있는 지를 보여준다. `drawRect:` 메서드에서 45도 회전 요소를 적용해서 모양을 45도 회전되 보이게 만든다. 또 다른 45도 회전 요소를 뷰에 적용하면 모양이 90도 회전 되어 보이게 만든다. 이 모양은 여전히 단순히 뷰에 45도 기울어진 상태이다. 그러나 뷰의 회전은 더 많이 회전된 것처럼 보이게 한다.

![img](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/xform_rotations.jpg)



> 중요: 만약 뷰의 `transform` 프로퍼티가 항등 변환이 아닌 경우, 해당 뷰의 프레임 프로퍼티는 정의되지 않고 무시된다. 뷰의 transformation을 적용할 때, 반드시 뷰의 `bounds`와 `center` 프로퍼티를 이용해서 뷰의 크기와 위치를 가져와야 한다. 그렇게 하면 어떤 서브 뷰의 frame 사각형이든 뷰의 bounds에 연관되어 있기 때문에 유효하다.

뷰의 transform 프로퍼티를 런타임에 변경시키는 것과 관련된 더 많은 정보는 이 문서의 [Translating, Scaling, and Rotating Views](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/CreatingViews/CreatingViews.html#//apple_ref/doc/uid/TP40009503-CH5-SW4)를 참고해라. 드로잉하는 동안 컨텐트를 위치시키는 것에 어떻게 transform을 사용하는 지에 대해 더 궁금하면 [Drawing and Printing Guide for iOS](https://developer.apple.com/library/archive/documentation/2DDrawing/Conceptual/DrawingPrintingiOS/Introduction/Introduction.html#//apple_ref/doc/uid/TP40010156)를 참고해라.



#### Points vs. Pixels

iOS에서 모든 좌표값과 거리는 point로 불리는 실수값으로 표현된다. 포인트의 측정 가능한 크기는 기기에 따라 다르며 크게 관련이 없다. point를 이해하기 위해  point가 드로잉을 위한 고정된 프레임 참조를 제공한다는 점을 아는 것이 중요하다. 

아래 테이블은 다양한 iOS 기기의 portrait 방향일 때의 화면 크기를 보여준다. (point 기준) 가로 x 세로로 표현되어 있다. 이러한 화면 크기에 맞게 인터페이스를 설계하면 해당 유형의 기기에 뷰가 적절하게 표시된다.

| Device                                                   | Screen dimensions (in points) |
| :------------------------------------------------------- | :---------------------------- |
| iPhone and iPod touch devices with 4-inch Retina display | 320 x 568                     |
| Other iPhone and iPod touch devices                      | 320 x 480                     |
| iPad                                                     | 768 x 1024                    |

각 유형의 기기에서 사용되는 point 기반 측정 시스템은 유저의 좌표 공간을 정의한다. 이것은 거의 모든 코드에서 사용하는 표준 좌표 시스템이다.  point와 유저 좌표 공간은 뷰의 지오메트리를 조작하거나 Core Graphics 함수를 호출해서 뷰의 컨텐츠를 그릴 때 사용된다. 유저 좌표 공간에 있는 좌표가 가끔은 기기의 화면에서 pixel에 직접적으로 매핑되지만, 이런 상황일 거라고 가정하면 안 된다. 대신, 아래의 사항을 반드시 고려해야 한다.

**한 point가 화면에서 반드시 한 pixel에 대응되는 것은 아니다.**

기기 레벨에서, 뷰의 모든 좌표는 어떤 point의 pixel로 변환되어야 한다. 그러나 유저 좌표 시스템의 point를 기기 좌표 시스템의 pixel로 매핑하는 것은 보통 시스템에 의해 제어된다. UIKit과 Core Graphics는 주로 모든 좌표 값이 point를 이용해서 표현되는 벡터 기반의 드로잉 모델을 사용한다. 만약 Core Graphics를 이용해서 curve를 그리고 싶으면, curve를 화면의 해상도에 상관 없이 같은 값을 이용해서 그리면 된다. 

OpenGL ES와 같은 이미지나 pixel 기반 기술로 작업해야 하는 경우, iOS는 pixel을 관리할 수 있는 기능을 제공한다. 앱 번들에 리소스로 저장된 정적 이미지 파일 같은 경우, iOS는 다른 pixel 밀도로 이미지를 정하고 현재 화면의 해상도와 가장 맞는 이미지를 로드하기 위한 규칙을 정의한다. 또한 뷰는 pixel 기반의 드로잉 코드가 수동으로 조정해서 고해상도의 화면을 수용할 수 있도록 현재 배율 요소에 대한 정보를 제공한다. 각각 다른 화면 해상도에서 pixel 기반 컨텐트를 다루는 기술에 대해 더 궁금하다면 [Drawing and Printing Guide for iOS](https://developer.apple.com/library/archive/documentation/2DDrawing/Conceptual/DrawingPrintingiOS/Introduction/Introduction.html#//apple_ref/doc/uid/TP40010156) 에 있는 [Supporting High-Resolution Screens In Views](https://developer.apple.com/library/archive/documentation/2DDrawing/Conceptual/DrawingPrintingiOS/SupportingHiResScreensInViews/SupportingHiResScreensInViews.html#//apple_ref/doc/uid/TP40010156-CH15)를 확인해라. 



### 뷰의 런타임 인터렉션 모델

유저가 유저 인터페이스와 상호작용하거나, 프로그래밍적으로 뭔가를 변경시킬 때마다 복잡한 이벤트 시퀀스가 인터렉션을 제어하기 위해서 UIKit의 내부에서 발생한다. 이 시퀀스 동안 특정 지점ㅁ에서 UIKit은 뷰 클래스를 호출하고 앱 대신 응답할 수 있는 기회를 제공한다. 이러한 콜 아웃 지점을 이해하는 것은 뷰가 어디에서 시스템에 적합한 지를 이해하는 것에 중요하다. 아래 그림은 유저가 화면을 터치하면서 시작되고, 응답으로 그래픽 시스템을 이용해서 화면의 컨텐트를 업데이트 시키는 기본적인 이벤트 시퀀스를 보여준다. 프로그래밍적으로 발생되는 다른 액션들에 대해서도 동일한 이벤트 시퀀스가 발생할 수 있다.

![UIKit interactions with your view objects](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Art/drawing_model.jpg)

아래 단계들은 위의 그림을 좀 더 세분화시켜 각 단계에서 어떤 일이 일어나는 지, 어떻게 앱이 응답하기를 바라는 지를 설명한다.

1. 유저가 화면을 터치한다.

2. 하드웨어가 터치 이벤트를 UIKit 프레임워크에 보고한다.

3. UIKit 프레임워크는 UIEvent 객체로 터치를 패키징하고 적절한 뷰로 보낸다. (어떻게 보내는지 자세한 설명은 Event Handling Guide for iOS를 참고해라)

4. 뷰의 이벤트 핸들링 코드가 이벤트에 응답한다. 예를 들어 코드는 다음과 같을 수 있다.

   * 뷰의 프로퍼티(frame, bounds, alpha, ...)를 바꾸거나, 서브 뷰를 바꾼다.
   * `setNeedsLayout` 메서드를 호출해서 뷰가 레이아웃 업데이트가 필요함을 표시한다.
   * `setNeedsDisplay`나 `setNeedsDisplayInRect:`를 호출해서 뷰(또는 서브 뷰)가 다시 그려져야 함을 표시한다.
   * 컨트롤러에게 몇몇 데이터가 변경되었음을 알린다.

   물론 뷰가 어떤 행동을 해야 할 지, 어떤 메서드를 호출해야 할 지는 개발자에게 달려있다.

5. 어떤 이유에서 뷰의 지오메트리가 변경되던, UIKit은 아래의 규칙에 따라 서브 뷰를 업데이트한다.

   1. 뷰의 autoresizing 규칙을 설정해뒀다면, UIKit은 각각의 뷰에 이 규칙을 적용시킨다. autoresizing 규칙이 어떻게 작동하는 지에 대한 정보가 궁금하다면 [Handling Layout Changes Automatically Using Autoresizing Rules](https://developer.apple.com/library/archive/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/CreatingViews/CreatingViews.html#//apple_ref/doc/uid/TP40009503-CH5-SW5)를 참고해라.

   2. 뷰가 `layoutSubviews` 메서드를 실행시킨다면, UIKit이 호출한다. 

      커스텀 뷰에서 이 메서드를 override할 수 있고, 서브 뷰의 위치와 크기를 조절하기 위해 이 메서드를 사용할 수 있다. 예를 들어 큰 스크롤 가능한 영역을 제공하는 뷰는 커다란 뷰를 생성하기 보다, 타일로서 여러 서브 뷰가 필요하고 그것은 메모리에 적합하지 않다. 이 메서드의 구현에서, 뷰는 현재 보이지 않는 서브 뷰들을 숨기거나 재위치 시켜서 새로 보여지는 컨텐트에 그린다. 이 프로세스의 과정에서, 뷰의 레이아웃 코드는 다시 그려질 필요가 있는 뷰들을 무효화(invalidate)시킬 수 있다.

6. 어떤 뷰의 어떤 부분이 다시 그려질 필요가 있다고 표시되면, UIKit이 view에 자기 자신을 다시 그릴 것을 요청한다.

   명시적으로 `drawRect:` 메서드를 정의한 커스텀 뷰의 경우, UIKit이 해당 메서드를 호출한다. 이 메서드의 구현은 뷰의 지정된 영역을 최대한 빨리 다시 그려야 한다. 이 지점에서 추가적인 레이아웃 변경을 만들지 말고 앱의 데이터 모델에 다른 변화를 주지 마라. 이 메서드이 목적은 뷰의 시각적인 컨텐트를 업데이트하는 것이다.

   표준 시스템 뷰는 보통 `drawRect:` 메서드를 구현하지 않고 드로잉을 관리한다.

7. 업데이트 된 뷰는 앱의 다른 보여지는 컨텐트들과 합쳐져서 그래픽 하드웨어로 보내진다.

8. 그래픽 하드웨어는 렌더링된 컨텐트를 화면으로 전송한다.



> 위의 업데이트 모델은 주로 표준 시스템 뷰와 드로잉 기술을 사용하는 앱에 적용된다. OpenGL ES를 드로잉에 사용하는 앱은 일반적으로 단일 전체 화면 뷰를 구성하고 관련된 OpenGL ES 그래픽 컨텍스트에 직접 그린다. 이 경우, 뷰는 여전히 터치 이벤트를 제어하지만, 전체 화면이므로 서브 뷰의 레이아웃을 정할 필요는 없다. 이에 대해 더 많은 정보가 필요하면 [OpenGL ES Programming Guide](https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/Introduction/Introduction.html#//apple_ref/doc/uid/TP40008793)를 참고해라.

위에서 언급 된 단계에서 커스텀 뷰의 주요 통합 지점은 다음과 같다.

* 이벤트 핸들링 메서드
  * [`touchesBegan:withEvent:`](https://developer.apple.com/documentation/uikit/uiresponder/1621142-touchesbegan)
  * [`touchesMoved:withEvent:`](https://developer.apple.com/documentation/uikit/uiresponder/1621107-touchesmoved)
  * [`touchesEnded:withEvent:`](https://developer.apple.com/documentation/uikit/uiresponder/1621084-touchesended)
  * [`touchesCancelled:withEvent:`](https://developer.apple.com/documentation/uikit/uiresponder/1621116-touchescancelled)
* `layoutSubviews` 메서드
* `drawRect` 메서드

이것들은 가장 보편적으로 view에서 override 되는 메서드들이다. 그렇지만 이 메서드들을 전부 override할 필요는 없다. 이벤트를 제어하기 위해 gesture recognizer를 사용하면 이벤트 핸들링 메서드를 전혀 사용할 필요가 없다. 유사하게 서브 뷰를 포함하고 있지 않거나, 그것의 사이즈가 변하지 않으면 `layoutSubviews` 메서드를 override할 이유가 없다. 마지막으로 `drawRect:` 메서드는 런타임에 뷰의 컨텐트가 변할 수 있거나, UIKit이나 Core Graphics과 같은 네이티브 기술을 이용해서 드로잉을 수행할 때만 필요하다. 

이 주요 통합 지점들을 기억하는 것도 중요하지만 이 지점이 유일한 지점은 아니라는 것을 기억해야 한다. UIView 클래스의 몇몇 메서드는 서브 클래스의 포인트들을 override하도록 설계되어 있다. 어떤 메서드가 커스텀 구현에서 override 하기 적합한 지 알기 위해서는 [UIView class Reference](https://developer.apple.com/documentation/uikit/uiview)의 메서드 설명을 봐야 한다.



### 뷰를 효과적으로 쓰기 위한 팁

커스텀 뷰는 표준 시스템 뷰가 제공하지 않는 것을 그려야 할 때 유용하다. 그러나 뷰의 성능을 보장하는 것은 개발자의 책임이다. UIKit은 뷰와 관련된 행위를 최적화할 수 있는만큼 해주고 커스텀 뷰에서 좋은 성능을 얻을 수 있도록 돕는다. 그러나 아래 팁들을 따르면 UIKit을 도울 수 있다.

> 드로잉 코드를 최적화시키기 전에 항상 현재 뷰의 성능과 관련된 데이터를 모아야 한다. 현재 성능을 측정하면 문제가 실제로 있는 지를 확인할 수 있고, 있을 경우 최적화 시키고 난 후 비교할 수 있는 기준 측정값을 얻을 수 있다.



#### 뷰는 항상 상응하는 뷰 컨트롤러를 가지지 않는다

뷰와 뷰 컨트롤러가 일대일 관계를 가지는 경우는 거의 없다. 뷰 컨트롤러의 역할은 뷰 계층 구조를 관리하는 것이고, 이는 종종 자기 자신을 포함하는 기능을 구현하는 데 사용되는 둘 이상의 뷰로 구성된다. 아이폰 앱에서, 각각의 뷰 계층 구조는 전체 화면을  채운다. 아이패드 앱의 뷰 계층 구조는 화면의 일부를 채울 수도 있다.

앱의 유저 인터페이스를 설계할 때, 뷰 컨트롤러가 어떤 역할을 할 지를 고려하는 것이 중요하다. 뷰 컨트롤러는 화면에 뷰를 표현하고, 뷰를 화면에서 없애고, 낮은 메모리 상황일 때 메모리를 해제하기도 하고 인터페이스의 방향이 변화함에 따라 뷰를 회전시키는 등의 중요한 역할을 한다. 이러한 동작을 피하면 앱이 잘못 작동하거나 의도하지 않은 방식으로 작동할 수 있다. 뷰 컨트롤러와 그 역할에 대해 더 알고 싶다면, [View Controller Programming Guide for iOS](https://developer.apple.com/library/archive/featuredarticles/ViewControllerPGforiPhoneOS/index.html#//apple_ref/doc/uid/TP40007457) 를 봐라.



#### 커스텀 드로잉을 최소화한다

커스텀 드로잉이 필요할 때가 있지만 피할 수 있다면 피하는 것이 좋다. 커스텀 드로잉이 필요할 때는 시스템 뷰 클래스가 당신이 필요한 대로 그려주지 못할 때 뿐이다. 컨텐츠를 기존 뷰의 조합으로 구성할 수 있다면 가장 좋은 방법은 커스텀 계층 구조로 결합하는 것이다. 



#### content mode를 활용해라

content mode는 뷰를 다시 그리는 시간을 최소화한다. 기본적으로 뷰는 `UIIViewContentModeScaleToFill` content mode를 사용하는데, 이는 뷰의 내용을 뷰의 프레임 사각형에 맞게 크기를 조절한다. 이 모드를 필요에 따라 변화시킬 수 있다. 하지만 `UIViewContentModeRedraw` 를 사용하는 것은 가능하면 피해야 한다. 어떤 content mode가 적용 되는가와 무관하게 `setNeedsDisplay` 또는 `setneedsDisplayInRect`를 호출해서 뷰가 컨텐츠를 다시 그리도록 할 수 있다.



#### 가능한 상황에서 뷰를 불투명하게 정의해라

UIKit은 각각의 뷰에서 `opaque` 프로퍼티를 사용해서 composition 작업을 최적화 할 수 있을 지를 결정한다. 이 프로퍼티의 값을 `YES`로 설정하면 커스텀 뷰는 UIKit에 이 뷰 뒤에 있는 다른 컨텐츠를 렌더링할 필요가 없음을 알린다. 적게 렌더링 할수록 드로잉 코드의 성능을 올릴 수 있다. 당연히 `opaque` 프로퍼티를 `YES`로 설정하면 뷰는 bounds 사각형을 완전히 불투명한 컨텐츠로 채우게 된다.



#### 스크롤 할 때의 드로잉 동작을 조절하라

스크롤은 짧은 시간 동안 많은 뷰 업데이트를 야기한다. 뷰의 드로잉 코드가 적절히 조절되지 않으면, 뷰의 스크롤 성능이 저하될 수 있다. 뷰의 컨텐츠가 초기 상태임을 보장하기 보다는 스크롤 작업이 시작될 때 뷰의 동작을 변화시키는 것을 고려해라. 컨텐츠 렌더링 품질을 일시적으로 줄이거나 스크롤이 진행 중일 때의 content mode를 변경시키는 것이 그 예가 될 수 있다. 스크롤 동작이 멈췄을 때, 이전 상태로 돌리거나 필요할 경우 컨텐츠를 업데이트 시킬 수 있다.



#### control에 서브 뷰를 추가해서 커스텀하지 마라

기술적으로 `UIControl`을 상속 받은 시스템 컨트롤에 서브 뷰를 추가하는 것은 가능하지만 이 방법으로 커스텀하지 마라. control은 명시적이고 잘 문서화 된 인터페이스를 통해서 커스텀이 가능하게 하고 잇다. 예를 들어 `UIButton` 클래스는 타이틀이나 버튼의 배경 이미지를 설정하기 위한 메서드를 제공한다. 정의 된 커스텀 지점은 당신의 코드가 항상 정확하게 작동할 것을 의미한다. 커스텀 이미지 뷰를 사용하는 것으로 정의된 메서드 사용을 우회하게 될 경우 현재 또는 미래에 앱이 올바르게 작동하지 않을 수 있다.