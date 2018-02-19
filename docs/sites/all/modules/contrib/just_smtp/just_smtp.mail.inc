<?php

/**
 * @file
 * The code processing mail in the just smtp module.
 *
 */

/**
 * Modifies the drupal mail system to use smtp when sending emails.
 */
class JustSmtpMailSystem implements MailSystemInterface {

  /**
   * Concatenate and wrap the e-mail body for either
   * plain-text or HTML emails.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   The formatted $message.
   */
  public function format(array $message) {
    if (!variable_get('just_smtp_on', FALSE)) {
      return $this->default_mail_system ()->format ($message);
    }

    return $message;
  }

  /**
   * Send the e-mail message.
   *
   * @see drupal_mail()
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   * @return
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
    if (!variable_get('just_smtp_on', FALSE)) {
      return $this->default_mail_system ()->mail ($message);
    }

    libraries_load('PHPMailer');
    module_load_include ('inc', 'just_smtp', 'just_smtp.phpmailer');
    $mail = new JustSmtpMailer;
    $mail->isSMTP();
    $mail->Host = variable_get('just_smtp_host', 'localhost');
    $mail->Port = variable_get('just_smtp_port', 25);
    if (variable_get('just_smtp_protocol', 'auto') == 'ssl')
      $mail->SMTPSecure = 'ssl';
    if ($mail->Port == 465)
      $mail->SMTPSecure = 'ssl';
    $mail->Username = variable_get('just_smtp_username', '');
    if(variable_get('just_smtp_encrypt', FALSE)) {
      $password = variable_get('just_smtp_password', '');
      $mail->Password = decrypt($password);
    }
    else {
      $mail->Password = variable_get('just_smtp_password', '');
    }
    $mail->SMTPAuth = empty ($mail->Username) ? FALSE : TRUE;
    $to = $this->parse_mail ($message['to']);
    if (!valid_email_address($to['mail'])) {
      drupal_set_message(t('The submitted to address (@to) is not valid.', array('@to' => $to['mail'])), 'error');
      watchdog('smtp', 'The submitted to address (@to) is not valid.', array('@from' => $to['mail']), WATCHDOG_ERROR);
      return FALSE;
    }
    $mail->addAddress ($to['mail'], $to['name']);

    $from = $this->parse_mail ($message['from']);
    if (!valid_email_address($from['mail'])) {
      drupal_set_message(t('The submitted from address (@from) is not valid.', array('@from' => $from['mail'])), 'error');
      watchdog('smtp', 'The submitted from address (@from) is not valid.', array('@from' => $from['mail']), WATCHDOG_ERROR);
      return FALSE;
    }
    $mail->setFrom ($from['mail'], $from['name']);

    $header = $this->mime_headers ($message);
    $body = $message['body'];
    $result = $mail->smtp_send($header, $body);
    if (!$result) {
      watchdog('just_smtp', 'Error sending e-mail from @from to @to : !error_message', array('@from' => $from['mail'], '@to' => $to['mail'], '!error_message' => $mail->ErrorInfo), WATCHDOG_ERROR);
    }
    return $result;
  }

  protected function mime_headers ($message) {
    $mimeheaders = array();
    $mimeheaders[] = 'To: ' . mime_header_encode($message['to']);
    $mimeheaders[] = 'Subject: ' . mime_header_encode($message['subject']);
    foreach ($message['headers'] as $name => $value) {
      $mimeheaders[] = $name . ': ' . mime_header_encode($value);
    }
    $mail_headers = join("\n", $mimeheaders);
    $mail_headers .= "\n";
    return $mail_headers;
  }

  /**
   * Parse mail address like: My Name <name@example.com>
   */
  protected function parse_mail ($mail) {
    if (preg_match('/^"?.*"?\s*<.*>$/', $mail)) {
      $name = preg_replace('/"?([^("\t\n)]*)"?.*$/', '$1', $mail); // Extract just the name
      $email = preg_replace("/(.*)\<(.*)\>/i", '$2', $mail); // Extract just the mail address
    }
    else {
      $email = $mail;
      $name = '';
    }
    return array ('mail' => $email, 'name' => $name);
  }

  protected function default_mail_system () {
    $default = &drupal_static(__FUNCTION__, array());
    if (!$default)
      $default = new DefaultMailSystem();
    return $default;
  }

}
