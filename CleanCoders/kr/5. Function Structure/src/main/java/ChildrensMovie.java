public class ChildrensMovie extends Movie {
  public ChildrensMovie(String title) {
    super(title, Movie.CHILDRENS);
  }

  @Override
  double determineAmount(int daysRented) {
    double rentalAmount = 1.5;
    if (daysRented > 3)
      rentalAmount += (daysRented - 3) * 1.5;
    return rentalAmount;
}

  @Override
  int determineFrequentRentalPoint(int daysRented) {
    return 1;
  }
}
