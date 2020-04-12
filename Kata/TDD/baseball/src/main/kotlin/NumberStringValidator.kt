class NumberStringValidator {
  fun validate(input: String): Boolean {
    return isNumberStringLengthThree(input) && isValidNumberString(input)
  }

  private fun isNumberStringLengthThree(input: String): Boolean {
    return input.length == 3
  }

  private fun isValidNumberString(input: String): Boolean {
    for (character in input) {
      if (!isValidCharacter(character)) {
        return false
      }
    }
    return true
  }

  private fun isValidCharacter(character: Char): Boolean {
    return character in '1'..'9'
  }
}
