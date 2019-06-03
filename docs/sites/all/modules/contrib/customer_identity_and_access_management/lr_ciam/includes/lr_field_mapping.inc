<?php

/**
 * create user profile2 fields.
 *
 * @param object $account User account information
 * @param object $userprofile User profile data that you got from social network
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 */
function lr_field_mapping_ciam_create_profile_user($account, $userprofile, $form, $form_state) {
  $field_map = variable_get('lr_ciam_profile_fields', array());

  foreach ($field_map as $key => $value) {
    $mapped_value[$key] = array_values($value);
  }

  $field_convert_info = lr_field_mapping_ciam_field_convert_info();
  foreach (field_info_instances('profile2') as $field_name => $instances) {
    if (!empty($mapped_value[$field_name]) && strlen(implode($mapped_value[$field_name])) != 0) {
      $profile = profile2_load_by_user($account->uid, $field_name);

      if (!$profile) {
        $profile = profile_create(array(
          'type' => $field_name,
          'uid' => $account->uid
        ));
      }

      foreach ($instances as $instance => $array) {
        $field = field_info_field($array['field_name']);

        if (isset($field_map[$field_name][$array['field_name']]) && isset($field_convert_info[$field['type']]['callback'])) {
          $callback = $field_convert_info[$field['type']]['callback'];
          $lr_field_mapping_property_name = $field_map[$field_name][$array['field_name']];
          $value = $callback($lr_field_mapping_property_name, $userprofile, $field, $array);
          $profile->{$array['field_name']}[LANGUAGE_NONE][0]['value'] = $value; // department info got from 3rd party site  
        }
        $form_state['values']['profile_' . $field_name] = array();
        $form = array();
        $form['#parents'] = array();
        field_attach_submit('profile2', $profile, $form, $form_state); // attach $profile to profile2 submit
        $profile->bundle = $field_name; // main is the profile type which is created in step 3
        profile2_save($profile);
      }
    }
  }
}

/**
 * Function that adding field data.
 *
 * @param array $form_state A keyed array containing the current state of the form.
 * @param object $userprofile User profile data that you got from social network
 */
function lr_field_mapping_ciam_create_user(&$form_state, $userprofile, $account = '') {
  if (isset($account->mail)) {
    $userprofile->Email_value = $account->mail;
  }

  $field_map = variable_get('lr_ciam_user_fields', array());
  $field_convert_info = lr_field_mapping_ciam_field_convert_info();
  $instances = field_info_instances('user', 'user');

  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($instance['field_name']);

    if (isset($field_map[$field_name]) && isset($field_convert_info[$field['type']]['callback'])) {
      $callback = $field_convert_info[$field['type']]['callback'];
      $lr_field_mapping_property_name = $field_map[$field_name];

      if ($value = $callback($lr_field_mapping_property_name, $userprofile, $field, $instance)) {
        $form_state[$field_name][LANGUAGE_NONE][0] = $value;
      }
    }
  }
}

/**
 * Function that provide a callback for data map.
 *
 * @return array Get array of label and callback through which we can field data
 */
function lr_field_mapping_ciam_field_convert_info() {
  $convert_info = array(
    'text' => array(
      'label' => t('Text'),
      'callback' => 'lr_field_mapping_ciam_field_convert_text',
    ),
    'text_long' => array(
      'label' => t('Long text'),
      'callback' => 'lr_field_mapping_ciam_field_convert_text',
    ),
    'list_text' => array(
      'label' => t('List (\'text\')'),
      'callback' => 'lr_field_mapping_ciam_field_convert_list',
    ),
    'datetime' => array(
      'label' => t('Date'),
      'callback' => 'lr_field_mapping_ciam_field_convert_date',
    ),
    'date' => array(
      'label' => t('Date'),
      'callback' => 'lr_field_mapping_ciam_field_convert_date',
    ),
    'datestamp' => array(
      'label' => t('Date'),
      'callback' => 'lr_field_mapping_ciam_field_convert_date',
    ),
  );

  drupal_alter('lr_field_mapping_ciam_field_convert_info', $convert_info);
  return $convert_info;
}

/**
 * Convert text and text_long data.
 *
 * @param string $lr_field_mapping_property_name User profile property name thorugh which data mapped
 * @param object $userprofile User profile data that you got from social network
 * @param string User field name stored in database
 * @param string $instance Field instance
 * @return array  Contain value of field map data
 */
function lr_field_mapping_ciam_field_convert_text($lr_field_mapping_property_name, $userprofile, $field, $instance) {
  $value = NULL;
if(!empty($lr_field_mapping_property_name) && !(strpos($lr_field_mapping_property_name, 'ciam|') !== false)){
  if (isset($userprofile->$lr_field_mapping_property_name)) {
    if (is_string($userprofile->$lr_field_mapping_property_name)) {
      $value = $userprofile->$lr_field_mapping_property_name;
    }
    elseif (is_object($userprofile->$lr_field_mapping_property_name)) {
      $object = $userprofile->$lr_field_mapping_property_name;

      if (isset($object->name)) {
        $value = $object->name;
      }
    }
    elseif(is_array($userprofile->likes)){
         $array = json_decode(json_encode($userprofile->likes), true);
         $result = array_map(function ($ar) {return $ar['Name'];}, $array);
         $value  =  implode(",", $result);
    }
  }

  return $value ? array('value' => $value) : NULL;
  }
}

/**
 * Convert list data.
 *
 * @param string $lr_field_mapping_property_name User profile property name thorugh which data mapped
 * @param object $userprofile User profile data that you got from social network
 * @param string $field User field name stored in database
 * @param string $instance Field instance
 * @return array Contain value of field map data
 */
function lr_field_mapping_ciam_field_convert_list($lr_field_mapping_property_name, $userprofile, $field, $instance) {
  if(!empty($lr_field_mapping_property_name) && !(strpos($lr_field_mapping_property_name, 'ciam|') !== false)){
  if (!isset($userprofile->$lr_field_mapping_property_name) && !is_string($userprofile->$lr_field_mapping_property_name)) {
    return;
  }

  $options = list_allowed_values($field);
  $best_match = 0.0;
  $match_sl = strtolower($userprofile->$lr_field_mapping_property_name);

  foreach ($options as $key => $option) {
    $option = trim($option);
    $match_option = strtolower($option);
    $this_match = 0;
    similar_text($match_option, $match_sl, $this_match);

    if ($this_match > $best_match) {
      $best_match = $this_match;
      $best_key = $key;
    }
  }

  return isset($best_key) ? array('value' => $best_key) : NULL;
  }
}

/**
 * Convert date data.
 *
 * @param $lr_field_mapping_property_name User profile property name thorugh which data mapped
 * @param $userprofile User profile data that you got from social network
 * @param $field User field name stored in database
 * @param $instance Field instance
 * @return array Contain value of field map data
 *
 */
function lr_field_mapping_ciam_field_convert_date($lr_field_mapping_property_name, $userprofile, $field, $instance) {
if(!empty($lr_field_mapping_property_name) && !(strpos($lr_field_mapping_property_name, 'ciam|') !== false)){
  if (isset($userprofile->$lr_field_mapping_property_name)) {
    $value = NULL;
    $sldate = explode('/', $userprofile->$lr_field_mapping_property_name);

    if (count($sldate) == 3) {
      $date = new DateObject($userprofile->$lr_field_mapping_property_name);

      if (date_is_date($date)) {
        $format = $field['type'] == 'datestamp' ? DATE_FORMAT_UNIX : DATE_FORMAT_ISO;
        $value = array(
          'value' => $date->format($format, TRUE),
          'date_type' => $field['type'],
        );
      }
    }

    return $value;
  }
  }
}

/**
 * Return a all list of social user properties.
 *
 * @return array Get all list of user properties that will show at field mapping
 */
function lr_field_mapping_ciam_user_properties() {
  $common = array(
    'ID' => array(
      'label' => t('Provider ID'),
    ),
    'Provider' => array(
      'label' => t('Social Provider'),
      'field_types' => array('text'),
    ),
    'FullName' => array(
      'label' => t('Full name'),
      'field_types' => array('text'),
    ),
    'FirstName' => array(
      'label' => t('First name'),
      'field_types' => array('text'),
    ),
    'LastName' => array(
      'label' => t('Last name'),
      'field_types' => array('text'),
    ),
    'Email_value' => array(
      'label' => t('E-mail'),
      'field_types' => array('text'),
    ),
    'Gender' => array(
      'label' => t('Gender'),
      'field_types' => array('text', 'list_text'),
    ),
    'BirthDate' => array(
      'label' => t('Birthday'),
      'field_types' => array('text', 'date', 'datetime', 'datestamp'),
    ),
    'About' => array(
      'label' => t('About me (a short bio)'),
      'field_types' => array('text', 'text_long'),
    ),
    'HomeTown' => array(
      'label' => t('HomeTown'),
      'field_types' => array('text'),
    ),
    'Company_name' => array(
      'label' => t('Work history'),
      'field_types' => array('text'),
    ),
    'Website' => array(
      'label' => t('Website Url'),
      'field_types' => array('text'),
    ),
    'Company' => array(
      'label' => t('Company'),
      'field_types' => array('text'),
    ),
    'NickName' => array(
      'label' => t('Nick name'),
      'field_types' => array('text'),
    ),
    'State' => array(
      'label' => t('State'),
      'field_types' => array('text'),
    ),
    'City' => array(
      'label' => t('City'),
      'field_types' => array('text'),
    ),
    'LocalCity' => array(
      'label' => t('Local City'),
      'field_types' => array('text'),
    ),
    'Country_name' => array(
      'label' => t('Country'),
      'field_types' => array('text'),
    ),
    'LocalCountry' => array(
      'label' => t('Local Country'),
      'field_types' => array('text'),
    ),
    'ID' => array(
      'label' => t('Social ID'),
      'field_types' => array('text'),
    ),
    'ThumbnailImageUrl' => array(
      'label' => t('Thumbnail'),
      'field_types' => array('text'),
    ),
    'PhoneNumber' => array(
      'label' => t('PhoneNumber'),
      'field_types' => array('text'),
    ),
    'PostalCode' => array(
      'label' => t('PostalCode'),
      'field_types' => array('text'),
    ),
    ''
  );

  $common = array_merge(advance_extended_columns_ciam_field_mapping(), $common);
  drupal_alter('lr_field_mapping_ciam_user_properties', $common);
  ksort($common);
  $common = array_map("unserialize", array_unique(array_map("serialize", $common)));
  return $common;
}

/**
 * Get mapping field data from extended data which is saved in database.
 *
 * @param string $key To identify the field mapping data is in basic or extended data
 * @param string $table_name Database table name of extended data
 * @param array $mappingfields Basic mapping fields in field mapping
 * @return array Add extended mapping fields to basic mapping fields
 */
function advance_extended_columns_ciam_field_mapping() {
  //Check table exists in database.
  $extended_profile = array('Website','Favicon','Industry','About',
    'TimeZone','Verified', 'UpdatedTime','Created', 'RelationshipStatus', 'Quota','InterestedIn', 'Interests', 'Religion','Political',
    'HttpsImageUrl', 'FollowersCount', 'FriendsCount', 'IsGeoEnabled', 'TotalStatusesCount', 'NumRecommenders', 'Honors',
    'Associations', 'Hireable', 'Age', 'ProfessionalHeadline', 'MainAddress','LocalCity','ProfileCity', 'ProfileUrl', 'LocalLanguage', 'Language');
  foreach ($extended_profile as $profile_column) {
    $columns_array[$profile_column] = array(
      'label' => $profile_column,
      'field_types' => array('text')
    );
  }
  return $columns_array;

}

/**
 * Fill user fields and profile fields to register form.
 *
 * @param $form Nested array of form elements that comprise the form.
 * @param $userprofile User profile data that you got from social network
 */
function lr_field_mapping_show_ciam_profile_field_to_register_form(&$form, $userprofile) {
  $field_map = variable_get('lr_ciam_user_fields', array());
  $field_convert_info = lr_field_mapping_ciam_field_convert_info();
  $instances = field_info_instances('user', 'user');

  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($instance['field_name']);

    if (isset($field_map[$field_name]) && isset($field_convert_info[$field['type']]['callback'])) {
      $callback = $field_convert_info[$field['type']]['callback'];
      $lr_field_mapping_property_name = $field_map[$field_name];

      if ($value = $callback($lr_field_mapping_property_name, $userprofile, $field, $instance)) {
        $form[$field_name][LANGUAGE_NONE][0]['value']['#default_value'] = $value;
      }
    }
  }

  if (module_exists('profile2')) {
    $field_map = variable_get('lr_ciam_profile_fields', array());
    $field_convert_info = lr_field_mapping_ciam_field_convert_info();

    foreach (field_info_instances('profile2') as $field_name => $instances) {
      foreach ($instances as $instance => $array) {
        $field = field_info_field($array['field_name']);

        if (isset($field_map[$field_name][$array['field_name']]) && isset($field_convert_info[$field['type']]['callback'])) {
          $callback = $field_convert_info[$field['type']]['callback'];
          $lr_field_mapping_property_name = $field_map[$field_name][$array['field_name']];

          if ($value = $callback($lr_field_mapping_property_name, $userprofile, $field, $array)) {
            $form['profile_' . $field_name][$array['field_name']][LANGUAGE_NONE][0]['value']['#default_value'] = $value;
          }
        }
      }
    }
  }
}