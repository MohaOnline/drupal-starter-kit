<?php

class TfaTestLogin extends TfaBasePlugin implements TfaLoginPluginInterface {

  protected $loginUid;

  public function __construct(array $context = array(), $uid) {
    parent::__construct($context);
    $this->loginUid = $uid;
  }

  public function loginAllowed() {
    if ($this->context['uid'] === $this->loginUid) {
      return TRUE;
    }
    return FALSE;
  }
}
