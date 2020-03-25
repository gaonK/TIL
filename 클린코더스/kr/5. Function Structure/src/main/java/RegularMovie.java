public class RegularMovie extends Movie {
  public RegularMovie(String title) {
    super(title, Movie.REGULAR);
  }

  @Override
  double determineAmount(int daysRented) {
    double rentalAmount = 2;
    if (daysRented > 2)
      rentalAmount += (daysRented - 2) * 1.5;
    return rentalAmount;
  }

  @Override
  int determineFrequentRentalPoint(int daysRented) {
    return 1;
  }
}
