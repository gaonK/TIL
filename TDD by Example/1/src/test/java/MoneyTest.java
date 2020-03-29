import org.junit.Test;

import static org.junit.Assert.assertEquals;

// 1. 작은 테스트를 하나 추가한다.
// 2. 모든 테스트를 실행해서 테스트가 실패하는 것을 확인한다.
// 3. 조금 수정한다.
// 4. 모든 테스트를 실행해서 테스트가 성공하는 것을 확인한다.
// 5. 중복을 제거하기 위해 리팩토링을 한다.
public class MoneyTest {

  // multiplication 구현하기.
  // 먼저 test를 작성하고, 컴파일 에러를 해결하자
  @Test
  public void testMultiplication() {
    Dollar five = new Dollar(5);
    five.times(2);
    assertEquals(10, five.amount);
  }
}