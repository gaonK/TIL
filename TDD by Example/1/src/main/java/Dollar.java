class Dollar extends Money {
  Dollar(int amount) {
    this.amount = amount;
  }

  @Override
  Money times(int multiplier) {
    return new Dollar(amount * multiplier);
  }
}
