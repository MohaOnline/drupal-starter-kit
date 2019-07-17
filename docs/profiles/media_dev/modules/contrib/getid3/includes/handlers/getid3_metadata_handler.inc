<?php

/**
 * Field handler to render metadata attributes
 *
 * @todo: Patch file entity to provide this?
 */
class getid3_metadata_handler extends views_handler_field {

  function render($values) {
    $fid = $this->get_value($values);
    $file = file_load($fid);
    return $file->metadata;
  }

}
