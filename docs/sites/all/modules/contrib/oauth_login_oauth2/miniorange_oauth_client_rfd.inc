<?php

function miniorange_oauth_client_request_for_demo($form,$form_state)
{

    drupal_add_css(drupal_get_path('module', 'oauth_login_oauth2') . '/css/bootstrap.min.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css(drupal_get_path('module', 'oauth_login_oauth2') . '/css/style_settings.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    $user_email = variable_get('miniorange_oauth_client_customer_admin_email');

    $form['mo_header_style'] = array('#markup' => '<div class="mo_oauth_table_layout_1">');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_oauth_table_layout mo_oauth_container">',
    );

    $form['markup_1'] = array(
        '#markup' => '<h2>Request for demo</h2><hr><br>'
    );

    $form['markup_2'] = array(
        '#markup' => '<div class="mo_oauth_highlight_background_note_export"><p><strong>Want to test any of the Premium module before purchasing? </strong></p>
            <p>Just send us a request, We will setup a demo site for you on our cloud with the premium module and provide you with the administrator credentials.
                You can configure it with your OAuth Server and test all the premium features as per your requirement.</p>
        </div><br>',
    );

    $form['customer_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#required' => TRUE,
        '#default_value' => t(strval($user_email)),
        '#attributes' => array('style' => 'width:65%;', 'placeholder' => 'Enter your email'),
        '#description' => t('<b>Note:</b> Use valid EmailId. ( We discourage the use of disposable emails )'),
    );

    $form['demo_plan'] = array(
        '#type' => 'select',
        '#title' => t('Demo Plan'),
        '#attributes' => array('style' => 'width:65%;'),
        '#options' => [
            'Drupal 7 OAuth Standard Module' => t('Drupal 7 OAuth Standard Module'),
            'Drupal 7 OAuth Premium Module' => t('Drupal 7 OAuth Premium Module'),
            'Drupal 7 OAuth Enterprise Module' => t('Drupal 7 OAuth Enterprise Module'),
            'Not Sure' => t('Not Sure'),
        ],
    );

    $form['description_doubt'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#attributes' => array('style' => 'width:65%', 'placeholder' => 'Describe your requirement'),
        '#required' => TRUE,
        '#resizable' => FALSE,
    );

    $form['submit_button'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#submit' => array('miniorange_oauth_client_rfd_submit'),
        '#prefix' => '<br>',
        '#suffix' => '<br><br></div>',
    );

    Utilities::spConfigGuide($form, $form_state);
    return $form;
}


function miniorange_oauth_client_rfd_submit(&$form, $form_state) {

    $email = trim($form['customer_email']['#value']);
    $demo_plan = $form['demo_plan']['#value'];
    $description_doubt = trim($form['description_doubt']['#value']);
    $query = $demo_plan.' -> '.$description_doubt;
    if(valid_email_address($email))
        Utilities::send_query($email,null, $query, 'demo');
    else {
        drupal_set_message('Please enter valid Email ID', 'error');
        return;
    }
}

?>