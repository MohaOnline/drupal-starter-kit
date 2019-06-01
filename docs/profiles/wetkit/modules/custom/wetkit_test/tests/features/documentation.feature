Feature: Documentation content type
  In order to know that the Documentation content type is functioning
  As a site administrator
  I need to see the Documentation fields displayed as expected

  @api @wetkit_admin @javascript
  Scenario: User creates and then translates a documentation page
    Given I am logged in as a user with the "administrator" role
    # Create a documentation page
    When I visit "/node/add/wetkit-documentation"
      And I fill in the following:
        | title_field[en][0][value] | Title of Documentation Page  |
        | body[en][0][format]       | plain_text                   |
        | body[en][0][value]        | Documentation Published body |
      And I check "Published"
      And I select "English" from "edit-language"
      And I check "field_featured_categories[und][1]"
      And I check "field_featured_categories[und][2]"
    When I press "edit-submit"
    # Check the fields of the documentation page
    Then the "h1" element should contain "Title of Documentation Page"
      And the ".field-item p" element should contain "Documentation Published body"
      And I should see "departments" in the "Content Well"
      And I should see "features" in the "Content Well"
    # Translate the documentation page
    When I click "Translate" in the "Tabs" region
      And I click the fake "add" button
      And I fill in the following:
        | title_field[fr][0][value]  | French Title of Documentation Page  |
        | body[fr][0][format]        | plain_text                          |
        | Body                       | French Documentation Published body |
    When I press "edit-submit"
    # Check the fields of the translated documentation page
    Then I should see "French Title of Documentation Page" in the "Content Well"
      And I should see the breadcrumb "French Title of Documentation Page"
      And I should see "French Documentation Published body" in the "Content Well"
