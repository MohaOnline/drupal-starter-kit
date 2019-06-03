<?php

namespace Drupal\campaignion_newsletters;

/**
 * Partial mock class for CronRunner.
 *
 * Mocking generators is not yet supported in PHPUnit.
 */
class _MockProviderCronRunner extends CronRunner {

  public $providers = [];


  protected function getProviders() {
    foreach ($this->providers as $p) {
      yield $p;
    }
  }

}


class CronRunnerTest extends \DrupalUnitTestCase {

  /**
   * Polling can handle having no providers.
   */
  public function testPollWithoutProviders() {
    $c = new _MockProviderCronRunner(50, 10);
    $start = microtime(TRUE);
    $c->poll();
    $this->assertLessThan(1, microtime(TRUE) - $start);
  }

  /**
   * Polling can handle providers without polling.
   */
  public function testPollWithNonPollingProvider() {
    $c = new _MockProviderCronRunner(50, 10);
    $c->providers = [$this->createMock(ProviderInterface::class)];
    $start = microtime(TRUE);
    $c->poll();
    $this->assertLessThan(1, microtime(TRUE) - $start);
  }

  /**
   * Polling stops early if there are no more batches.
   */
  public function testOneProviderOneBatch() {
    $c = new _MockProviderCronRunner(50, 10);
    $provider = $this->createMock(ProviderInterface::class);
    $polling = $this->createMock(PollingInterface::class);
    $provider->method('polling')->willReturn($polling);
    $polling->expects($this->once())->method('poll')->willReturn(FALSE);
    $c->providers = [$provider];
    $c->poll();
  }

  /**
   * Polling stops when time is up (even if there are more batches).
   */
  public function testPollTimeOut() {
    $c = new _MockProviderCronRunner(50, 0.0001);
    $provider = $this->createMock(ProviderInterface::class);
    $polling = $this->createMock(PollingInterface::class);
    $provider->method('polling')->willReturn($polling);
    $polling->expects($this->once())->method('poll')->will($this->returnCallback(function () {
      usleep(101);
      return TRUE;
    }));
    $c->providers = [$provider];
    $start = microtime(TRUE);
    $c->poll();
    $this->assertLessThan(1, microtime(TRUE) - $start);
  }

}

