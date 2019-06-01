Feature: Add landing page
  In order to create a fully customizable page
  As a site administrator
  I need to be able to create a landing page

  @api @javascript @wetkit_pages
  Scenario: Add a landing page
    Given I am logged in as a user with the "administrator" role
    When I visit "/node/add/landing_page"
      And I fill in the following:
        | Title | Testing landing page Pearson |
        | URL   | lp-pearson                   |
      And I check "menu_entry"
      And I select "Layouts-Sidebar" from "categories"
      And I select the radio button "Pearson"
      And I press "Create Page"
    Then the "title" element should contain "Testing landing page Pearson"
