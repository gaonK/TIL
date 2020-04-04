class Money {
  protected int amount;
  protected String currency;

  Money(int amount, String currency) {
    this.amount = amount;
    this.currency = currency;
  }

  static Money dollar(int amount) {
    return new Money(amount, "USD");
  }

  static Money franc(int amount) {
    return new Money(amount, "CHF");
  }

  String currency() {
    return currency;
  }

  Money times(int multiplier) {
    return new Money(amount * multiplier, currency);
  }

  public boolean equals(Object object) {
    Money money = (Money) object;
    return amount == money.amount && currency().equals(money.currency());
  }

  public String toString() {
    return amount + " " + currency;
  }
}
