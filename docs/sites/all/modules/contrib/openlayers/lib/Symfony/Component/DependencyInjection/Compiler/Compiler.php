<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * This class is used to remove circular dependencies between individual passes.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class Compiler {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $passConfig;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $log = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $loggingFormatter;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $serviceReferenceGraph;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->passConfig = new PassConfig();
    $this->serviceReferenceGraph = new ServiceReferenceGraph();
    $this->loggingFormatter = new LoggingFormatter();
  }

  /**
   * Returns the PassConfig.
   *
   * @return PassConfig
   *   The PassConfig instance.
   */
  public function getPassConfig() {
    return $this->passConfig;
  }

  /**
   * Returns the ServiceReferenceGraph.
   *
   * @return ServiceReferenceGraph
   *   The ServiceReferenceGraph instance.
   */
  public function getServiceReferenceGraph() {
    return $this->serviceReferenceGraph;
  }

  /**
   * Returns the logging formatter which can be used by compilation passes.
   *
   * @return LoggingFormatter
   *   FIX - insert comment here.
   */
  public function getLoggingFormatter() {
    return $this->loggingFormatter;
  }

  /**
   * Adds a pass to the PassConfig.
   *
   * @param CompilerPassInterface $pass
   *   A compiler pass.
   * @param string $type
   *   The type of the pass.
   */
  public function addPass(CompilerPassInterface $pass, $type = PassConfig::TYPE_BEFORE_OPTIMIZATION) {
    $this->passConfig->addPass($pass, $type);
  }

  /**
   * Adds a log message.
   *
   * @param string $string
   *   The log message.
   */
  public function addLogMessage($string) {
    $this->log[] = $string;
  }

  /**
   * Returns the log.
   *
   * @return array
   *   Log array.
   */
  public function getLog() {
    return $this->log;
  }

  /**
   * Run the Compiler and process all Passes.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function compile(ContainerBuilder $container) {
    foreach ($this->passConfig->getPasses() as $pass) {
      $pass->process($container);
    }
  }

}
