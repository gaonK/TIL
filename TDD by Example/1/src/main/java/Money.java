class Money implements Expression {
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

  Expression plus(Money addend) {
    return new Sum(this, addend);
  }

  Money times(int multiplier) {
    return new Money(amount * multiplier, currency);
  }

  public Money reduce(String to) {
    return this;
  }

  public boolean equals(Object object) {
    Money money = (Money) object;
    return amount == money.amount && currency().equals(money.currency());
  }

  public String toString() {
    return amount + " " + currency;
  }
}
