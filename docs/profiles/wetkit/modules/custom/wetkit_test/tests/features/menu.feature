Feature: Test Menus
  In order to know that Menus are functioning as expected
  As a site administrator
  I need to be able to trust that creating menus and managing menu links will work consistently

  @api @wetkit_admin @javascript
  Scenario: User create new menu and a add a menu link
    Given I am logged in as a user with the "administrator" role
    # Test creating a menu
    When I visit "/admin/structure/menu/add"
      And I fill in the following:
        | Title            | WxT menu title |
        | Description      | WxT menu description   |
      And I select "5" from "edit-i18n-mode-5"
      And I press "edit-submit"
    Then the "h1" element should contain "WxT menu title"
    # Test creating a menu link
    When I click the fake "Add link" button
      Then I click "Menu link attributes"
      And I fill in the following:
        | Menu link title                 | WxT menu link         |
        | edit-link-path                  | <front>               |
        | edit-options-attributes-title   | WxT menu link title   |
        | edit-options-attributes-rel     | nofollow              |
        | edit-options-attributes-class   | custom-menu-link-css  |
      Then I click "Menu item attributes"
      And I fill in the following:
        | edit-options-item-attributes-class | custom-menu-item-css  |
      And I select "English" from "Language"
      And I press "edit-submit"
    Then the ".custom-menu-link-css" element should contain "WxT menu link"
    And the "#menu-overview" element should contain "WxT menu link title"
    # Test translating the menu link
    When I click the fake "edit" button
      Then I click the fake "Translate" button
      Then I click the fake "add translation" button
      Then I click "Menu link attributes"
      And I fill in the following:
        | Menu link title                  | WxT menu french link         |
        | edit-options-attributes-title    | WxT menu link french title   |
        | edit-options-attributes-rel      | nofollow                     |
        | edit-options-attributes-class    | custom-menu-link-french-css  |
      Then I click "Menu item attributes"
      And I fill in the following:
        | edit-options-item-attributes-class | custom-menu-item-french-css |
      And I press "edit-submit"
    Then the ".table-responsive" element should contain "WxT menu french link"
      Then I press "edit-update"
