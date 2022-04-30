<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

use OpenlayersSymfony\Component\ExpressionLanguage\ExpressionFunction;
use OpenlayersSymfony\Component\ExpressionLanguage\ExpressionFunctionProviderInterface;

/**
 * Define some ExpressionLanguage functions.
 *
 * To get a service, use service('request').
 * To get a parameter, use parameter('kernel.debug').
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class ExpressionLanguageProvider implements ExpressionFunctionProviderInterface {

  /**
   * FIX - insert comment here.
   */
  public function getFunctions() {
    return array(
      new ExpressionFunction('service', function ($arg) {
              return sprintf('$this->get(%s)', $arg);
      }, function (array $variables, $value) {
        return $variables['container']->get($value);
      }),

      new ExpressionFunction('parameter', function ($arg) {
                return sprintf('$this->getParameter(%s)', $arg);
      }, function (array $variables, $value) {
          return $variables['container']->getParameter($value);
      }),
    );
  }

}
