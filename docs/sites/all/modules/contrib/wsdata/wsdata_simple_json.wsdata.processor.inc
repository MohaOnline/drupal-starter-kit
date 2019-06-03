<?php

class wsdata_simple_json_processor extends WsData {

  // Parse the web service response string, and returns a structured data array
  public function parse($data) {
    if (!isset($data) || empty($data)) {
      return;
    }

    // Remove UTF-8 BOM if present, json_decode() does not like it.
    if(substr($data, 0, 3) == pack("CCC", 0xEF, 0xBB, 0xBF)) {
      $data = substr($data, 3);
    }

    $data = trim($data);
    return json_decode($data, TRUE);
  }

   function accepts() {
    return array('json');
  }
}
