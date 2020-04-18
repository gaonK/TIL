import fitnesse.wiki.*;

public class FitnessExample {
  public String testableHtml(PageData pageData, boolean includeSuiteSetup) throws Exception {
    return new SetupTeardownSurrounder(pageData, includeSuiteSetup).surround();
  }

}