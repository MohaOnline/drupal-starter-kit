<?php

namespace Drupal\openlayers\ServiceContainer\ServiceProvider;

use OpenlayersDrupal\Component\FileCache\FileCacheFactory;
use OpenlayersDrupal\Core\DependencyInjection\ContainerBuilder;
use OpenlayersDrupal\Core\DependencyInjection\YamlFileLoader;
use OpenlayersDrupal\Core\DependencyInjection\Dumper\PhpArrayDumper;
use Drupal\openlayers\DependencyInjection\ServiceProviderInterface;

/**
 * Provides render cache service definitions.
 *
 * @codeCoverageIgnore
 */
class ServiceContainerSymfonyServiceProvider implements ServiceProviderInterface {

  /**
   * {@inheritdoc}
   */
  public function getContainerDefinition() {
    FileCacheFactory::setConfiguration(
      array(
        'default' => array(
          'class' => '\OpenlayersDrupal\Component\FileCache\NullFileCache',
        ),
      )
    );
    $container_builder = new ContainerBuilder();
    $yaml_loader = new YamlFileLoader($container_builder);

    foreach (module_list() as $module) {
      $filename = drupal_get_filename('module', $module);
      $services = dirname($filename) . "/$module.services.yml";
      if (file_exists($services)) {
        $yaml_loader->load($services);
      }
    }

    // Disabled for now.
    // $container_builder->compile();
    $dumper = new PhpArrayDumper($container_builder);
    return $dumper->getArray();
  }

  /**
   * {@inheritdoc}
   */
  public function alterContainerDefinition(array &$container_definition) {}

}
