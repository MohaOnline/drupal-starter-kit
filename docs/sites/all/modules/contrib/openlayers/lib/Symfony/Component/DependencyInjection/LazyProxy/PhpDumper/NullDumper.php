<?php

namespace OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper;

use OpenlayersSymfony\Component\DependencyInjection\Definition;

/**
 * FIX - insert comment here.
 *
 * Null dumper, negates any proxy code generation for any given service
 * definition.
 *
 * @author Marco Pivetta <ocramius@gmail.com>
 */
class NullDumper implements DumperInterface {

  /**
   * {@inheritdoc}
   */
  public function isProxyCandidate(Definition $definition) {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getProxyFactoryCode(Definition $definition, $id) {
    return '';
  }

  /**
   * {@inheritdoc}
   */
  public function getProxyCode(Definition $definition) {
    return '';
  }

}
