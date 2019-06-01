Feature: URL alias
  In order to know that the URL aliases are functional and can be modified
  As a website user
  I need to be able to change the URL alias in various ways

Background:
    Given I am logged in as a user with the "administrator" role
    When I visit "/node/add/wetkit-page"
      Then I fill in the following:
        | title_field[und][0][value] | Title of Basic Page |
        | body[und][0][format]       | plain_text          |
        | body[und][0][value]        | Published body      |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
    When I press "edit-submit"
    Then the "h1" element should contain "Title of Basic Page"
    Given I am on "en/content/title-basic-page"
    Then the "h1" element should contain "Title of Basic Page"

  @api @wetkit_admin @javascript
  Scenario: User saves with a new URL alias then reverts to an auto URL
    Given I am logged in as a user with the "administrator" role
    # Create basic page and test a manually-entered URL alias
    When I visit "en/content/title-basic-page"
    And I click the fake "New draft" button
      Then I fill in the following:
        | title_field[en][0][value]  | Test URL change         |
        | edit-path-alias            | content/test-url-change |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
    When I press "edit-submit"
    Then the "h1" element should contain "Test URL change"
    Given I am on "en/content/test-url-change"
    Then the "h1" element should contain "Test URL change"
    # Test the URL reset using auto-alias
    Then I click the fake "New draft" button
      Then I fill in the following:
        | title_field[en][0][value]  | Test new title and URL  |
        | edit-path-alias            |                         |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
    When I press "edit-submit"
    Then the "h1" element should contain "Test new title and URL"
    Given I am on "en/content/test-new-title-and-url"
    Then the "h1" element should contain "Test new title and URL"
    # Test the URL reset using Path alias option
    Given I am on "en/admin/config/search/path"
      Then I fill in the following:
        | edit-filter      | test-new-title-and-url |
    Then I press "Filter"
    And I click the fake "edit" button
      Then I fill in the following:
        | edit-alias     | content/test-url-alias-change |
    When I press "edit-submit"
    Given I am on "en/content/test-url-alias-change"
    Then the "h1" element should contain "Test new title and URL"
