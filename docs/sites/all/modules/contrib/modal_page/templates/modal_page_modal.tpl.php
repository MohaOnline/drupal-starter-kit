<?php

/**
 * @file
 * Default theme implementation for modal.
 *
 * Available variables:
 * - $title: The title of modal.
 * - $text: The text of modal.
 * - $button: The button label of modal.
 */
?>

<div class="modal fade" id="js-modal-page-show-modal" role="dialog">
    <div class="modal-dialog">
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title"><?php print $title; ?></h4>
        </div>
        <div class="modal-body">
          <p><?php print $text; ?></p>
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-default" data-dismiss="modal"><?php print $button; ?></button>
        </div>
      </div>
    </div>
  </div>
