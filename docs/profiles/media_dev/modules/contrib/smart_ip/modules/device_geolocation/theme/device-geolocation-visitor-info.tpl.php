<?php

/**
 * @file device-geolocation-visitor-info.tpl.php
 * Default theme implementation for rendering user's geolocation details block.
 *
 * Available variables:
 * - $location: An associative array with possible array items:
 * -- latitude: May came from Smart IP or W3C Geolocation API 
 * -- longitude: May came from Smart IP or W3C Geolocation API
 * -- street_number: Google Geocoder service specific item
 * -- postal_code: Google Geocoder service specific item
 * -- route: Google Geocoder service specific item
 * -- neighborhood: Google Geocoder service specific item
 * -- locality: Google Geocoder service specific item
 * -- sublocality: Google Geocoder service specific item
 * -- establishment: Google Geocoder service specific item
 * -- administrative_area_level_N: Google Geocoder service specific item
 * -- country: May came from Smart IP or Google Geocoder service
 * -- country_code: May came from Smart IP or Google Geocoder service
 * -- zip: Smart IP specific item
 * -- region: Smart IP specific item
 * -- region_code: Smart IP specific item
 * -- time_zone: Smart IP specific item
 * -- is_eu_country: Smart IP specific item
 * -- is_gdpr_country: Smart IP specific item
 * -- ip_address: Smart IP specific item
 * -- timestamp: Timestamp of these data stored
 *
 * @ingroup themeable
 */
?>
<?php if (!empty($location)): ?>
  <dl>
    <?php foreach ($location as $item => $value): ?>
      <?php if (($item == 'is_eu_country' && !$value) || ($item == 'is_gdpr_country' && !$value) || (!empty($value) && $item != 'region_code' && $item != 'original_data')): ?>
        <?php
          if ($item == 'source') {
            switch ($value) {
              case SMART_IP_SOURCE_GEOCODED_SMART_IP:
                $value = t('Google Map Geocoded Smart IP coordinates');
                break;
              case SMART_IP_SOURCE_W3C:
                $value = t('Geocoded W3C coordinates');
                break;
              default:
                $value = t('Smart IP geolocation');
                break;
            }
          }
          elseif ($item == 'is_eu_country' || $item == 'is_gdpr_country') {
            if ($value) {
              $value = t('Yes');
            }
            else {
              $value = t('No');
            }
          }
          elseif ($item == 'timestamp') {
            $value = format_date($value, 'long');
          }
          $item = str_replace('_', ' ', $item);
          $item = ucwords(strtolower($item));
        ?>
        <dt><?php print $item; ?></dt>
        <dd><?php print $value; ?></dd>
      <?php endif; ?>
    <?php endforeach; ?>
  </dl>
<?php endif; ?>