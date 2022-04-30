<?php

namespace OpenlayersDrupal\Core\Block\Annotation;

use OpenlayersDrupal\Component\Annotation\Plugin;

/**
 * Defines a Block annotation object.
 *
 * @ingroup block_api
 *
 * @Annotation
 */
class Block extends Plugin {

  /**
   * The plugin ID.
   *
   * @var string
   */
  public $id;

  /**
   * The administrative label of the block.
   *
   * @var \OpenlayersDrupal\Core\Annotation\Translation
   *
   * @ingroup plugin_translatable
   */
  public $admin_label = '';

  /**
   * The category in the admin UI where the block will be listed.
   *
   * @var \OpenlayersDrupal\Core\Annotation\Translation
   *
   * @ingroup plugin_translatable
   */
  public $category = '';

  /**
   * Class used to retrieve derivative definitions of the block.
   *
   * @var string
   */
  public $derivative = '';

}
