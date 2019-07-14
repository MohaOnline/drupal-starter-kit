<?php

/**
 * @file
 * Fallback calendar implementation in case php-intl is not avaiable.
 */

/**
 * Jalali calendar for datex.
 *
 * 4 Years later: apparently Amin has deprecated it. It's not on his github
 * page. Don't go bother him with issues about it.
 *
 * Original conversion algorithm by: Amin Saeedi.
 * Forked from project: Shamsi-Date v4.0 (GPL).
 * On github: http://github.com/amsa
 * mail: amin.w3dev@gmail.com
 */
final class DatexPoorMansJaliliCalendar extends DatexPartialImplementation implements DatexInterface {

  /**
   * Default constructor.
   */
  public function __construct($tz, $lang_code) {
    $lang_code = $lang_code !== 'fa' && $lang_code !== 'en' ? 'fa' : $lang_code;
    parent::__construct($tz, 'persian', $lang_code);
  }

  public function format($format) {
    $format = str_replace('M', 'F', $format);
    return self::_format($format, parent::getOrigin(), $this->getLangcode() === 'fa');
  }

  public function setDateLocale($y = 1, $m = 1, $d = 1) {
    list($y, $m, $d) = self::toGregorian($y, $m, $d);
    $this->xSetDate($y, $m, $d);
    return $this;
  }

  /**
   * Creates a clone of this object.
   */
  public function copy() {
    return new DatexPoorMansJaliliCalendar($this->timezone, $this->langCode);
  }

  public function validate(array $arr) {
    if ((!isset($arr['year']) || empty($arr['year'])) &&
      (!isset($arr['month']) || empty($arr['month'])) &&
      (!isset($arr['day']) || empty($arr['day']))) {
      return NULL;
    }
    return self::validate_($arr['year'], $arr['month'], $arr['day']);
  }

  private static function validate_($year, $month, $day) {
    $zero = TRUE;
    $year = intval($year);
    $month = intval($month);
    $day = intval($day);
    if ($year < 0 || $year === 0 && $zero) {
      return t('Year out of range');
    }
    if ($month < 0 || 12 < $month || $month === 0 && $zero) {
      return t('Month out of range');
    }
    if ($day === 0 && $zero || $day < 0 || 31 < $day || $month > 6 && $day > 30 || $month === 12 && $day > 29) {
      return t('Day out of range');
    }
    return FALSE;
  }

  /**
   * Is supposed to parse a date string into date value.
   *
   * by morilog.
   */
  public function parse($date, $format) {
    // reverse engineer date formats
    $keys = [
      'Y' => ['year', '\d{4}'],
      'y' => ['year', '\d{2}'],
      'm' => ['month', '\d{2}'],
      'n' => ['month', '\d{1,2}'],
      'M' => ['month', '[A-Z][a-z]{3}'],
      'F' => ['month', '[A-Z][a-z]{2,8}'],
      'd' => ['day', '\d{2}'],
      'j' => ['day', '\d{1,2}'],
      'D' => ['day', '[A-Z][a-z]{2}'],
      'l' => ['day', '[A-Z][a-z]{6,9}'],
      'u' => ['hour', '\d{1,6}'],
      'h' => ['hour', '\d{2}'],
      'H' => ['hour', '\d{2}'],
      'g' => ['hour', '\d{1,2}'],
      'G' => ['hour', '\d{1,2}'],
      'i' => ['minute', '\d{2}'],
      's' => ['second', '\d{2}'],
    ];
    // convert format string to regex
    $regex = '';
    $chars = str_split($format);
    foreach ($chars as $n => $char) {
      $lastChar = isset($chars[$n - 1]) ? $chars[$n - 1] : '';
      $skipCurrent = '\\' == $lastChar;
      if (!$skipCurrent && isset($keys[$char])) {
        $regex .= '(?P<' . $keys[$char][0] . '>' . $keys[$char][1] . ')';
      }
      else {
        if ('\\' == $char) {
          $regex .= $char;
        }
        else {
          $regex .= preg_quote($char);
        }
      }
    }
    $dt = [];

    if (preg_match('#^' . $regex . '$#', $date, $dt)) {
      foreach ($dt as $k => $v) {
        if (is_int($k)) {
          unset($dt[$k]);
        }
      }
      if (!self::validate_($dt['month'], $dt['day'], $dt['year'])) {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }

    if (strlen($dt['year']) == 2) {
      $now = new DatexPoorMansJaliliCalendar($this->timezone, 'en');
      $x = $now->format('Y') - $now->format('y');
      $dt['year'] += $x;
    }
    $dt['year'] = isset($dt['year']) ? (int) $dt['year'] : $this->format('Y');
    $dt['month'] = isset($dt['month']) ? (int) $dt['month'] : $this->format('n');
    $dt['day'] = isset($dt['day']) ? (int) $dt['day'] : $this->format('j');
    $dt['hour'] = isset($dt['hour']) ? (int) $dt['hour'] : $this->format('G');
    $dt['minute'] = isset($dt['minute']) ? (int) $dt['minute'] : $this->format('i');
    $dt['second'] = isset($dt['second']) ? (int) $dt['second'] : $this->format('s');
    $this->setDateLocale($dt['year'], $dt['month'], $dt['day']);
    $this->setTime($dt['hour'], $dt['minute'], $dt['second']);
    return TRUE;
  }

  function getBaseYear() {
    return 1390;
  }



  // ___________________________________________________________________________

  private static function filterArray($needle, $heystack, $always = []) {
    return array_intersect(array_merge($needle, $always), $heystack);
  }

  private static function div($a, $b) {
    return (int) ($a / $b);
  }

  private static function substr($str, $start, $len) {
    if (function_exists('mb_substr')) {
      return mb_substr($str, $start, $len, 'UTF-8');
    }
    else {
      return substr($str, $start, $len * 2);
    }
  }


  private static $days_conv = [
    'sat' => [1, 'شنبه'],
    'sun' => [2, 'یکشنبه'],
    'mon' => [3, 'دوشنبه'],
    'tue' => [4, 'سه شنبه'],
    'wed' => [5, 'چهارشنبه'],
    'thu' => [6, 'پنجشنبه'],
    'fri' => [7, 'جمعه'],
  ];

  private static $days = [
    'sat' => [1, 'saturday'],
    'sun' => [2, 'sunday'],
    'mon' => [3, 'monday'],
    'tue' => [4, 'tuesday'],
    'wed' => [5, 'wednesday'],
    'thu' => [6, 'thursday'],
    'fri' => [7, 'friday'],
  ];

  private static $months_conv = [
    'فروردین',
    'اردیبهشت',
    'خرداد',
    'تیر',
    'مرداد',
    'شهریور',
    'مهر',
    'آبان',
    'آذر',
    'دی',
    'بهمن',
    'اسفند',
  ];

  private static $months = [
    'farvardin',
    'ordibehesht',
    'khordad',
    'tir',
    'mordad',
    'shahrivar',
    'mehr',
    'aban',
    'azar',
    'dey',
    'bahman',
    'esfand',
  ];

  private static $order = [
    'Yekom',
    'Dovom',
    'Sevom',
    'Cheharom',
    'Panjom',
    'Sheshom',
    'Haftom',
    'Hashtom',
    'Nohom',
    'Dahom',
    'Yazdahom',
    'Davazdahom',
    'Sizdahom',
    'Chehardahom',
    'Panzdahom',
    'Shanzdahom',
    'Hefdahom',
    'Hejdahom',
    'Noozdahom',
    'Bistom',
    'Bisto yekom',
    'Bisto dovom',
    'Bisto sevom',
    'Bisto cheharom',
    'Bisto panjom',
    'Bisto sheshom',
    'Bisto haftom',
    'Bisto hashtom',
    'Bisto nohom',
    'Siom',
    'Sio yekom',
    'Sio dovom',
  ];

  private static $order_conv = [
    'یکم',
    'دوم',
    'سوم',
    'چهارم',
    'پنجم',
    'ششم',
    'هفتم',
    'هشتم',
    'نهم',
    'دهم',
    'یازدهم',
    'دوازده‌ام',
    'سیزده‌ام',
    'چهارده‌ام',
    'پانزده‌ام',
    'شانزده‌ام',
    'هفده‌ام',
    'هجده‌ام',
    'نوزده‌ام',
    'بیست‌ام',
    'بیست‌ویکم',
    'بیست‌ودوم',
    'بیست‌وسوم',
    'بیست‌وچهارم',
    'بیست‌وپنجم',
    'بیست‌وششم',
    'بیست‌وهفتم',
    'بیست‌وهشتم',
    'بیست‌ونهم',
    'سی‌ام',
    'سی‌ویکم',
    'سی‌ودوم',
  ];

  private static $ampm = [
    'am' => 'ghabl az zohr',
    'pm' => 'bad az zohr',
  ];

  private static $ampm_convert = [
    'am' => 'قبل از ظهر',
    'pm' => 'بعد از ظهر',
  ];

  private static $ampms = [
    'am' => 'gh.z',
    'pm' => 'b.z',
  ];

  private static $ampms_convert = [
    'am' => 'ق.ظ',
    'pm' => 'ب.ظ',
  ];


  private static function getDayNames($convert, $day, $shorten = FALSE, $len = 1, $numeric = FALSE) {
    $day = substr(strtolower($day), 0, 3);
    $day = $convert ? self::$days_conv[$day] : self::$days[$day];
    return ($numeric) ? $day[0] : (($shorten) ? self::substr($day[1], 0, $len) : $day[1]);
  }

  private static function getMonthNames($convert, $month, $shorten = FALSE, $len = 3) {
    $ret = $convert ? self::$months_conv[$month - 1] : self::$months[$month - 1];
    return ($shorten) ? self::substr($ret, 0, $len) : $ret;
  }

  /**
   * Converts gregorian date to jalali date.
   *
   * by Sallar Kaboli
   */
  private static function toJalali($g_y, $g_m, $g_d) {
    $g_days_in_month = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    $j_days_in_month = [31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29];
    $gy = $g_y - 1600;
    $gm = $g_m - 1;
    $gd = $g_d - 1;
    $g_day_no = 365 * $gy + self::div($gy + 3, 4) - self::div($gy + 99, 100) + self::div($gy + 399, 400);
    for ($i = 0; $i < $gm; ++$i) {
      $g_day_no += $g_days_in_month[$i];
    }
    if ($gm > 1 && (($gy % 4 == 0 && $gy % 100 != 0) || ($gy % 400 == 0))) {
      $g_day_no++;
    }
    $g_day_no += $gd;
    $j_day_no = $g_day_no - 79;
    $j_np = self::div($j_day_no, 12053);
    $j_day_no = $j_day_no % 12053;
    $jy = 979 + 33 * $j_np + 4 * self::div($j_day_no, 1461);
    $j_day_no %= 1461;
    if ($j_day_no >= 366) {
      $jy += self::div($j_day_no - 1, 365);
      $j_day_no = ($j_day_no - 1) % 365;
    }
    for ($i = 0; $i < 11 && $j_day_no >= $j_days_in_month[$i]; ++$i) {
      $j_day_no -= $j_days_in_month[$i];
    }
    $jm = $i + 1;
    $jd = $j_day_no + 1;
    return [$jy, $jm, $jd];
  }

  /**
   * Converts Jalali date to gregorian date.
   *
   * by Sallar Kaboli
   */
  private static function toGregorian($j_y, $j_m, $j_d) {
    $g_days_in_month = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    $j_days_in_month = [31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29];
    $jy = $j_y - 979;
    $jm = $j_m - 1;
    $jd = $j_d - 1;
    $j_day_no = 365 * $jy + self::div($jy, 33) * 8 + self::div($jy % 33 + 3, 4);
    for ($i = 0; $i < $jm; ++$i) {
      $j_day_no += $j_days_in_month[$i];
    }
    $j_day_no += $jd;
    $g_day_no = $j_day_no + 79;
    $gy = 1600 + 400 * self::div($g_day_no, 146097);
    $g_day_no = $g_day_no % 146097;
    $leap = TRUE;
    if ($g_day_no >= 36525) {
      $g_day_no--;
      $gy += 100 * self::div($g_day_no, 36524);
      $g_day_no = $g_day_no % 36524;
      if ($g_day_no >= 365) {
        $g_day_no++;
      }
      else {
        $leap = FALSE;
      }
    }
    $gy += 4 * self::div($g_day_no, 1461);
    $g_day_no %= 1461;
    if ($g_day_no >= 366) {
      $leap = FALSE;
      $g_day_no--;
      $gy += self::div($g_day_no, 365);
      $g_day_no = $g_day_no % 365;
    }
    for ($i = 0; $g_day_no >= $g_days_in_month[$i] + ($i == 1 && $leap); $i++) {
      $g_day_no -= $g_days_in_month[$i] + ($i == 1 && $leap);
    }
    $gm = $i + 1;
    $gd = $g_day_no + 1;
    return [$gy, $gm, $gd];
  }

  private static function _format($format, $obj, $convert) {
    //Find what to replace
    $chars = (preg_match_all('/([a-zA-Z]{1})/', $format, $chars)) ? $chars[0] : [];

    //Intact Keys
    $intact = [
      'B',
      'h',
      'H',
      'g',
      'G',
      'i',
      's',
      'I',
      'U',
      'u',
      'Z',
      'O',
      'P',
    ];
    $intact = self::filterArray($chars, $intact);
    $intactValues = [];
    foreach ($intact as $k => $v) {
      $intactValues[$k] = $obj->format($v);
    }
    //End Intact Keys
    //Changed Keys
    list($year, $month, $day) = [
      $obj->format('Y'),
      $obj->format('n'),
      $obj->format('j'),
    ];
    list($jyear, $jmonth, $jday) = self::toJalali($year, $month, $day);
    $keys = [
      'd',
      'D',
      'j',
      'l',
      'N',
      'S',
      'w',
      'z',
      'W',
      'F',
      'm',
      'M',
      'n',
      't',
      'L',
      'o',
      'Y',
      'y',
      'a',
      'A',
      'c',
      'r',
      'e',
      'T',
    ];
    $keys = self::filterArray($chars, $keys, ['z']);
    $values = [];
    $temp_z = NULL;
    foreach ($keys as $k => $key) {
      $v = '';
      switch ($key) {
        //Day
        case 'd':
          $v = sprintf('%02d', $jday);
          break;
        case 'D':
          $v = self::getDayNames($convert, $obj->format('D'), TRUE);
          break;
        case 'j':
          $v = $jday;
          break;
        case 'l':
          $v = self::getDayNames($convert, $obj->format('l'));
          break;
        case 'N':
          $v = self::getDayNames($convert, $obj->format('l'), FALSE, 1, TRUE);
          break;
        case 'S':
          $from = $convert ? self::$order_conv : self::$order;
          $v = $from[$jday - 1];
          break;
        case 'w':
          $v = self::getDayNames($convert, $obj->format('l'), FALSE, 1, TRUE) - 1;
          break;
        case 'z':
          if ($jmonth > 6) {
            $v = 186 + (($jmonth - 6 - 1) * 30) + $jday;
          }
          else {
            $v = (($jmonth - 1) * 31) + $jday;
          }
          $temp_z = $v;
          break;
        //Week
        case 'W':
          $v = is_int($temp_z / 7) ? ($temp_z / 7) : intval($temp_z / 7 + 1);
          break;
        //Month
        case 'F':
          $v = self::getMonthNames($convert, $jmonth);
          break;
        case 'm':
          $v = sprintf('%02d', $jmonth);
          break;
        case 'M':
          $v = self::getMonthNames($convert, $jmonth, TRUE);
          break;
        case 'n':
          $v = $jmonth;
          break;
        case 't':
          if ($jmonth >= 1 && $jmonth <= 6) {
            $v = 31;
          }
          else {
            if ($jmonth >= 7 && $jmonth <= 11) {
              $v = 30;
            }
            else {
              if ($jmonth == 12 && $jyear % 4 == 3) {
                $v = 30;
              }
              else {
                if ($jmonth == 12 && $jyear % 4 != 3) {
                  $v = 29;
                }
              }
            }
          }
          break;
        //Year
        case 'L':
          $tmpObj = new DateTime('@' . (time() - 31536000));
          $v = $tmpObj->format('L');
          break;
        case 'o':
        case 'Y':
          $v = $jyear;
          break;
        case 'y':
          $v = $jyear % 100;
          break;
        //Time
        case 'a':
          $from = $convert ? self::$ampms_convert : self::$ampms;
          $v = $from[$obj->format('a')];
          break;
        case 'A':
          $from = $convert ? self::$ampm_convert : self::$ampm;
          $v = $from[$obj->format('a')];
          break;
        //Full Dates
        case 'c':
          $v = $jyear . '-' . sprintf('%02d', $jmonth) . '-' . sprintf('%02d', $jday) . 'T';
          $v .= $obj->format('H') . ':' . $obj->format('i') . ':' . $obj->format('s') . $obj->format('P');
          break;
        case 'r':
          $v = self::getDayNames($convert, $obj->format('D'), TRUE)
            . ', '
            . sprintf('%02d', $jday)
            . ' '
            . self::getMonthNames($convert, $jmonth, TRUE);
          $v .= ' '
            . $jyear
            . ' '
            . $obj->format('H')
            . ':'
            . $obj->format('i')
            . ':'
            . $obj->format('s')
            . ' '
            . $obj->format('P');
          break;
        //Timezone
        case 'e':
          $v = $obj->format('e');
          break;
        case 'T':
          $v = $obj->format('T');
          break;
      }
      $values[$k] = $v;
    }
    //End Changed Keys
    //Merge
    $keys = array_merge($intact, $keys);
    $values = array_merge($intactValues, $values);

    $ret = strtr($format, array_combine($keys, $values));

    if (FALSE && $convert) {
      $farsi_array = ['۰', '۱', '۲', '۳', '۴', '۵', '۶', '۷', '۸', '۹'];
      $english_array = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];
      return str_replace($english_array, $farsi_array, $ret);
    }
    else {
      return $ret;
    }
  }

}
