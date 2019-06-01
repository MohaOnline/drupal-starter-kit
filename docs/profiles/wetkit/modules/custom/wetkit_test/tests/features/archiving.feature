Feature: Basic Page Archiving
  In order to know that the Basic Page content type can be Archived
  As a site administrator
  I need to be able to Archive a Basic Page and see the Archived notice displayed as expected

Background:
    Given I am logged in as a user with the "administrator" role
    When I visit "/node/add/wetkit-page"
      And I fill in the following:
        | title_field[und][0][value] | Title of Basic Page |
        | body[und][0][format]       | plain_text          |
        | Body                       | Published body      |
      And I select "Published" from "workbench_moderation_state_new"
      And I check "field_archived_content[und]"
    When I press "edit-submit"

  @api @wetkit_admin
  Scenario: Creating a Basic Page and setting it to Archived has the expected results
    Then the "h1" element should contain "Title of Basic Page"
    And I should see "This page has been archived on the Web" in the "Content Well"
