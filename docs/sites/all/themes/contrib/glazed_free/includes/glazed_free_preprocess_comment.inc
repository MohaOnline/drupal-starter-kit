<?php

/**
 * Implements template_preprocess_comment().
 * Using comment patch https://www.drupal.org/files/bootstrap-use-media-object-as-comments-2109369-6.patch
 */
function glazed_free_preprocess_comment(&$vars) {
  $comment = $vars['elements']['#comment'];
  $node = $vars['elements']['#node'];
  if ($vars['elements']['#comment']->depth > 0) {
    $vars['author'] .= ' ' . t('replied') . ':';
  }
  else {
    $vars['author'] .= ' ' . t('said') . ':';
  }
  $vars['classes_array'][] = 'media';
  $vars['title_attributes_array']['class'][] = 'media-heading';
  $vars['time_ago'] = format_interval((time() - $comment->changed) , 2) . t(' ago');
}
