import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

public class PrintPrimesTest {
  private PrintStream out;

  @Before
  public void setUp() throws FileNotFoundException {
    out = System.out;
    System.setOut(new PrintStream(new FileOutputStream("lead")));
  }

  @After
  public void tearDown() {
    System.setOut(out);
    new File("lead").delete();
  }

  @Test
  public void
  makeSureMatchesGold() throws IOException {
    PrintPrimes.main(new String[0]);
    BufferedReader leadReader = new BufferedReader(new FileReader("lead"));
    BufferedReader goldReader = new BufferedReader(new FileReader("src/test/resources/gold"));
    String line;
    while ((line = goldReader.readLine()) != null) {
      assertEquals(line, leadReader.readLine());
    }
    assertNull(leadReader.readLine());
  }
}