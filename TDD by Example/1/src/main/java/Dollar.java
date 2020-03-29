class Dollar extends Money {
  Dollar(int amount, String currency) {
    super(amount, currency);
  }

  @Override
  Money times(int multiplier) {
    return Money.dollar(amount * multiplier);
  }
}
