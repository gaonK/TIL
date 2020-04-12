import org.junit.Test

class RandomNumberGeneratorTest {
  @Test
  fun canGenerateRandomNumber() {
    // TODO: 흠 random 섞이면 test를 어떻게 하는 게 좋으려나~~ 일단 잘래!
    val numberGenerator = RandomNumberGenerator()
    val randomNumber = numberGenerator.generate()
  }
}