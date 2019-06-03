<?php

/**
 * Form constructor for the moxtra configuration form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup forms
 *
 * @return array
 */
function opigno_moxtra_app_moxtra_configuration_form($form, &$form_state) {
  $org_id = variable_get('opigno_moxtra_app_org_id');

  if (empty($org_id)) {
    $form['disable_markup'] = array(
      '#markup' => '<div class="row">
          <div class="col col-1-out-of-4 pl-0">
            <div class="bg-faded text-center mb-3 pa-4">
              <div class="amount">' . '12$' . '</div>
              <div class="unit">' . t('/user /month') . '</div>
              <ul class="text-left my-4">
                <li>' . t('File/Document Sharing and Viewing') . '</li>
                <li>' . t('Multiparty Messaging') . '</li>
                <li>' . t('Share Screen from any device') . '</li>
                <li>' . t('Whiteboard with all Meeting Members') . '</li>
                <li>' . t('Ability to Record and Share Meetings') . '</li>
				        <li>' . t('Ability to save annotated files') . '</li>
              </ul>
              <div><strong>' . t('30 days free trial') . '</strong></div>
            </div>
            <div class="bg-faded text-center mb-3 pa-4">
              <input type="submit" name="create_organization" class="form-submit try" value="' . t('START TRIAL') . '">
            </div>
          </div>
          <div class="col col-3-out-of-4 pr-0">
            <h2>' . t('ENHANCE THE INTERACTIONS BETWEEN YOUR LEARNERS') . '</h2>
            <div class="row">
              <div class="col col-3-out-of-4 pl-0">' . t('Opigno offer a great collaborative workspace feature to facilitate collaboration between your learners and teachers. This feature enables mutli-partite chat with multi-layered document and content interactions. Improve your participation / engagement rate on our platform thanks the tools of our collaborative solution : chat, document sharing, annotations, to-do lists, live meetings.') . '</div>
              <div class="col col-1-out-of-4 pr-0">
                <input type="submit" name="create_organization" class="form-submit try" value="' . t('START TRIAL') . '">
              </div>
            </div>
          </div>
        </div>'
    );
    $form['button_create_form'] = array(
      '#type' => 'submit',
      '#submit' => array('opigno_moxtra_app_moxtra_configuration_form_submit'),
      '#value' => t('Activate the app on Opigno.org'),
      '#name' => 'create_organization',
      '#attributes' => array(
        'style' => 'display: none;'
      )
    );
  }
  else {
    $info = opigno_moxtra_app_api_opigno_get_organization_info();

    //// When its in trial and current trial is still active
    /*$info = array(
      'http_code' => 200,
      'max_start_meeting_users' => 0,
      'current_start_meeting_users' => 0,
      'email' => 'admin@opignolms128.local',
      'valid_until' => 1519986398,
      'current_total_users' => 3,
      'max_total_users' => 0,
      'active' => 0,
    );

    $info = (object) $info;

    //// When its in trial and current trial has finished
    $info = array(
      'http_code' => 200,
      'max_start_meeting_users' => 0,
      'current_start_meeting_users' => 0,
      'email' => 'admin@opignolms128.local',
      'valid_until' => 1511986398,
      'current_total_users' => 3,
      'max_total_users' => 0,
      'active' => 0,
    );

    $info = (object) $info;


    //// When its a paid account its active
    $info = array(
      'http_code' => 200,
      'max_start_meeting_users' => 0,
      'current_start_meeting_users' => 0,
      'email' => 'admin@opignolms128.local',
      'valid_until' => 1519986398,
      'current_total_users' => 3,
      'max_total_users' => 10,
      'active' => 0,
    );

    $info = (object) $info;



    //// When its a paid account and the time has passed
    $info = array(
      'http_code' => 200,
      'max_start_meeting_users' => 0,
      'current_start_meeting_users' => 0,
      'email' => 'admin@opignolms128.local',
      'valid_until' => 1511986398,
      'current_total_users' => 3,
      'max_total_users' => 10,
      'active' => 0,
    );

    $info = (object) $info;*/


    if (!isset($info->opigno_error_message)) {
      if ($info->active) {
        variable_set('opigno_moxtra_app_status', 1);
      }
      else {
        variable_set('opigno_moxtra_app_status', 0);
      }
    }

    if (opigno_moxtra_app_organization_is_active()) {
      $active = t('Active');
    }
    else {
      $active = t('Inactive');
    }

    $want_more = '';
    if ($info->max_total_users == 0) {
      $trial = TRUE;
      $quota = t('There is no user quota during trial');
      $organization_type = t('Trial');
    }
    else {
      $trial = FALSE;
      $quota = $info->current_total_users . '/' . $info->max_total_users . ' ' . t('user(s)');
      $organization_type = t('Paid');
      $want_more = l(t('Want more?'), OPIGNO_MOXTRA_APP_OPIGNO_RENEW_GATEWAY . $org_id, array(
          'attributes' => array(
            'target' => '_blank',
            'class' => 'decoration-none style-italic'
          )
        ));
    }

    $date_diff = $info->valid_until - time();
    $date_diff_days = round($date_diff / (60 * 60 * 24));

    $contact_form = "";
    if ($date_diff_days < 0) {
      $date_diff_days = 0;
      if ($trial) {
        $validity_text = t('Your trial period has ended');
        $contact_form = '<div class="mb-2">' . l(t('Because the trial has expired users might be out of sync, please contact us after renewing the plan'), 'https://www.opigno.org/en#contact', array(
            'attributes' => array(
              'target' => '_blank',
              'class' => 'decoration-none style-italic'
            )
          )) . '</div>';
      }
      else {
        $validity_text = t('Your plan has expired');
        $contact_form = '<div class="mb-2">' . l(t('Because your plan has expired users might be out of sync, please contact us after renewing the plan'), 'https://www.opigno.org/en#contact', array(
            'attributes' => array(
              'target' => '_blank',
              'class' => 'decoration-none style-italic'
            )
          )) . '</div>';
      }
    }
    else {
      if ($trial) {
        $validity_text = t('days before the end of the trial');
      }
      else {
        $validity_text = t('days remaining in your plan');
      }
    }

    $form['enable_markup_1'] = array(
      '#markup' => '<div class="row">
          <div class="col col-1-out-of-4 pl-0 bg-faded text-center mb-3 pa-4">
            <div class="d-xl-flex align-items-center mb-4">
              <div class="timeout ">' . $date_diff_days . '</div>
              <div class="unit text-xl-left ml-2">' . $validity_text . '</div>
            </div>
            ' . l(t('Buy'), OPIGNO_MOXTRA_APP_OPIGNO_RENEW_GATEWAY . $org_id, array(
            'attributes' => array(
              'target' => '_blank',
              'class' => 'decoration-none form-submit cart'
            )
          )) . '
          </div>
          <div class="col col-3-out-of-4 mb-3 pr-0">
            <div class="bg-faded text-center pa-4">
              <h3 class="text-left mt-0">' . t('Informations') . '</h3>
              ' . $contact_form . '
              <div class="d-flex border-top border-bottom py-2">
                <div class="text-label text-bold mr-3">' . t('Your organization ID on Opigno.org is') . '</div>
                <div>' . $org_id . '</div>
              </div>

              <div class="d-flex border-bottom py-2">
                <div class="text-label text-bold mr-3">' . t('Your organization is:') . '</div>
                <div>' . $active . '</div>
              </div>

              <div class="d-flex border-bottom py-2">
                <div class="text-label text-bold mr-3">' . t('Your organization type is:') . '</div>
                <div>' . $organization_type . '</div>
              </div>

              <div class="d-flex border-bottom py-2">
                <div class="text-label text-bold mr-3">' . t('You organization contact is:') . '</div>
                <div>' . $info->email . ' </div>
              </div>

              <div class="d-flex border-bottom py-2">
                <div class="text-label text-bold mr-3">' . t('Your instance user quota') . '</div>
                <div>
                  ' . $quota . '<br />
                  ' . $want_more . '
                </div>
              </div>
            </div>
          </div>
        </div>'
    );
  }

  return $form;
}

/**
 * Form submit handler for the moxtra configuration form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup forms
 */
function opigno_moxtra_app_moxtra_configuration_form_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#name'] == 'create_organization') {
    $response = opigno_moxtra_app_api_opigno_create_organization();
    if (isset($response->opigno_error_message)) {
      drupal_set_message(t('An error occurred while creating the organization. Try again, check the logs or contact an opigno.org administrator.'));
    }
    else {
      drupal_set_message(t('Organization created successfully. You can now use the Moxtra App.'));

      // Install automatically the collaborative workspace module and the live meetings module.
      module_enable(array('opigno_collaborative_workspaces'));
      module_enable(array('opigno_live_meetings'));
      variable_get('opigno_moxtra_app_status', 1);
      drupal_flush_all_caches();
    }
  }
}
