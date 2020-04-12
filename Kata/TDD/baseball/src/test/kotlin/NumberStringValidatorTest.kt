import org.junit.Assert.assertEquals
import org.junit.Before
import org.junit.Test

class NumberStringValidatorTest {
  lateinit var validator: NumberStringValidator

  @Before
  fun setUp() {
    validator = NumberStringValidator()
  }

  @Test
  fun givenNumberStringIsInvalidWhenStringLengthIsShorterThan3() {
    val numberString = "1"
    assertEquals(false, validator.validate(numberString))
  }

  @Test
  fun givenNumberStringIsValidWhenStringLengthIs3() {
    val numberString = "111"
    assertEquals(true, validator.validate(numberString))
  }

  @Test
  fun givenNumberStringIsInvalidWhenStringLengthIsLongerThan3() {
    val numberString = "12345"
    assertEquals(false, validator.validate(numberString))
  }

  @Test
  fun givenNumberStringIsValidWhenAllCharacterIsInRange1to9() {
    val numberString = "123"
    assertEquals(true, validator.validate(numberString))
  }

  @Test
  fun givenNumberStringIsInvalidWhenNotInCorrectRange() {
    val numberString = "12a"
    assertEquals(false, validator.validate(numberString))
  }
}