<?php
/**
 * this function shows iframe of the FAQ tab
 */
function mo_oauth_faq($form,$form_state)
{
    $form['miniorange_oauth_client_faq'] = array(
        '#id' => 'miniorange_oauth_client_faq',
        '#markup' => "<div>
                        <object type='text/html' data='https://faq.miniorange.com/kb/oauth-openid-connect/' width='100%' height='600px' ></object>
                    </div>"
    );
    return $form;
}