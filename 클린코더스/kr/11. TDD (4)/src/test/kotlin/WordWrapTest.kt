import junit .framework.Assert.assertEquals
import org.junit.Test

class WordWrapTest {
    @Test
    fun nothing() {
        assertWraps("", null, 1)
        assertWraps("", "", 1)
        assertWraps("x", "x", 1)
        assertWraps("x\nx", "xx", 1)
        assertWraps("x\nx\nx", "xxx", 1)
        assertWraps("x\nx", "x x", 1)
        assertWraps("four\nscore\nand\nseven\nyears\nago our\nfathers\nbrought\nforth\nupon\nthis\ncontine\nnt",
            "four score and seven years ago our fathers brought forth upon this continent", 7)
    }

    private fun assertWraps(expected: String, input: String?, width: Int) {
        assertEquals(expected, wrap(input, width))
    }

    private fun wrap(input: String?, width: Int): String {
        input ?: return ""

        return if (input.length <= width) {
            input
        } else {
            var breakPoint = input.lastIndexOf(' ', width)
            if (breakPoint == -1) {
                breakPoint = width
            }
            input.substring(0, breakPoint) + "\n" + wrap(input.substring(breakPoint).trim(), width)
        }
    }
}