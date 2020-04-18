import org.junit.Test;

import static org.junit.Assert.*;

public class NewReleaseMovieTest {

  @Test
  public void determineFrequentRentalPoint() {
    NewReleaseMovie uut = new NewReleaseMovie("sample");
    int actual1 = uut.determineFrequentRentalPoint(3);
    assertEquals(2, actual1);

    int actual2 = uut.determineFrequentRentalPoint(1);
    assertEquals(1, actual2);
  }
}