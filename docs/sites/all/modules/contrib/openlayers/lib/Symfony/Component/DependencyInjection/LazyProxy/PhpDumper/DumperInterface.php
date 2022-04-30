<?php

namespace OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper;

use OpenlayersSymfony\Component\DependencyInjection\Definition;

/**
 * FIX - insert comment here.
 *
 * Lazy proxy dumper capable of generating the instantiation logic PHP code
 * for proxied services.
 *
 * @author Marco Pivetta <ocramius@gmail.com>
 */
interface DumperInterface {

  /**
   * FIX - insert comment here.
   *
   * Inspects whether the given definitions should produce proxy instantiation
   * logic in the dumped container.
   *
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isProxyCandidate(Definition $definition);

  /**
   * FIX - insert comment here.
   *
   * Generates the code to be used to instantiate a proxy in the dumped
   * factory code.
   *
   * @param \Definition $definition
   *   FIX - insert comment here.
   * @param string $id
   *   Service identifier.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getProxyFactoryCode(Definition $definition, $id);

  /**
   * Generates the code for the lazy proxy.
   *
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getProxyCode(Definition $definition);

}
