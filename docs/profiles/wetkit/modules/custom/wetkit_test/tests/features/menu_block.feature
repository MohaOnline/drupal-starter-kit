Feature: Test Menu Blocks
  In order to know that Menu Blocks are functioning as expected
  As a site administrator
  I need to be able to trust that creating, editing and theming menu blocks will work consistently

  @api @javascript @wetkit_core
  Scenario: Single previews on the 'Add content' dialog
    Given I am logged in as a user with the "administrator" role
      And Panopoly magic live previews are disabled
      And Panopoly magic add content previews are single
      And I am viewing a landing page
    When I customize this page with the Panels IPE
      And I click "Add new pane" in the "Pearson Content" region
    And I click "View panes" in the "CTools modal" region
    Then I should see "View: WxT Theme: Home Highlight" in the "CTools modal" region
    When I click "View: WxT Theme: Home Highlight" in the "CTools modal" region
    Then I should see "Most requested services and information"
