<?php

interface DatexInterface {

  function xFormat($format);

  function format($format);

  function formatArray();

  function xFormatArray();


  //-----------------------------------

  function xSetDate($y, $m, $d);

  function setDateLocale($y, $m, $d);

  function setTime($hour, $minute, $second);

  function parse($value, $format);

  //-----------------------------------

  function setTimestamp($timestamp);

  function getTimestamp();

  //-----------------------------------

  function getLangcode();

  function getCalendarName();

  function listOptions($name, $required);

  //-----------------------------------

  function copy();

  function validate(array $arr);

}

