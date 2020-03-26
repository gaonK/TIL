public class PrintPrimes {
  private static final int numberOfPrimes = 1000;
  private static final int linesPerPage = 50;
  private static final int columns = 4;

  public static void main(String[] args) {
    PrimeGenerator primeGenerator = new PrimeGenerator();
    int[] primes = primeGenerator.generate(numberOfPrimes);
    new NumberPrinter(linesPerPage, columns).printNumber(primes, numberOfPrimes);
  }
}
