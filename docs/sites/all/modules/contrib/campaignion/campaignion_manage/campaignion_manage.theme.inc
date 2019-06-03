<?php

function theme_campaignion_manage_node($variables) {
  $node = $variables['node'];
  $title = $node->title;
  $status_translated = ($node->status == 1) ? t('Published') : t('Draft');
  $status_css = 'status-' . (($node->status == 1) ? 'published' : 'draft');
  $type_translated = t($node->type);
  $content_type_name = node_type_get_name($node);
  $user = user_load($node->uid);
  $author = $user->name;

  if (($path = drupal_lookup_path('alias','node/' . $node->nid)) == FALSE) {
    $path = 'node/' . $node->nid;
  }
  $link = l($title, $path);
  $langcode = $node->language;
  $language_html = "";
  if ($langcode) {
    $languages = language_list();
    $langcode_uppercase = strtoupper($langcode);

    if (isset($languages[$langcode])) {
      $language = $languages[$langcode];
      $language_html = "<span class=\"language\">".$language->name." (".$langcode_uppercase.")</span>";
    } else {
      $language_html = "<span class=\"language\">" . t('No language') . " (".$langcode_uppercase.")</span>";
    }
  }

  $author_html = '';
  if (!empty($author)) {
    $author_html .= empty($language_html) ? "\n" : ",\n";
    $author_html .= '<span class="author">' . $author . '</span>';
  }

  $content = <<<EOT
<span class="icon"></span>
<div class="heading">
<span class="title">{$link}</span>
<span class="status {$status_css}">{$status_translated}</span>
</div>
<div class="additional-info">
<span class="type">{$content_type_name}</span>
{$language_html}{$author_html}
</div>
EOT;

  return $content;
}

function theme_campaignion_manage_contact($variables) {
  $contact = $variables['contact'];
  $wrappedContact = entity_metadata_wrapper('redhen_contact', $contact->contact_id);
  $link = l($wrappedContact->full_name->value(), "redhen/contact/{$contact->contact_id}/view");
  $email = $contact->redhen_contact_email_value;

  $tags = '';
  foreach ($contact->tags as $tag) {
    $tags .= "<span class=\"tag\">$tag</span>";
  }

  $hold = ($contact->redhen_contact_email_hold === '1') ? '<span class="hold">Hold!</span>' : '';

  $content = <<<EOT
<span class="icon"></span>
<div class="heading">
<span class="title">{$link}</span>
{$tags}
{$hold}
</div>
<div class="additional-info">
<span class="email">{$email}</span>
</div>
EOT;
  return $content;
}

function theme_campaignion_manage_ajax($variables) {
  $element = &$variables['element'];
  // We need a double wrapper because jquery.replaceWith seems to produce extra-divs.
  return '<div id="campaignion-manage-ajax-wrapper"><div>' . $element['#children'] . '</div></div>';
}

/**
 * Workaround for the hardcoded $_GET['q'] in theme_pager_link.
 */
function theme_campaignion_manage_pager($variables) {
  $element = &$variables['element'];
  $element['#theme'] = 'pager';
  $old_q = $_GET['q'];
  if ($old_q == 'system/ajax' && !empty($_SERVER['HTTP_REFERER'])) {
    $url = parse_url($_SERVER['HTTP_REFERER']);
    $_GET['q'] = $url['path'];
  }
  $output = drupal_render($element);
  $_GET['q'] = $old_q;
  return $output;
}
