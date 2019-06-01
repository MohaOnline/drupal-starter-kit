Feature: Breadcrumb
  In order to know that breadcrumbs are functional and correctly displaying
  As a website user
  I need to see that the breadcrumb can be created normally

  @api
  Scenario: User leveraging the breadcrumbs to navigate the site
    # Check breadcrumb on english demo page
    Given I am on "en/content/drupal-wxt"
    Then I should see the breadcrumb "Drupal WxT"
    # Check breadcrumb on french demo page
    And I am on "fr/contenu/wxt-drupal"
    Then I should see the breadcrumb "WxT Drupal"

  @api @wetkit_admin
  Scenario: User creating breadcrumbs while creating new content
    Given I am logged in as a user with the "administrator" role
    # Create a test page to test breadcrumbs
    When I visit "/node/add/wetkit-page"
      And I fill in the following:
        | title_field[und][0][value] | Title of Basic Page |
        | body[und][0][format]       | plain_text          |
        | body[und][0][value]        | Published body      |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
    When I press "edit-submit"
    Then the "h1" element should contain "Title of Basic Page"
    And I should see the breadcrumb "Home"
    And I should see the breadcrumb "Title of Basic Page"
    # Translate test page to check that french breadcrumb appears
    When I click "Translate" in the "Tabs" region
    And I click the fake "add" button
      And I fill in the following:
        | edit-title-field-fr-0-value   | French Title of Basic Page  |
      And I select "Published" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "French Title of Basic Page"
    And I should see the breadcrumb "French Title of Basic Page"
