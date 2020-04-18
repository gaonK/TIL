import junit.framework.Assert.assertEquals
import org.junit.Test

class PrimeFactorsTest {
    @Test
    fun canFactorIntoPrimes() {
        assertPrimeFactors(1, listOf())
        assertPrimeFactors(2, listOf(2))
        assertPrimeFactors(3, listOf(3))
        assertPrimeFactors(4, listOf(2, 2))
        assertPrimeFactors(5, listOf(5))
        assertPrimeFactors(6, listOf(2, 3))
        assertPrimeFactors(7, listOf(7))
        assertPrimeFactors(8, listOf(2, 2, 2))
        assertPrimeFactors(9, listOf(3, 3))
        assertPrimeFactors(10, listOf(2, 5))
        assertPrimeFactors(2 * 2 * 3 * 5 * 7 * 101, listOf(2, 2, 3, 5, 7, 101))
    }

    private fun assertPrimeFactors(i: Int, listOf: List<Int>) {
        assertEquals(listOf, of(i))
    }

    private fun of(n: Int): List<Int> {
        var i = n
        val factors = mutableListOf<Int>()
        var divisor = 2
        while (i > 1) {
            while (i % divisor == 0) {
                factors.add(divisor)
                i /= divisor
            }
            divisor += 1
        }
        return factors
    }
}