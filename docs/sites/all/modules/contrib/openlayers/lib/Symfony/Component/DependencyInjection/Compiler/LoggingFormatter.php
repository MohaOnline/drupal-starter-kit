<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

/**
 * Used to format logging messages during the compilation.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class LoggingFormatter {

  /**
   * FIX - insert comment here.
   */
  public function formatRemoveService(CompilerPassInterface $pass, $id, $reason) {
    return $this->format($pass, sprintf('Removed service "%s"; reason: %s', $id, $reason));
  }

  /**
   * FIX - insert comment here.
   */
  public function formatInlineService(CompilerPassInterface $pass, $id, $target) {
    return $this->format($pass, sprintf('Inlined service "%s" to "%s".', $id, $target));
  }

  /**
   * FIX - insert comment here.
   */
  public function formatUpdateReference(CompilerPassInterface $pass, $serviceId, $oldDestId, $newDestId) {
    return $this->format($pass, sprintf('Changed reference of service "%s" previously pointing to "%s" to "%s".', $serviceId, $oldDestId, $newDestId));
  }

  /**
   * FIX - insert comment here.
   */
  public function formatResolveInheritance(CompilerPassInterface $pass, $childId, $parentId) {
    return $this->format($pass, sprintf('Resolving inheritance for "%s" (parent: %s).', $childId, $parentId));
  }

  /**
   * FIX - insert comment here.
   */
  public function format(CompilerPassInterface $pass, $message) {
    return sprintf('%s: %s', get_class($pass), $message);
  }

}
