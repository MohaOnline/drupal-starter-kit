<?php
/**
 * @file
 * Opigno statistics app - User - General informations template file
 *
 * @param $general_informations
 *  $general_informations['picture']
 *  $general_informations['name']
 *  $general_informations['email']
 *  $general_informations['date_joined']
 *  $general_informations['last_access']
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-user-widget-general-informations">
  <div class="text-center mb-5">
    <div class="d-inline-block v-align-middle mx-4 profile-image">
      <?php print theme('image', array('path' => $general_informations['picture'], 'width' => 255)); ?>
    </div>
    <div class="d-inline-block v-align-middle mx-4">
      <div class="profile-name"><?php print $general_informations['name']; ?></div>
      <div class="separator my-3"></div>
      <div class="profile-email"><?php print $general_informations['email']; ?></div>
    </div>
  </div>
  <div class="bg-grey text-center pa-2">
    <div class="d-inline-block mx-3">
      <?php try { $date=new DateTime('@' . $general_informations['date_joined']); // If no date, Exception caught and nothing appear ?>
      <b><?php print t('Date joined'); ?></b><br/><span><?php print $date->format('Y-m-d'); ?></span>
      <?php } catch (Exception $e) {} ?>
    </div>
    <div class="d-inline-block mx-3">
      <?php try { $date=new DateTime('@' . $general_informations['last_access']); ?>
      <b><?php print t('Last access'); ?></b><br/><span><?php print $date->format('Y-m-d'); ?></span>
      <?php } catch (Exception $e) {} ?>
    </div>
    <div class="d-inline-block mx-3">
      <?php if (isset($general_informations['date_joined'])): ?>
        <b><?php print t('Member for'); ?></b><br/><span><?php print format_interval(REQUEST_TIME - $general_informations['date_joined']); ?></span>
      <?php endif; ?>
    </div>
  </div>
</div>
