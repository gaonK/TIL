# 4. Function (2)

## PrimeNumber 예제

하나의 큰 메서드를 두 개의 클래스로 분리한다. 

`PrimePrinter` -> `PrimeGenerator`, `NumberPrinter`

원칙을 잘 따르다보면, 디자인이 예쁜 코드가 나온다. 20줄 정도면 큰 함수라고 볼 수 있다. 큰 함수를 볼 때 클래스로 추출할 생각을 해야 한다.



## One thing

* Function should do one thing, do it well, do it only. 함수는 한 가지를 해야 하고, 그 한 가지를 잘 해야 하고 그것은 유일해야(중복이 없어야) 한다.
* 하나 이상의 섹션으로 구성된 함수는 적어도 reader 입장에서는 one thing을 하는 것이 아니다. 함수를 작성할 때는 reader를 고려해야 한다. 살아있는 코드라면 굉장히 많은 사람에게 읽히게 된다. Reader가 읽기 쉬운, 이해하기 쉬운 코드를 작성해야 한다.
* 큰 함수를 작은 함수들로 쪼갤 때 흥미로운 일을 수행한다. 주요 섹션들을 함수로 추출하고, 함수를 서로 다른 추상화 레벨로 분리한다. 추상화 수준은 자로 잴 수 있는 것이 아니기 때문에 이는 어려운 작업이다.
* Extract Till You Drop! 포기할 때까지 추출해라. 더 이상 extract 할 수 없을 때까지! 그러면 4줄 이내의 함수들로만 구성된 클래스를 얻을 수 있을 것이다. if, while 문 등에서 { } 가 보인다면 이는 extract 대상이다. 



## Conclusion

* 1st rule: function should be small
* 2nd rule: smaller than that
* 이름을 잘 지으면 모든 사람들의 시간을 절약해 준다. 하지만 이름이 처음부터 잘 떠오르지 않는다. 그렇다면 무의미한 이름을 일단 짓고, 구체적으로 extract한 다음 이름을 지어주는 것이 괜찮은 방법일 수 있다.
* 함수는 한 가지 일만 해야 하고, 한 가지만 하는지 확신할 수 있는 유일한 방법은 extract till you drop이다.

