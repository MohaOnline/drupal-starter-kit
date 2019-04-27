<?php

/**
 * @file
 * The PoolParty Thesaurus (PPT) API class. PoolParty >= 4.6 is supported.
 */

/**
 * Abstract Class SemanticConnectorPPTApi
 *
 * API Class for the PoolParty Thesaurus.
 */
class SemanticConnectorSparqlApi extends EasyRdf_Sparql_Client{

  /** Create a new SPARQL endpoint client
   *
   * If the query and update endpoints are the same, then you
   * only need to give a single URI.
   *
   * @param string $queryUri The address of the SPARQL Query Endpoint
   * @param string $updateUri Optional address of the SPARQL Update Endpoint
   */
  public function __construct($queryUri, $updateUri = null) {
    parent::__construct($queryUri, $updateUri);
  }

  /**
   * Get all the data for a specified URI for the Visual Mapper.
   *
   * @param string $root_uri
   *   The uri, which should be used as root.
   * @param string $lang
   *   The language of the selected concept.
   * @param boolean $broader_transitive
   *   If TRUE all the parent information for the root concept up to the concept
   *   schemes will be provided as a concept property name "parent_info".
   *
   * @return object
   *   The concept data as an object
   */
  public function getVisualMapperData($root_uri = NULL, $lang = 'en', $broader_transitive = FALSE) {
    // Create the root object
    $concept = $this->createRootUriObject($root_uri, $lang);

    switch ($concept->type) {
      case 'project':
        // Get all conceptSchemes.
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?n ?nLabel ?nn
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                ?n rdf:type skos:ConceptScheme .
                ?n skos:prefLabel|dc:title|dce:title|rdfs:label ?nLabel . FILTER(lang(?nLabel) = '$lang') .
                OPTIONAL {
                    ?n skos:hasTopConcept ?nn .
                }
            }";
        if ($children = $this->getRelationData($concept, $query, 'n')) {
          $concept->relations->children = $children;
        }
        break;

      case 'conceptScheme':
        // Get all topConcepts.
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?n ?nLabel ?nb ?nn ?nr
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                <$root_uri> skos:hasTopConcept ?n .
                ?n skos:prefLabel|dc:title|dce:title|rdfs:label ?nLabel . FILTER(lang(?nLabel) = '$lang') .
                ?nb skos:hasTopConcept ?n .
                OPTIONAL { ?n skos:narrower ?nn . }
                OPTIONAL { ?n skos:related ?nr . }
            }";
        if ($children = $this->getRelationData($concept, $query, 'n')) {
          $concept->relations->children = $children;
        }

        if ($broader_transitive) {
          // Get transitive parent information.
          $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?bTrans ?bTransLabel ?bTransN ?bTransNLabel ?bTransNN
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                ?bTrans rdf:type skos:ConceptScheme . 
                ?bTrans skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransLabel . FILTER(lang(?bTransLabel) = '$lang') .
                OPTIONAL { ?bTrans skos:hasTopConcept|skos:narrower ?bTransN . }
            }";

          $concept->parent_info = $this->getParentsInfo($query);
        }
        break;

      case 'topConcept':
        // Get all conceptSchemes.
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?b ?bLabel ?bn
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                ?b skos:hasTopConcept <$root_uri> .
                ?b skos:prefLabel|dc:title|dce:title|rdfs:label ?bLabel . FILTER(lang(?bLabel) = '$lang') .
                OPTIONAL { ?b skos:hasTopConcept ?bn . }
            }";
        if ($parents = $this->getRelationData($concept, $query, 'b')) {
          $concept->relations->parents = $parents;
        }
        // Get all narrower concepts
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?n ?nLabel ?nb ?nn ?nr
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                <$root_uri> skos:narrower ?n .
                ?n skos:prefLabel|dc:title|dce:title|rdfs:label ?nLabel . FILTER(lang(?nLabel) = '$lang') .
                ?n skos:broader ?nb.
                OPTIONAL { ?n skos:narrower ?nn . }
                OPTIONAL { ?n skos:related ?nr . }
            }";
        if ($children = $this->getRelationData($concept, $query, 'n')) {
          $concept->relations->children = $children;
        }
        // Get all related concepts
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?r ?rLabel ?rb ?rn ?rr
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                <$root_uri> skos:related ?r .
                ?r skos:prefLabel|dc:title|dce:title|rdfs:label ?rLabel . FILTER(lang(?rLabel) = '$lang') .
                { ?r skos:broader ?rb . } UNION { ?rb skos:hasTopConcept ?r }
                OPTIONAL { ?r skos:narrower ?rn . }
                OPTIONAL { ?r skos:related ?rr . }
            }";
        if ($related = $this->getRelationData($concept, $query, 'r')) {
          $concept->relations->related = $related;
        }

        if ($broader_transitive) {
          // Get transitive parent information.
          $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?bTrans ?bTransLabel ?bTransN ?bTransNLabel ?bTransNN
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                {
                    ?bTrans rdf:type skos:ConceptScheme . 
                    ?bTrans skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransLabel . FILTER(lang(?bTransLabel) = '$lang') .
                    OPTIONAL { ?bTrans skos:hasTopConcept|skos:narrower ?bTransN . }
                }
                UNION
                {
                    <$root_uri> (skos:topConceptOf)* ?bTrans .
                    ?bTrans skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransLabel . FILTER(lang(?bTransLabel) = '$lang') .
                    ?bTrans skos:hasTopConcept|skos:narrower ?bTransN .
                    FILTER NOT EXISTS {<$root_uri> skos:hasTopConcept|skos:narrower ?bTransN }
                    ?bTransN skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransNLabel . FILTER(lang(?bTransNLabel) = '$lang') .
                    OPTIONAL { ?bTransN skos:narrower ?bTransNN . }
                }
            }";

          $concept->parent_info = $this->getParentsInfo($query);
        }
        break;

      case 'concept':
        // Get all broader concepts.
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?b ?bLabel ?bb ?bn ?br
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                <$root_uri> skos:broader ?b .
                ?b skos:prefLabel|dc:title|dce:title|rdfs:label ?bLabel . FILTER(lang(?bLabel) = '$lang') .
                { ?b skos:broader ?bb . } UNION { ?bb skos:hasTopConcept ?b }
                ?b skos:narrower ?bn .
                OPTIONAL { ?b skos:related ?br . }
            }";
        if ($parents = $this->getRelationData($concept, $query, 'b')) {
          $concept->relations->parents = $parents;
        }
        // Get all narrower concepts.
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?n ?nLabel ?nb ?nn ?nr
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                <$root_uri> skos:narrower ?n.
                ?n skos:prefLabel|dc:title|dce:title|rdfs:label ?nLabel . FILTER(lang(?nLabel) = '$lang') .
                OPTIONAL { ?n skos:narrower ?nn . }
                ?n skos:broader ?nb.
                OPTIONAL { ?n skos:related ?nr . }
            }";
        if ($children = $this->getRelationData($concept, $query, 'n')) {
          $concept->relations->children = $children;
        }
        // Get all related concepts.
        $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?r ?rLabel ?rb ?rn ?rr
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                <$root_uri> skos:related ?r.
                ?r skos:prefLabel|dc:title|dce:title|rdfs:label ?rLabel . FILTER(lang(?rLabel) = '$lang') .
                { ?r skos:broader ?rb . } UNION { ?rb skos:hasTopConcept ?r }
                OPTIONAL { ?r skos:narrower ?rn . }
                OPTIONAL { ?r skos:related ?rr . }
            }";
        if ($related = $this->getRelationData($concept, $query, 'r')) {
          $concept->relations->related = $related;
        }

        if ($broader_transitive) {
          // Get transitive parent information.
          $query = "
            PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
            PREFIX dc:<http://purl.org/dc/terms/>
            PREFIX dce:<http://purl.org/dc/elements/1.1/>
            PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
            PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

            SELECT DISTINCT ?bTrans ?bTransLabel ?bTransN ?bTransNLabel ?bTransNN
            " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
            WHERE {
                {
                    ?bTrans rdf:type skos:ConceptScheme . 
                    ?bTrans skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransLabel . FILTER(lang(?bTransLabel) = '$lang') .
                    OPTIONAL { ?bTrans skos:hasTopConcept|skos:narrower ?bTransN . }
                }
                UNION
                {
                    <$root_uri> (skos:topConceptOf|skos:broader)* ?bTrans .
                    ?bTrans skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransLabel . FILTER(lang(?bTransLabel) = '$lang') .
                    ?bTrans skos:hasTopConcept|skos:narrower ?bTransN .
                    FILTER NOT EXISTS {<$root_uri> skos:hasTopConcept|skos:narrower ?bTransN }
                    ?bTransN skos:prefLabel|dc:title|dce:title|rdfs:label ?bTransNLabel . FILTER(lang(?bTransNLabel) = '$lang') .
                    OPTIONAL { ?bTransN skos:narrower ?bTransNN . }
                }
            }";

          $concept->parent_info = $this->getParentsInfo($query);
        }

        break;
    }

    return $concept;
  }

  /**
   * Creates a data object for the root concept with id, name, type and size.
   * Important for the Visual Mapper data.
   *
   * @param string $uri
   *  The concept URI.
   * @param string $lang
   *  The language for the concept data.
   *
   * @return object
   *  The root concept object.
   */
  public function createRootUriObject($uri, $lang) {
    $object = new stdClass();
    $object->id = $uri;
    $object->size = 1;
    $object->relations = new stdClass();

    if (is_null($uri)) {
      $object->name = '';
      $object->type = 'project';
      return $object;
    }

    // Get the label and the type of the given concept
    $query = "
        PREFIX skos:<http://www.w3.org/2004/02/skos/core#>
        PREFIX dc:<http://purl.org/dc/terms/>
        PREFIX dce:<http://purl.org/dc/elements/1.1/>
        PREFIX rdf:<http://www.w3.org/1999/02/22-rdf-syntax-ns#>
        PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#>

        SELECT ?label ?topConcept ?concept
        " . (!empty($this->graphUri) ? 'FROM <' . $this->graphUri . '>' : '') . "
        WHERE {
            <$uri> skos:prefLabel|dc:title|dce:title|rdfs:label ?label . FILTER(lang(?label) = '$lang') .
            OPTIONAL {
                <$uri> skos:broader ?concept .
            }
            OPTIONAL {
                ?topConcept skos:hasTopConcept <$uri> .
            }
        }";
    $rows = $this->query($query);
    $object->name = $rows[0]->label->getValue();
    $object->type = isset($rows[0]->concept) ? 'concept' : (isset($rows[0]->topConcept) ? 'topConcept' : 'conceptScheme');

    return $object;
  }

  /**
   * Get the data from the SPARQL endpoint for a given relation type (broader,
   * narrower or related).
   * Important for the Visual Mapper data.
   *
   * @param object $concept
   *    The root concept object..
   * @param string $query
   *    The query to get the data from SPARQL endpoint.
   * @param string $type
   *    The relation type:
   *      b => broader (parents),
   *      n => narrower (children),
   *      r => related (related)
   *
   * @return array
   *
   */
  protected function getRelationData(&$concept, $query, $type) {
    try {
      $rows = $this->query($query);
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred calling the query %query (%error).', array(
        '%query' => $query,
        '%error' => print_r($e->getMessage(), TRUE)
      )), 'error');
      exit();
    }

    $map = array('b' => 'parents', 'n' => 'children', 'r' => 'related');
    if (!isset($map[$type])) {
      return NULL;
    }

    $relations = array();
    foreach ($rows as $row) {
      if (isset($row->{$type})) {
        $uri = $row->{$type}->getUri();
        if (!isset($relations[$uri])) {
          $concept->size++;
          $relations[$uri] = new stdClass();
          $relations[$uri]->id = $uri;
          $relations[$uri]->name = $row->{$type . 'Label'}->getValue();
          $relations[$uri]->size = 1;
        }
        if (isset($row->{$type . 'b'})) {
          $broader_uri = $row->{$type . 'b'}->getUri();
          if (!isset($relations[$uri]->relations->parents[$broader_uri])) {
            $concept->size++;
            $relations[$uri]->size++;
            $relations[$uri]->relations->parents[$broader_uri] = new stdClass();
            $relations[$uri]->relations->parents[$broader_uri]->id = $broader_uri;
            $relations[$uri]->relations->parents[$broader_uri]->size = 1;
          }
        }
        if (isset($row->{$type . 'n'})) {
          $narrower_uri = $row->{$type . 'n'}->getUri();
          if (!isset($relations[$uri]->relations->children[$narrower_uri])) {
            $concept->size++;
            $relations[$uri]->size++;
            $relations[$uri]->relations->children[$narrower_uri] = new stdClass();
            $relations[$uri]->relations->children[$narrower_uri]->id = $narrower_uri;
            $relations[$uri]->relations->children[$narrower_uri]->size = 1;
          }
        }
        if (isset($row->{$type . 'r'})) {
          $related_uri = $row->{$type . 'r'}->getUri();
          if (!isset($relations[$uri]->relations->related[$related_uri])) {
            $concept->size++;
            $relations[$uri]->size++;
            $relations[$uri]->relations->related[$related_uri] = new stdClass();
            $relations[$uri]->relations->related[$related_uri]->id = $related_uri;
            $relations[$uri]->relations->related[$related_uri]->size = 1;
          }
        }
      }
    }

    if (empty($relations)) {
      return NULL;
    }

    foreach ($relations as &$relation) {
      if (isset($relation->relations->parents)) {
        $relation->relations->parents = array_values($relation->relations->parents);
      }
      if (isset($relation->relations->children)) {
        $relation->relations->children = array_values($relation->relations->children);
      }
      if (isset($relation->relations->related)) {
        $relation->relations->related = array_values($relation->relations->related);
      }
    }

    usort($relations, array($this, 'sortRelationsBySize'));
    $relations = array_values($relations);

    return $relations;
  }

  /**
   * Get transitive information about the parents of a concept.
   *
   * @param string $query
   *   The query to use at the SPARQL endpoint.
   *
   * @return array
   *   An array of broader concepts, starting at the concept scheme level.
   */
  protected function getParentsInfo($query) {
    try {
      $rows = $this->query($query);
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred calling the query %query (%error).', array(
        '%query' => $query,
        '%error' => print_r($e->getMessage(), TRUE)
      )), 'error');
      exit();
    }

    $parent_concepts = array();
    foreach ($rows as $row) {
      if (isset($row->bTrans)) {
        $uri = $row->bTrans->getUri();
        if (!isset($parent_concepts[$uri]) || !isset($parent_concepts[$uri]->relations)) {
          $parent_concepts[$uri] = new stdClass();
          $parent_concepts[$uri]->id = $uri;
          $parent_concepts[$uri]->name = $row->bTransLabel->getValue();
          $parent_concepts[$uri]->size = 1;
          $parent_concepts[$uri]->relations = new stdClass();
          $parent_concepts[$uri]->relations->_children = [];
          $parent_concepts[$uri]->relations->children = [];
        }

        if (isset($row->bTransN)) {
          $child_uri = $row->bTransN->getUri();
          if (!in_array($child_uri, $parent_concepts[$uri]->relations->_children)) {
            $parent_concepts[$uri]->relations->_children[] = $child_uri;
          }

          if (!isset($parent_concepts[$child_uri]) || !isset($parent_concepts[$child_uri]->relations)) {
            $parent_concepts[$child_uri] = new stdClass();
            $parent_concepts[$child_uri]->id = $child_uri;
            $parent_concepts[$child_uri]->size = 1;
            if (isset($row->bTransNLabel)) {
              $parent_concepts[$child_uri]->name = $row->bTransNLabel->getValue();
              $parent_concepts[$child_uri]->relations = new stdClass();
              $parent_concepts[$child_uri]->relations->children = [];
              $parent_concepts[$child_uri]->relations->_children = [];
            }
            $parent_concepts[$uri]->size++;
          }

          if (isset($row->bTransNN)) {
            $blank_child_uri = $row->bTransNN->getUri();
            if (isset($parent_concepts[$child_uri]->relations) && !in_array($blank_child_uri, $parent_concepts[$child_uri]->relations->_children)) {
              $parent_concepts[$child_uri]->relations->_children[] = $blank_child_uri;
            }

            if (!isset($parent_concepts[$blank_child_uri])) {
              $parent_concepts[$uri]->size++;
              $parent_concepts[$child_uri]->size++;
              $parent_concepts[$blank_child_uri] = new stdClass();
              $parent_concepts[$blank_child_uri]->id = $blank_child_uri;
              $parent_concepts[$blank_child_uri]->size = 1;
            }
          }
        }
      }
    }

    $handled_uris = [];
    foreach ($parent_concepts as $uri => &$parent_concept) {
      $this->buildParentsRecursive($parent_concepts, $parent_concept, $handled_uris);
    }

    return array_values(array_diff_key($parent_concepts, array_flip($handled_uris)));
  }

  /**
   * Builds the parents array recursively.
   *
   * @param array $all_concepts
   *   An associative array of all fetched concepts keyed by URI.
   * @param \stdClass $concept
   *   The current concept to build the data for.
   * @param array $handled_uris
   *   An array of already used URIs
   */
  protected function buildParentsRecursive($all_concepts, &$concept, &$handled_uris) {
    if (!in_array($concept->id, $handled_uris) && isset($concept->relations)) {
      foreach ($concept->relations->_children as $child_uri) {
        if (isset($all_concepts[$child_uri]) && !in_array($child_uri, $handled_uris)) {
          $this->buildParentsRecursive($all_concepts, $all_concepts[$child_uri], $handled_uris);
          $concept->relations->children[] = $all_concepts[$child_uri];
          $handled_uris[] = $child_uri;
        }
      }
      unset($concept->relations->_children);
    }
  }

  /**
   * Callback function to sort the concepts by size.
   *
   * @param object $a
   *    First concept to compare.
   * @param object $b
   *    Second concept to compare.
   *
   * @return boolean
   */
  protected function sortRelationsBySize($a, $b) {
    return $a->size >= $b->size ? ($a->size == $b->size ? 0 : -1) : 1;
  }
}