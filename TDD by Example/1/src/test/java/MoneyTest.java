import org.junit.Test;

import static org.junit.Assert.*;

// 1. 작은 테스트를 하나 추가한다.
// 2. 모든 테스트를 실행해서 테스트가 실패하는 것을 확인한다.
// 3. 조금 수정한다.
// 4. 모든 테스트를 실행해서 테스트가 성공하는 것을 확인한다.
// 5. 중복을 제거하기 위해 리팩토링을 한다.
public class MoneyTest {
  @Test
  public void testMultiplication() {
    Money five = Money.dollar(5);
    assertEquals(Money.dollar(10), five.times(2));
    assertEquals(Money.dollar(15), five.times(3));
  }

  @Test
  public void testFrancMultiplication() {
    Money five = Money.franc(5);
    assertEquals(Money.franc(10), five.times(2));
    assertEquals(Money.franc(15), five.times(3));
  }

  @Test
  public void testEquality() {
    assertTrue(Money.dollar(5).equals(Money.dollar(5)));
    assertFalse(Money.dollar(5).equals(Money.dollar(6)));
    assertFalse(Money.franc(5).equals(Money.dollar(5)));
  }

  @Test
  public void testCurrency() {
    assertEquals("USD", Money.dollar(1).currency());
    assertEquals("CHF", Money.franc(1).currency());
  }

  @Test
  public void testDifferentClassEquality() {
    assertTrue(new Money(10, "CHF").equals(new Money(10, "CHF")));
  }
}