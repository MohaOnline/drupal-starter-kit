<?php

include_once 'miniorange_config.php';

function miniorange_oauth_client_config($form, &$formstate)
{
    $form = miniorange_config($form, $formstate);
    return $form;
}

?>