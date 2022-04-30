<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

use OpenlayersSymfony\Component\ExpressionLanguage\ExpressionLanguage as BaseExpressionLanguage;
use OpenlayersSymfony\Component\ExpressionLanguage\ParserCache\ParserCacheInterface;

/**
 * Adds some function to the default ExpressionLanguage.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @see ExpressionLanguageProvider
 */
class ExpressionLanguage extends BaseExpressionLanguage {

  /**
   * FIX - insert comment here.
   */
  public function __construct(ParserCacheInterface $cache = NULL, array $providers = array()) {
    // Prepend the default provider to let users override it easily.
    array_unshift($providers, new ExpressionLanguageProvider());

    parent::__construct($cache, $providers);
  }

}
