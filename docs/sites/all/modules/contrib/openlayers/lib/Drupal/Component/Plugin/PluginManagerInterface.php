<?php

namespace OpenlayersDrupal\Component\Plugin;

use OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface;
use OpenlayersDrupal\Component\Plugin\Factory\FactoryInterface;
use OpenlayersDrupal\Component\Plugin\Mapper\MapperInterface;

/**
 * Interface implemented by plugin managers.
 *
 * There are no explicit methods on the manager interface. Instead plugin
 * managers broker the interactions of the different plugin components, and
 * therefore, must implement each component interface, which is enforced by
 * this interface extending all of the component ones.
 *
 * While a plugin manager may directly implement these interface methods with
 * custom logic, it is expected to be more common for plugin managers to proxy
 * the method invocations to the respective components, and directly implement
 * only the additional functionality needed by the specific pluggable system.
 * To follow this pattern, plugin managers can extend from the PluginManagerBase
 * class, which contains the proxying logic.
 *
 * @see \OpenlayersDrupal\Component\Plugin\PluginManagerBase
 *
 * @ingroup plugin_api
 */
interface PluginManagerInterface extends DiscoveryInterface, FactoryInterface, MapperInterface {

}
