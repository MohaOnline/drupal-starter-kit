<?php

/**
 * @file
 * Rules hook definitions.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function opigno_forum_app_default_rules_configuration() {
  $items = array();

  $items['rules_create_forum_based_on_course'] = entity_import('rules_config', '{ "rules_create_forum_based_on_course" : {
    "LABEL" : "Create forum based on course",
    "PLUGIN" : "action set",
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "og" ],
    "USES VARIABLES" : { "course" : { "label" : "Course", "type" : "node" } },
    "ACTION SET" : [
      { "get_system_text_variable" : {
          "USING" : { "variable" : "forum_nav_vocabulary" },
          "PROVIDE" : { "variable_added" : { "forum_vid" : "Forum vocabulary id" } }
        }
      },
      { "data_convert" : {
          "USING" : { "type" : "integer", "value" : [ "forum-vid" ] },
          "PROVIDE" : { "conversion_result" : { "forum_vid_int" : "forum_vid" } }
        }
      },
      { "entity_fetch" : {
          "USING" : { "type" : "taxonomy_vocabulary", "id" : [ "forum-vid-int" ] },
          "PROVIDE" : { "entity_fetched" : { "forum_vid_voc" : "forum_vid_voc" } }
        }
      },
      { "entity_create" : {
          "USING" : {
            "type" : "taxonomy_term",
            "param_name" : [ "course:title" ],
            "param_vocabulary" : [ "forum-vid-voc" ]
          },
          "PROVIDE" : { "entity_created" : { "forum_term" : "Forum" } }
        }
      },
      { "entity_save" : { "data" : [ "forum-term" ], "immediate" : "1" } },
      { "og_group_content_add" : { "entity" : [ "forum-term" ], "group" : [ "course" ] } }
    ]
  }
}');

  $items['rules_forum_add'] = entity_import('rules_config', '{ "rules_forum_add" : {
    "LABEL" : "Forum Add",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "course", "Forum", "og" ],
    "REQUIRES" : [ "rules", "opigno_forum_app" ],
    "ON" : { "node_insert" : [] },
    "IF" : [
      { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "course" : "course" } } } },
      { "opigno_forum_app_is_checked" : { "node" : [ "node" ] } }
    ],
    "DO" : [
      { "component_rules_create_forum_based_on_course" : { "course" : [ "node" ] } }
    ]
  }
}');

 $items['rules_forum_update'] = entity_import('rules_config', '{ "rules_forum_update" : {
    "LABEL" : "Forum Update",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "course", "Forum", "og" ],
    "REQUIRES" : [ "og", "rules", "rules_conditional", "opigno_forum_app" ],
    "ON" : { "node_update--course" : { "bundle" : "course" } },
    "DO" : [
      { "og_get_group_content" : {
          "USING" : { "group" : [ "node" ], "entity_type" : "taxonomy_term" },
          "PROVIDE" : { "group_content" : { "group_content" : "Group content" } }
        }
      },
      { "variable_add" : {
          "USING" : { "type" : "integer", "value" : "0" },
          "PROVIDE" : { "variable_added" : { "has_forum" : "Has a forum" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "group-content" ] },
          "ITEM" : { "group_content_item" : "Group content item" },
          "DO" : [
            { "CONDITIONAL" : [
                {
                  "IF" : { "entity_is_of_bundle" : {
                      "entity" : [ "group-content-item" ],
                      "type" : "taxonomy_term",
                      "bundle" : { "value" : { "forums" : "forums" } }
                    }
                  },
                  "DO" : [
                    { "data_set" : { "data" : [ "has-forum" ], "value" : "1" } },
                    { "entity_fetch" : {
                        "USING" : { "type" : "taxonomy_term", "id" : [ "group-content-item:tid" ] },
                        "PROVIDE" : { "entity_fetched" : { "fetched_term" : "Fetched term" } }
                      }
                    },
                    { "data_set" : { "data" : [ "fetched-term:name" ], "value" : [ "node:title" ] } }
                  ]
                }
              ]
            }
          ]
        }
      },
      { "CONDITIONAL" : [
          {
            "IF" : { "text_matches" : { "text" : [ "has-forum" ], "match" : "0" } },
            "DO" : [
              { "CONDITIONAL" : [
                  {
                    "IF" : { "opigno_forum_app_is_checked" : { "node" : [ "node" ] } },
                    "DO" : [
                      { "component_rules_create_forum_based_on_course" : { "course" : [ "node" ] } }
                    ]
                  }
                ]
              }
            ]
          }
        ]
      }
    ]
  }
}');

  return $items;
}
