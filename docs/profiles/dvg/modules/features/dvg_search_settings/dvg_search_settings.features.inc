<?php
/**
 * @file
 * dvg_search_settings.features.inc
 */

/**
 * Implements hook_default_search_api_index().
 */
function dvg_search_settings_default_search_api_index() {
  $items = array();
  $items['default_node_index'] = entity_import('search_api_index', '{
    "name" : "Default node index",
    "machine_name" : "default_node_index",
    "description" : "An automatically created search index for indexing node data. Might be configured to specific needs.",
    "server" : "custom",
    "item_type" : "node",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "data_alter_callbacks" : {
        "search_api_alter_node_status" : { "status" : 1, "weight" : "-50", "settings" : [] },
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-49",
          "settings" : {
            "default" : "1",
            "bundles" : { "general_text" : "general_text", "webform" : "webform", "crisis_banner" : "crisis_banner", "crisis_block" : "crisis_block" }
          }
        },
        "search_api_noindex_alter_noindex_filter" : { "status" : 1, "weight" : "-48", "settings" : [] },
        "search_api_alter_node_access" : { "status" : 1, "weight" : "-47", "settings" : [] },
        "search_api_alter_language_control" : {
          "status" : 0,
          "weight" : "-46",
          "settings" : { "lang_field" : "", "languages" : [] }
        },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "-45", "settings" : { "fields" : [] } },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "-44", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "-43", "settings" : [] },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "-42", "settings" : { "mode" : "full" } }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : {
              "title" : true,
              "field_alternate_keywords" : true,
              "field_introduction" : true,
              "field_search_result" : true,
              "body:value" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "fields" : {
              "title" : true,
              "field_alternate_keywords" : true,
              "field_introduction" : true,
              "field_search_result" : true,
              "body:value" : true
            },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_transliteration" : {
          "status" : 0,
          "weight" : "15",
          "settings" : { "fields" : {
              "title" : true,
              "field_alternate_keywords" : true,
              "field_introduction" : true,
              "field_search_result" : true,
              "body:value" : true
            }
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : {
            "fields" : {
              "title" : true,
              "field_alternate_keywords" : true,
              "field_introduction" : true,
              "field_search_result" : true,
              "body:value" : true
            },
            "spaces" : "[^\\\\p{L}\\\\p{N}]",
            "ignorable" : "[-]"
          }
        },
        "search_api_stopwords" : {
          "status" : 1,
          "weight" : "30",
          "settings" : {
            "fields" : {
              "title" : true,
              "field_alternate_keywords" : true,
              "field_introduction" : true,
              "field_search_result" : true,
              "body:value" : true
            },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc\\r\\naan\\r\\naangaande\\r\\naangezien\\r\\nachter\\r\\nachterna\\r\\nafd\\r\\nafgelopen\\r\\nal\\r\\naldaar\\r\\naldus\\r\\nalhoewel\\r\\nalias\\r\\nalle\\r\\nallebei\\r\\nalleen\\r\\nals\\r\\nalsnog\\r\\naltijd\\r\\naltoos\\r\\nander\\r\\nandere\\r\\nanders\\r\\nanderszins\\r\\nbehalve\\r\\nbehoudens\\r\\nbeide\\r\\nbeiden\\r\\nben\\r\\nbeneden\\r\\nbent\\r\\nbepaald\\r\\nbetreffende\\r\\nbij\\r\\nbinnen\\r\\nbinnenin\\r\\nboven\\r\\nbovenal\\r\\nbovendien\\r\\nbovengenoemd\\r\\nbovenstaand\\r\\nbovenvermeld\\r\\nbreda\\r\\nbuiten\\r\\ndaar\\r\\ndaarheen\\r\\ndaarin\\r\\ndaarna\\r\\ndaarnet\\r\\ndaarom\\r\\ndaarop\\r\\ndaarvanlangs\\r\\ndan\\r\\ndat\\r\\nde\\r\\nden\\r\\nder\\r\\ndes\\r\\ndeze\\r\\ndie\\r\\ndikwijls\\r\\ndit\\r\\ndl\\r\\ndoor\\r\\ndoorgaand\\r\\ndr\\r\\ndus\\r\\nechter\\r\\ned\\r\\neen\\r\\neer\\r\\neerdat\\r\\neerder\\r\\neerlang\\r\\neerst\\r\\nelk\\r\\nelke\\r\\nen\\r\\nenig\\r\\nenige\\r\\nenigszins\\r\\nenkel\\r\\nenkele\\r\\nenz\\r\\ner\\r\\nerdoor\\r\\net\\r\\netc\\r\\neven\\r\\neveneens\\r\\nevenwel\\r\\ngauw\\r\\ngedurende\\r\\ngeen\\r\\ngehad\\r\\ngekund\\r\\ngeleden\\r\\ngelijk\\r\\ngemeente\\r\\ngemoeten\\r\\ngemogen\\r\\ngeweest\\r\\ngewoon\\r\\ngewoonweg\\r\\nhaar\\r\\nhad\\r\\nhadden\\r\\nhare\\r\\nheb\\r\\nhebben\\r\\nhebt\\r\\nheeft\\r\\nhem\\r\\nhen\\r\\nhet\\r\\nhierbeneden\\r\\nhierboven\\r\\nhierin\\r\\nhij\\r\\nhoe\\r\\nhoewel\\r\\nhun\\r\\nhunne\\r\\nik\\r\\nikzelf\\r\\nin\\r\\ninmiddels\\r\\ninzake\\r\\nis\\r\\nje\\r\\njezelf\\r\\njij\\r\\njijzelf\\r\\njou\\r\\njouw\\r\\njouwe\\r\\njuist\\r\\njullie\\r\\nkan\\r\\nklaar\\r\\nkon\\r\\nkonden\\r\\nkrachtens\\r\\nkunnen\\r\\nkunt\\r\\nlater\\r\\nliever\\r\\nmaar\\r\\nmag\\r\\nmeer\\r\\nmet\\r\\nmezelf\\r\\nmij\\r\\nmijn\\r\\nmijnent\\r\\nmijner\\r\\nmijzelf\\r\\nmisschien\\r\\nmocht\\r\\nmochten\\r\\nmoest\\r\\nmoesten\\r\\nmoet\\r\\nmoeten\\r\\nmogen\\r\\nna\\r\\nnaar\\r\\nnabij\\r\\nnadat\\r\\nnet\\r\\nniet\\r\\nno\\r\\nnoch\\r\\nnog\\r\\nnogal\\r\\nnu\\r\\nof\\r\\nofschoon\\r\\nom\\r\\nomdat\\r\\nomhoog\\r\\nomlaag\\r\\nomstreeks\\r\\nomtrent\\r\\nomver\\r\\nonder\\r\\nondertussen\\r\\nongeveer\\r\\nons\\r\\nonszelf\\r\\nonze\\r\\nook\\r\\nop\\r\\nopnieuw\\r\\nopzij\\r\\nover\\r\\novereind\\r\\noverigens\\r\\npas\\r\\nprecies\\r\\nreeds\\r\\nrond\\r\\nrondom\\r\\nsedert\\r\\nsinds\\r\\nsindsdien\\r\\nsl\\r\\nslechts\\r\\nsommige\\r\\nspoedig\\r\\nst\\r\\nsteeds\\r\\ntamelijk\\r\\nte\\r\\ntegen\\r\\nten\\r\\ntenzij\\r\\nter\\r\\nterwijl\\r\\nthans\\r\\ntijdens\\r\\ntoch\\r\\ntoen\\r\\ntoenmaals\\r\\ntoenmalig\\r\\ntot\\r\\ntotdat\\r\\ntussen\\r\\nuit\\r\\nuitgezonderd\\r\\nvaak\\r\\nvan\\r\\nvanaf\\r\\nvandaan\\r\\nvanuit\\r\\nvanwege\\r\\nveeleer\\r\\nverder\\r\\nvervolgens\\r\\nvol\\r\\nvolgens\\r\\nvoor\\r\\nvooraf\\r\\nvooral\\r\\nvooralsnog\\r\\nvoorbij\\r\\nvoordat\\r\\nvoordezen\\r\\nvoordien\\r\\nvoorheen\\r\\nvoorop\\r\\nvooruit\\r\\nvrij\\r\\nvroeg\\r\\nwaar\\r\\nwaarom\\r\\nwanneer\\r\\nwant\\r\\nwaren\\r\\nwas\\r\\nwat\\r\\nweer\\r\\nweg\\r\\nwegens\\r\\nwel\\r\\nweldra\\r\\nwelk\\r\\nwelke\\r\\nwie\\r\\nwiens\\r\\nwier\\r\\nwij\\r\\nwijzelf\\r\\nzal\\r\\nze\\r\\nzelfs\\r\\nzichzelf\\r\\nzij\\r\\nzijn\\r\\nzijne\\r\\nzo\\r\\nzodra\\r\\nzonder\\r\\nzou\\r\\nzouden\\r\\nzowat\\r\\nzulke\\r\\nzullen\\r\\nzult"
          }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "35",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always"
          }
        }
      },
      "fields" : {
        "author" : { "type" : "integer", "entity_type" : "user" },
        "body:value" : { "type" : "text" },
        "field_alternate_keywords" : { "type" : "text", "boost" : "5.0" },
        "field_boost_keywords" : { "type" : "text", "boost" : "21.0" },
        "field_introduction" : { "type" : "text" },
        "field_menu_description" : { "type" : "text" },
        "field_search_result" : { "type" : "text" },
        "field_sections" : {
          "type" : "list\\u003Cinteger\\u003E",
          "entity_type" : "field_collection_item"
        },
        "field_sections:field_body:value" : { "type" : "list\\u003Ctext\\u003E" },
        "field_sections:field_title" : { "type" : "list\\u003Ctext\\u003E", "boost" : "2.0" },
        "language" : { "type" : "string" },
        "nid" : { "type" : "integer" },
        "search_api_access_node" : { "type" : "list\\u003Cstring\\u003E" },
        "search_api_language" : { "type" : "string" },
        "status" : { "type" : "integer" },
        "title" : { "type" : "text", "boost" : "5.0" },
        "type" : { "type" : "string" },
        "url" : { "type" : "uri" }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function dvg_search_settings_default_search_api_server() {
  $items = array();
  $items['custom'] = entity_import('search_api_server', '{
    "name" : "Custom",
    "machine_name" : "custom",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "1",
      "partial_matches" : 1,
      "indexes" : { "default_node_index" : {
          "type" : {
            "table" : "search_api_db_default_node_index",
            "column" : "type",
            "type" : "string",
            "boost" : "1.0"
          },
          "title" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "5.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_default_node_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "body:value" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "url" : {
            "table" : "search_api_db_default_node_index",
            "column" : "url",
            "type" : "uri",
            "boost" : "1.0"
          },
          "field_alternate_keywords" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "5.0"
          },
          "field_search_result" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_introduction" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_sections" : {
            "table" : "search_api_db_default_node_index_field_sections",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_menu_description" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "nid" : {
            "table" : "search_api_db_default_node_index",
            "column" : "nid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "search_api_access_node" : {
            "table" : "search_api_db_default_node_index_search_api_access_node",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "status" : {
            "table" : "search_api_db_default_node_index",
            "column" : "status",
            "type" : "integer",
            "boost" : "1.0"
          },
          "author" : {
            "table" : "search_api_db_default_node_index",
            "column" : "author",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_sections:field_title" : {
            "table" : "search_api_db_default_node_index_text",
            "column" : "value",
            "type" : "list\\u003Ctext\\u003E",
            "boost" : "2.0"
          },
          "field_sections:field_body:value" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "list\\u003Ctext\\u003E",
            "boost" : "1.0"
          },
          "field_boost_keywords" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "21.0"
          },
          "language" : {
            "table" : "search_api_db_default_node_index",
            "column" : "language",
            "type" : "string",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}
