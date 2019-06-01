Feature: Date Modified
  In order to know that the Date Modified ctools plugin is functioning correctly
  As a site administrator
  I need to confirm the date displayed is tied to the node

  @api @wetkit_admin @javascript
  Scenario: User creates and then translates a basic_page
    Given I am logged in as a user with the "administrator" role
    # Create a basic page
    When I visit "/node/add/wetkit-page"
      And I fill in the following:
        | title_field[und][0][value] | Title of Basic Page |
        | body[und][0][format]       | plain_text          |
        | body[und][0][value]        | Published body      |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
    When I press "edit-submit"
    # Check the fields of the basic page
    Then the "h1" element should contain "Title of Basic Page"
        And I should see the breadcrumb "Title of Basic Page"
        And the ".field-item p" element should contain "Published body"
        And I should see the modified date in the "time" element in the "Pearson Content" region
