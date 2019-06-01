Feature: Test Taxonomies
  In order to know that Taxonomies are functioning as expected
  As a site administrator
  I need to be able to trust that Taxonomies and Terms work consistently

  @api @wetkit_admin @javascript
  Scenario: User creates and translates a new Taxonomy category
    Given I am logged in as a user with the "administrator" role
    When I visit "/admin/structure/taxonomy/add"
      And I fill in the following:
        | Name                           | WxT Taxonomy Name           |
        | Description                    | WxT Taxonomy Description    |
      And I select "1" from "edit-i18n-mode-1"
      And I press "edit-translate"
      And I wait 2 seconds
      Then I click the fake "translate" button
    Then the "h1" element should contain "Translate to French"
      And I fill in the following:
        | Name                           | WxT Taxonomy French Name         |
        | Description                    | WxT Taxonomy French Description  |
      And I press "edit-submit"
      And I wait 2 seconds
         And I should see "2 translations were saved successfully" in the "Content Well"

 @api @wetkit_admin @javascript
  Scenario: User creates and translates a new Taxonomy term
    Given I am logged in as a user with the "administrator" role
    When I visit "/admin/structure/taxonomy/wetkit_categories"
      Then I click the fake "Add term" button
      And I fill in the following:
        | Name         | WxT Taxonomy Term |
      And I press "edit-translate"
      Then I click the fake "translate" button
      And I fill in the following:
        | Name         | WxT Taxonomy French Term |
      And I press "edit-submit"
      And I wait 2 seconds
    Given I am on "en/admin/structure/taxonomy/wetkit_categories"
    Then I should see "WxT Taxonomy Term" in the "Content Well"
