Feature: Basic Page Fields
  In order to know that Basic Page content type fields are functioning correctly
  As a site administrator
  I need to enter information in the fields and see everything displayed as expected

  @api @wetkit_admin @javascript
  Scenario: User creates and then translates a basic page with moderation
    Given I am logged in as a user with the "administrator" role
    # Create a basic page
    When I visit "/node/add/wetkit-page"
      And I fill in the following:
        | title_field[und][0][value] | Title of Basic Page |
        | body[und][0][format]       | plain_text          |
        | body[und][0][value]        | Published body      |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
      And I check "field_featured_categories[und][1]"
      And I check "field_featured_categories[und][2]"
    When I press "edit-submit"
    # Check the fields of the basic page
    Then the "h1" element should contain "Title of Basic Page"
        And I should see the breadcrumb "Title of Basic Page"
        And the ".field-item p" element should contain "Published body"
        And I should see "departments" in the "Content Well"
        And I should see "features" in the "Content Well"
    # Translate the basic page
    When I click "Translate" in the "Tabs" region
      And I click the fake "add" button
      And I fill in the following:
        | title_field[fr][0][value] | French Title of Basic Page  |
        | body[fr][0][format]       | plain_text                  |
        | body[fr][0][value]        | French Published body       |
      And I select "Published" from "workbench_moderation_state_new"
    When I press "edit-submit"
    # Check the fields of the translated page
    Then the "h1" element should contain "French Title of Basic Page"
        And I should see the breadcrumb "French Title of Basic Page"
        And the ".field-item p" element should contain "French Published body"
        And I should see "departments" in the "Content Well"
        And I should see "features" in the "Content Well"
    # Create a New Draft in French
    When I click "Nouveau brouillon" in the "Tabs" region
      And I fill in the following:
        | title_field[fr][0][value] | French Title of Basic Page Draft |
      And I select "Draft (Current)" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "French Title of Basic Page Draft"
    # Edit the Draft in French
    When I click "Modifier le brouillon" in the "Tabs" region
    Then the "title_field[fr][0][value]" field should contain "French Title of Basic Page Draft"
