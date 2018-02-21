<?php

/**
 * @file
 * Default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function opigno_default_rules_configuration() {
  $items = array();
  $items['rules_activate_group_membership'] = entity_import('rules_config', '{ "rules_activate_group_membership" : {
      "LABEL" : "Activate group membership",
      "PLUGIN" : "rule",
      "TAGS" : [ "og", "opigno" ],
      "REQUIRES" : [ "og", "opigno", "rules" ],
      "USES VARIABLES" : {
        "course" : { "label" : "Course", "type" : "node" },
        "user" : { "label" : "User", "type" : "user" }
      },
      "IF" : [
        { "og_entity_is_group" : { "entity" : [ "course" ] } },
        { "og_user_in_group" : {
            "account" : [ "user" ],
            "group" : [ "course" ],
            "states" : { "value" : { "2" : "2" } }
          }
        }
      ],
      "DO" : [
        { "opigno_load_og_membership" : {
            "USING" : { "user" : [ "user" ], "group" : [ "course" ] },
            "PROVIDE" : { "og_membership" : { "og_membership" : "OG Membership" } }
          }
        },
        { "data_set" : { "data" : [ "og-membership:state" ], "value" : "1" } }
      ]
    }
  }');

  $items['rules_course_managers_subscription_email_notification'] = entity_import('rules_config', '{ "rules_course_managers_subscription_email_notification" : {
    "LABEL" : "Course managers subscription email notification",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "course", "notifications" ],
    "REQUIRES" : [ "rules", "rules_conditional", "rules_i18n", "og" ],
    "ON" : { "og_user_insert" : [] },
    "IF" : [
      { "entity_is_of_type" : { "entity" : [ "og-membership:group" ], "type" : "node" } },
      { "node_is_of_type" : {
          "node" : [ "og-membership:group" ],
          "type" : { "value" : { "course" : "course" } }
        }
      },
      { "entity_is_of_type" : { "entity" : [ "og-membership:entity" ], "type" : "user" } },
      { "data_is" : { "data" : [ "og-membership:state" ], "value" : "2" } }
    ],
    "DO" : [
      { "LOOP" : {
          "USING" : { "list" : [ "og-membership:group:members" ] },
          "ITEM" : { "course_member" : "Course member" },
          "DO" : [
            { "CONDITIONAL" : [
                {
                  "IF" : { "og_user_has_role" : {
                      "account" : [ "course-member" ],
                      "group" : [ "og-membership:group" ],
                      "roles" : { "value" : { "manager" : "manager" } }
                    }
                  },
                  "DO" : [
                    { "mail" : {
                        "to" : [ "course-member:mail" ],
                        "subject" : "New course subscription",
                        "message" : "New user has subscribed to the course and waiting for your approval.",
                        "from" : [ "site:mail" ],
                        "language" : [ "course-member:language" ]
                      }
                    }
                  ]
                }
              ]
            }
          ]
        }
      }
    ]
  }
}');

  return $items;
}
