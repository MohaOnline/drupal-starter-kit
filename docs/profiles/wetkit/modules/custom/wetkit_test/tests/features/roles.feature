Feature: Roles and permissions
  In order to know that roles and permissions are functional
  As a website user
  I need to be able to add roles and permissions without error

  @api @wetkit_admin
  Scenario: User creates a new role and assigns a permission
    Given I am logged in as a user with the "administrator" role
    When I visit "/admin/people/permissions/roles"
      And I fill in the following:
        | edit-name      | Test New Role |
    And I press "edit-add"
    Then I should see "Test New Role" in the "Content Well"
    Given I am on "admin/people/permissions/5"
    And I check "edit-5-administer-administration-menu-select"
    And I press "edit-submit"
    Then I should see "The changes have been saved."
