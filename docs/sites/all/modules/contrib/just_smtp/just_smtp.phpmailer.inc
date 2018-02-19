<?php

/**
 * @file
 * Make certain internals of PHPMailer available to the outside.
 */

class JustSmtpMailer extends PHPMailer {

  public function smtp_send($header, $body) {
    return $this->smtpSend ($header, $body);
  }

}
