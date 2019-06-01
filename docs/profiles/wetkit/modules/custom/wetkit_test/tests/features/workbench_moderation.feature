Feature: Workbench Moderation
  In order to know that Workbench Moderation is functioning correctly
  As a site administrator
  I need to be able to trust that nodes are moderated consistently

Background:
    # First Publish
    Given I am logged in as a user with the "administrator" role
    When I visit "/node/add/wetkit-page"
      And I fill in the following:
        | title_field[und][0][value]  | Title (Published) |
        | body[und][0][format]        | plain_text        |
        | Body                        | Body (Published)  |
      And I select "Published" from "workbench_moderation_state_new"
      And I select "English" from "edit-language"
    When I press "edit-submit"
    Then the "h1" element should contain "Title (Published)"
    # New Draft
    When I click "New draft" in the "Tabs" region
      And I fill in the following:
        | title_field[en][0][value]  | Title (Draft)     |
        | body[en][0][format]        | plain_text        |
        | Body                       | Body (Draft)      |
    When I press "edit-submit"
    Then the "h1" element should contain "Title (Draft)"
    # Ensure Draft is not in Published section
    When I click "View published" in the "Tabs" region
    Then the "h1" element should contain "Title (Published)"
    # Assign Draft to Needs Review section
    When I click "Edit draft" in the "Tabs" region
      And I fill in the following:
        | title_field[en][0][value]  | Title (Review) |
        | body[en][0][format]        | plain_text                       |
        | Body                       | Body (Review)      |
      And I select "Needs Review" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "Title (Review)"
    # Ensure Needs Review is not in Published section
    When I click "View published" in the "Tabs" region
    Then the "h1" element should contain "Title (Published)"
    # Assign Needs Review to Published section
    When I click "Edit draft" in the "Tabs" region
      And I fill in the following:
        | title_field[en][0][value]  | Title (Published + Corrections) |
        | body[en][0][format]        | plain_text                      |
        | Body                       | Title (Published + Corrections) |
      And I select "Published" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "Title (Published + Corrections)"

  @api @wetkit_admin
  Scenario: Translating a revision should work with the various states
    # Translations should work with Moderation states
    When I click "Translate" in the "Tabs" region
      And I click the fake "add" button
      And I fill in the following:
        | title_field[fr][0][value] | Titre (Publié)        |
        | body[fr][0][format]       | plain_text            |
        | body[fr][0][value]        | Corps (Publié)        |
      And I select "Published" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "Titre (Publié)"
    # New Draft (fr)
    When I click "Nouveau brouillon" in the "Tabs" region
      And I fill in the following:
        | title_field[fr][0][value]  | Titre (Brouillon)    |
        | body[fr][0][format]        | plain_text           |
        | Body                       | Corps (Brouillon)    |
    When I press "edit-submit"
    Then the "h1" element should contain "Titre (Brouillon)"
    # Ensure Draft (fr) is not in Published (fr) section
    When I click "Révision publiée" in the "Tabs" region
    Then the "h1" element should contain "Titre (Publié)"
    # Assign Draft (fr) to Needs Review (fr) section
    When I click "Modifier le brouillon" in the "Tabs" region
      And I fill in the following:
        | title_field[fr][0][value]  | Titre (Revue)     |
        | body[fr][0][format]        | plain_text        |
        | Body                       | Corps (Revue)     |
      And I select "Needs Review" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "Titre (Revue)"
    # Ensure Needs Review (FR) is not in Published (FR) section
    When I click "Révision publiée" in the "Tabs" region
    Then the "h1" element should contain "Titre (Publié)"
    # Assign Needs Review (FR) to Published (FR) section
    When I click "Modifier le brouillon" in the "Tabs" region
      And I fill in the following:
        | title_field[fr][0][value]  | Titre (Publié + correctionnel) |
        | body[fr][0][format]        | plain_text                     |
        | Body                       | Corps (Publié + correctionnel) |
      And I select "Published" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "Titre (Publié + correctionnel)"
    # Ensure Published (en) has not been affected
    When I click "English" in the "Language Bar" region
    Then the "h1" element should contain "Title (Published + Corrections)"

  @api @wetkit_admin
  Scenario: Reverting a revision should work with the various states
    When I click "Moderate" in the "Tabs" region
      And I click "Revert" in the "Moderation" region
      And I press "edit-submit"
    Then the ".current-revision" element should contain "Title (Review)"
