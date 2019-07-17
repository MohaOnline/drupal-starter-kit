<?php

/**
 * Field handler to render audio_format
 */
class getid3_metadata_audio_format extends getid3_metadata_handler {
  function render($values) {
    $rendered_values = parent::render($values);
    return $this->sanitize_value($rendered_values['audio_format']);
  }
}
