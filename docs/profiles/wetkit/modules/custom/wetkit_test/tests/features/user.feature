Feature: Test User Accounts
  In order to know that new user signups are coming in as expected
  As a site administrator
  I need to be able to trust that a new user account can be created consistently

  @api @wetkit_admin
  Scenario: User creates new login account and assigns editor Role
    Given I am logged in as a user with the "administrator" role
    When I visit "/admin/people/create"
      And I fill in the following:
        | Username         | WxT User Name    |
        | E-mail address   | WxT-User@test.ca |
        | Password         | Str0ngpass!      |
        | Confirm password | Str0ngpass!      |
      And I select "1" from "edit-status-1"
      And I select "4" from "edit-roles-4"
      Then I press "edit-submit"
    Then I should see "Created a new user account" in the "Content Well"
    Given I am on "en/admin/people"
    Then I should see "editor" in the "Content Well"
    Then I should see "WxT User Name" in the "Content Well"
    And I should see "WxT-User@test.ca" in the "Content Well"

  @api @wetkit_admin @javascript
  Scenario: User attempts to delete the new user account
    Given I am logged in as a user with the "administrator" role
    When I visit "en/users/wxt-user-name"
     And I click the fake "Edit" button
      Then I press "edit-cancel"
      And I select "user_cancel_delete" from "edit-user-cancel-method--5"
      Then I press "edit-submit"
    Then I should see "WxT User Name has been deleted." in the "Content Well"
