<?php
/**
 * @file
 * Interface for Bean Slide plugins.
 */

interface WetKitBeanSlidePluginInterface {

  /**
   * Attachments for Slide.
   */
  public function attachments();

  /**
   * The Form Settings to be applied.
   */
  public function form($settings);

  /**
   * The values for the slide.
   */
  public function values();

  /**
   * The submit handler.
   */
  public function submit(&$settings);
}

class WetKitBeanSlideNullPlugin {

  /**
   * Implements WetKitBeanSlidePluginInterface::attachments().
   */
  public function attachments() {}

  /**
   * Implements WetKitBeanSlidePluginInterface::form().
   */
  public function form($settings) {
    return array();
  }

  /**
   * Implements WetKitBeanSlidePluginInterface::values().
   */
  public function values() {
    return array();
  }
}
