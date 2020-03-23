import fitnesse.responders.run.SuiteResponder;
import fitnesse.wiki.*;

class SetupTeardownSurrounder {
  private PageData pageData;
  private boolean includeSuiteSetup;
  private WikiPage wikiPage;
  private String content;

  public SetupTeardownSurrounder(PageData pageData, boolean includeSuiteSetup) {
    this.pageData = pageData;
    this.includeSuiteSetup = includeSuiteSetup;
    wikiPage = pageData.getWikiPage();
    content = "";
  }

  public String surround() throws Exception {
    if (isTestPage()) {
      surroundPageWithSetUpsAndTearDowns();
    }

    return pageData.getHtml();
  }

  private void surroundPageWithSetUpsAndTearDowns() throws Exception {
    content = includeSetups();
    content += pageData.getContent();
    content += includeTeardowns();
    pageData.setContent(content);
  }

  private boolean isTestPage() throws Exception {
    return pageData.hasAttribute("Test");
  }

  private String includeTeardowns() throws Exception {
    String teardowns = includeInherited("teardown", "TearDown");
    if (includeSuiteSetup) {
      teardowns += includeInherited("teardown", SuiteResponder.SUITE_TEARDOWN_NAME);
    }
    return teardowns;
  }

  private String includeSetups() throws Exception {
    String setups = "";
    if (includeSuiteSetup) {
      setups += includeInherited("setup", SuiteResponder.SUITE_SETUP_NAME);
    }
    setups += includeInherited("setup", "SetUp");
    return setups;
  }

  private String includeInherited(String mode, String pageName) throws Exception {
    WikiPage suiteSetup = PageCrawlerImpl.getInheritedPage(pageName, wikiPage);
    if (suiteSetup != null) {
      return includePage(mode, suiteSetup);
    }
    return "";
  }

  private String includePage(String mode, WikiPage suiteSetup) throws Exception {
    WikiPagePath pagePath = wikiPage.getPageCrawler().getFullPath(suiteSetup);
    String pagePathName = PathParser.render(pagePath);
    return String.format("!include -%s .%s\n", mode, pagePathName);
  }
}
