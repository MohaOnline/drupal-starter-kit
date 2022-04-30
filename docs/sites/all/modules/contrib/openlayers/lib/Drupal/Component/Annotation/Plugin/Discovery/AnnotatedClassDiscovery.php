<?php

namespace OpenlayersDrupal\Component\Annotation\Plugin\Discovery;

use OpenlayersDrupal\Component\Annotation\AnnotationInterface;
use OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface;
use OpenlayersDrupal\Component\Annotation\Reflection\MockFileFinder;
use OpenlayersDoctrine\Common\Annotations\SimpleAnnotationReader;
use OpenlayersDoctrine\Common\Annotations\AnnotationRegistry;
use OpenlayersDoctrine\Common\Reflection\StaticReflectionParser;
use OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryTrait;

/**
 * Defines a discovery mechanism to find annotated plugins in PSR-0 namespaces.
 */
class AnnotatedClassDiscovery implements DiscoveryInterface {

  use DiscoveryTrait;

  /**
   * The namespaces within which to find plugin classes.
   *
   * @var string[]
   */
  protected $pluginNamespaces;

  /**
   * The name of the annotation that contains the plugin definition.
   *
   * The class corresponding to this name must implement
   * \OpenlayersDrupal\Component\Annotation\AnnotationInterface.
   *
   * @var string
   */
  protected $pluginDefinitionAnnotationName;

  /**
   * The doctrine annotation reader.
   *
   * @var \OpenlayersDoctrine\Common\Annotations\Reader
   */
  protected $annotationReader;

  /**
   * Constructs a new instance.
   *
   * @param array $plugin_namespaces
   *   (optional) An array of namespace that may contain plugin
   *   implementations.
   *   Defaults to an empty array.
   * @param string $plugin_definition_annotation_name
   *   (optional) The name of the annotation that contains the plugin
   *   definition.
   *   Defaults to 'OpenlayersDrupal\Component\Annotation\Plugin'.
   */
  public function __construct(array $plugin_namespaces = array(), $plugin_definition_annotation_name = 'OpenlayersDrupal\Component\Annotation\Plugin') {
    $this->pluginNamespaces = $plugin_namespaces;
    $this->pluginDefinitionAnnotationName = $plugin_definition_annotation_name;
  }

  /**
   * Gets the used doctrine annotation reader.
   *
   * @return \OpenlayersDoctrine\Common\Annotations\Reader
   *   The annotation reader.
   */
  protected function getAnnotationReader() {
    if (!isset($this->annotationReader)) {
      $this->annotationReader = new SimpleAnnotationReader();

      // Add the namespaces from the main plugin annotation, like @EntityType.
      $namespace = substr($this->pluginDefinitionAnnotationName, 0, strrpos($this->pluginDefinitionAnnotationName, '\\'));
      $this->annotationReader->addNamespace($namespace);
    }
    return $this->annotationReader;
  }

  /**
   * {@inheritdoc}
   */
  public function getDefinitions() {
    $definitions = array();

    $reader = $this->getAnnotationReader();

    // Clear the annotation loaders of any previous annotation classes.
    AnnotationRegistry::reset();
    // Register the namespaces of classes that can be used for annotations.
    AnnotationRegistry::registerLoader('class_exists');

    // Search for classes within all PSR-0 namespace locations.
    foreach ($this->getPluginNamespaces() as $namespace => $dirs) {
      foreach ($dirs as $dir) {
        if (file_exists($dir)) {
          $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($dir, \RecursiveDirectoryIterator::SKIP_DOTS)
          );
          foreach ($iterator as $fileinfo) {
            if ($fileinfo->getExtension() == 'php') {
              $sub_path = $iterator->getSubIterator()->getSubPath();
              $sub_path = $sub_path ? str_replace(DIRECTORY_SEPARATOR, '\\', $sub_path) . '\\' : '';
              $class = $namespace . '\\' . $sub_path . $fileinfo->getBasename('.php');

              // The filename is already known, so there is no need to find the
              // file. However, StaticReflectionParser needs a finder, so use a
              // mock version.
              $finder = MockFileFinder::create($fileinfo->getPathName());
              $parser = new StaticReflectionParser($class, $finder, TRUE);

              /** @var \OpenlayersDrupal\Component\Annotation\AnnotationInterface */
              if ($annotation = $reader->getClassAnnotation($parser->getReflectionClass(), $this->pluginDefinitionAnnotationName)) {
                $this->prepareAnnotationDefinition($annotation, $class);
                $definitions[$annotation->getId()] = $annotation->get();
              }
            }
          }
        }
      }
    }

    // Don't let annotation loaders pile up.
    AnnotationRegistry::reset();

    return $definitions;
  }

  /**
   * Prepares the annotation definition.
   *
   * @param \OpenlayersDrupal\Component\Annotation\AnnotationInterface $annotation
   *   The annotation derived from the plugin.
   * @param string $class
   *   The class used for the plugin.
   */
  protected function prepareAnnotationDefinition(AnnotationInterface $annotation, $class) {
    $annotation->setClass($class);
  }

  /**
   * Gets an array of PSR-0 namespaces to search for plugin classes.
   *
   * @return array
   *   FIX - insert comment here.
   */
  protected function getPluginNamespaces() {
    return $this->pluginNamespaces;
  }

}
