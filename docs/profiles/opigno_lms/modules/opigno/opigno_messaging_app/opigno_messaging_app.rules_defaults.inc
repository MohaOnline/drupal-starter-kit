<?php

/**
 * @file
 * Contains default rules for opigno_messaging_app.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function opigno_messaging_app_default_rules_configuration() {
  $items = array();

  $entity = entity_import('rules_config', '{ "rules_course_managers_subscription_message_notification" : {
    "LABEL" : "Course managers subscription message notification",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "course", "notifications" ],
    "REQUIRES" : [ "rules", "rules_conditional", "rules_i18n", "privatemsg_rules", "og" ],
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
                    { "privatemsg_rules_new" : {
                        "USING" : {
                          "recipient" : [ "course-member" ],
                          "author" : [ "og-membership:entity" ],
                          "subject" : "New course subscription",
                          "body" : "I\u0027ve subscribed to the course and waiting for your approval."
                        },
                        "PROVIDE" : { "thread_id" : { "thread_id" : "ID of new thread" } }
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

  if ($entity) {
    $items['rules_course_managers_subscription_message_notification'] = $entity;
  }

  return $items;
}
