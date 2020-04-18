public class PrimeGenerator {
  private final int ordmax = 30;
  private int[] primes;
  private int ord;
  private int square;
  private int[] multiples;

  public int[] generate(int numberOfPrimes) {
    primes = new int[numberOfPrimes + 1];
    multiples = new int[ordmax + 1];
    primes[1] = 2;
    ord = 2;
    square = 9;
    int candidate = 1;
    int primeIndex = 1;
    while (primeIndex < numberOfPrimes) {
      candidate = findNextPrime(candidate);
      primeIndex = primeIndex + 1;
      primes[primeIndex] = candidate;
    }

    return primes;
  }

  private int findNextPrime(int candidate) {
    boolean possiblyPrime;
    do {
      candidate = candidate + 2;
      if (candidate == square) {
        ord = ord + 1;
        square = primes[ord] * primes[ord];
        multiples[ord - 1] = candidate;
      }
      int n = 2;
      possiblyPrime = true;
      while (n < ord && possiblyPrime) {
        while (multiples[n] < candidate)
          multiples[n] = multiples[n] + 2 * primes[n];
        if (multiples[n] == candidate)
          possiblyPrime = false;
        n = n + 1;
      }
    } while (!possiblyPrime);
    return candidate;
  }
}
