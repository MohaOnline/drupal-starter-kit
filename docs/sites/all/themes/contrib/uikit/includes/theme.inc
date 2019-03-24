<?php

/**
 * @file
 * The UIkit theme system, which controls the output of the UIkit theme.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by UIkit themes.
 *
 * @ingroup uikit_themeable
 */

/**
 * Implements theme_admin_block_content().
 */
function uikit_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';

  if (!empty($content)) {
    $class = 'uk-description-list-line';

    if ($compact = system_admin_compact_mode()) {
      $class .= ' compact';
    }

    $output .= '<dl class="' . $class . '">';

    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';

      if (!$compact && isset($item['description'])) {
        $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
      }
    }

    $output .= '</dl>';
  }
  return $output;
}

/**
 * Implements theme_breadcrumb().
 */
function uikit_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  $output = '';

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .uk-hidden.
    $output .= '<h2 class="uk-hidden">' . t('You are here') . '</h2>';

    $output .= '<ul class="uk-breadcrumb">';

    foreach ($breadcrumb as $crumb) {
      $output .= '<li>' . $crumb . '</li>';
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Implements theme_button().
 */
function uikit_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));
  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $value = $element['#attributes']['value'];

  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<button' . drupal_attributes($element['#attributes']) . '>' . $value . '</button>';
}

/**
 * Implements theme_checkbox().
 */
function uikit_checkbox($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array(
    'id',
    'name',
    '#return_value' => 'value',
  ));

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox', 'uk-checkbox'));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_container().
 */
function uikit_container($variables) {
  $element = $variables['element'];
  // Ensure #attributes is set.
  $element += array('#attributes' => array());
  $prefix = '';
  $suffix = '';

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {
    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }

    // Add the 'form-wrapper' class.
    $element['#attributes']['class'][] = 'form-wrapper';
  }

  $type = isset($element['#type']) ? $element['#type'] : 0;
  $actions = $type && $type === 'actions';
  if ($actions) {
    $prefix .= '<div class="uk-button-group">';
    $suffix .= '</div>';
  }

  return '<div' . drupal_attributes($element['#attributes']) . '>' . $prefix . $element['#children'] . $suffix . '</div>';
}

/**
 * Implements theme_date_text().
 */
function uikit_date_text($variables) {
  $element = $variables['element'];
  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : array('class' => array());
  $attributes['class'][] = 'container-inline-date';
  // If there is no description, the floating date elements need some extra
  // padding below them.
  $wrapper_attributes = array();
  if (empty($element['date']['#description'])) {
    $wrapper_attributes['class'][] = 'clearfix';
  }
  // Add an wrapper to mimic the way a single value field works, for ease in
  // using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) . '>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) . '>' . theme('form_element', $element) . '</div>';
}

/**
 * Implements theme_field() for tag fields.
 */
function uikit_field__field_tags($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h4>' . $variables['label'] . '</h4>';
  }

  // Render the items.
  $output .= '<ul class="uk-subnav">';

  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }

  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Implements theme_fieldset().
 */
function uikit_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';

  if (!empty($element['#title'])) {
    $output .= '<legend class="uk-legend">' . $element['#title'] . '</legend>';
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</fieldset>';

  return $output;
}

/**
 * Implements theme_fieldset() for collapsible fieldsets.
 */
function uikit_fieldset__collapsible($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper', 'uk-form-width-large'));

  $output = '<ul' . drupal_attributes($element['#attributes']) . '>';

  $open = $element['#collapsed'] ? '' : ' class="uk-open"';
  $output .= '<li' . $open . '>';

  if (!empty($element['#title'])) {
    $output .= '<h3 class="uk-accordion-title">' . $element['#title'] . '</h3>';
  }

  $output .= '<div class="uk-accordion-content">';

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</div>';
  $output .= '</li>';
  $output .= '</ul>';

  return $output;
}

/**
 * Implements theme_fieldset() for formatted fieldsets.
 */
function uikit_fieldset__format($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper', 'uk-card', 'uk-card-body'));
  $output = '<div' . drupal_attributes($element['#attributes']) . '>';

  if (!empty($element['#title'])) {
    $output .= '<legend class="fieldset-legend">' . $element['#title'] . '</legend>';
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</div>';

  return $output;
}

/**
 * Implements theme_fieldset() for grouped fieldsets.
 */
function uikit_fieldset__grouped($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';

  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend class="fieldset-legend">' . $element['#title'] . '</legend>';
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="uk-text-small uk-text-muted uk-margin-small-top">' . $element['#description'] . '</p>';
  }

  $output .= $element['#children'];

  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }

  $output .= '</fieldset>';

  return $output;
}

/**
 * Implements theme_file().
 */
function uikit_file($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'file';
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-file'));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_filter_tips().
 */
function uikit_filter_tips($variables) {
  $tips = $variables['tips'];
  $long = $variables['long'];
  $output = '';

  $multiple = count($tips) > 1;
  if ($multiple) {
    $output = '<h2>' . t('Text Formats') . '</h2>';
  }

  if (count($tips)) {
    if ($multiple) {
      $output .= '<div class="compose-tips">';
    }

    foreach ($tips as $name => $tiplist) {
      if ($multiple) {
        $output .= '<div class="filter-type filter-' . drupal_html_class($name) . '">';
        $output .= '<h3>' . check_plain($name) . '</h3>';
      }

      if (count($tiplist) > 0) {
        $output .= '<ul class="tips uk-list">';
        foreach ($tiplist as $tip) {
          $output .= '<li' . ($long ? ' id="filter-' . str_replace("/", "-", $tip['id']) . '">' : '>') . $tip['tip'] . '</li>';
        }
        $output .= '</ul>';
      }

      if ($multiple) {
        $output .= '</div><hr class="uk-article-divider">';
      }
    }

    if ($multiple) {
      $output .= '</div>';
    }
  }

  return $output;
}

/**
 * Implements theme_form().
 */
function uikit_form($variables) {
  $element = $variables['element'];

  $horizontal_forms = array(
    'node_admin_content',
  );

  if (in_array($element['#form_id'], $horizontal_forms)) {
    $element['#attributes']['class'][] = 'uk-form-horizontal';
  }
  else {
    $element['#attributes']['class'][] = 'uk-form-stacked';
  }

  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }

  element_set_attributes($element, array('method', 'id'));

  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }

  return '<form' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</form>';
}

/**
 * Implements theme_form_element().
 */
function uikit_form_element($variables) {
  $element = &$variables['element'];
  $name = !empty($element['#name']) ? $element['#name'] : FALSE;
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $prefix = isset($element['#field_prefix']) ? $element['#field_prefix'] : '';
  $suffix = isset($element['#field_suffix']) ? $element['#field_suffix'] : '';
  $checkbox = $type && $type === 'checkbox';
  $radio = $type && $type === 'radio';
  $select = $type && $type === 'select';

  // Create an attributes array for the wrapping container.
  if (empty($element['#wrapper_attributes'])) {
    $element['#wrapper_attributes'] = array();
  }

  $wrapper_attributes = &$element['#wrapper_attributes'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add wrapper ID for 'item' type.
  if ($type && $type === 'item' && !empty($element['#markup']) && !empty($element['#id'])) {
    $wrapper_attributes['id'] = $element['#id'];
  }

  // Add necessary classes to wrapper container.
  if ($name) {
    $wrapper_attributes['class'][] = 'form-item-' . drupal_html_class($name);
  }
  if ($type) {
    $wrapper_attributes['class'][] = 'form-type-' . drupal_html_class($type);
  }
  if (!empty($element['#attributes']['disabled'])) {
    $wrapper_attributes['class'][] = 'form-disabled';
  }
  if (!empty($element['#autocomplete_path']) && drupal_valid_path($element['#autocomplete_path'])) {
    $wrapper_attributes['class'][] = 'form-autocomplete';
  }
  if (!$select) {
    $wrapper_attributes['class'][] = 'uk-margin';
  }

  // TODO: Add advanced password options in theme settings.
  // Add a space before the labels of checkboxes and radios.
  if (($checkbox || $radio) && isset($element['#title'])) {
    $variables['element']['#title'] = ' ' . $element['#title'];
  }

  // Create a render array for the form element.
  $build = array(
    '#theme_wrappers' => array('container__form_element'),
    '#attributes' => $wrapper_attributes,
  );

  // Render the label for the form element.
  $build['label'] = array(
    '#markup' => theme('form_element_label', $variables),
  );

  // Increase the label weight if it should be displayed after the element.
  if ($element['#title_display'] === 'after') {
    $build['label']['#weight'] = 10;
  }

  // Checkboxes and radios render the input element inside the label. If the
  // element is neither of those, then the input element must be rendered here.
  if (!$checkbox && !$radio) {

    if ((!empty($prefix) || !empty($suffix))) {
      if (!empty($element['#field_prefix'])) {
        $prefix = '<span class="form-item-prefix">' . $prefix . '</span>';
      }
      if (!empty($element['#field_suffix'])) {
        $suffix = '<span class="form-item-suffix">' . $suffix . '</span>';
      }

      // Add a wrapping container around the elements.
      $prefix .= '<div>' . $prefix;
      $suffix .= '</div>';
    }

    // Build the form element.
    $build['element'] = array(
      '#markup' => $element['#children'],
      '#prefix' => !empty($prefix) ? $prefix : NULL,
      '#suffix' => !empty($suffix) ? $suffix : NULL,
    );
  }

  // Construct the element's description markup.
  if (!empty($element['#description'])) {
    $build['description'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'uk-text-small',
          'uk-text-muted',
          'uk-margin-small-top',
        ),
      ),
      '#weight' => 20,
      0 => array('#markup' => $element['#description']),
    );
  }

  // Print the form element build array.
  return drupal_render($build);
}

/**
 * Implements theme_form_element_label().
 */
function uikit_form_element_label($variables) {
  $element = $variables['element'];
  $display = isset($element['#title_display']) ? $element['#title_display'] : 'before';
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $checkbox = $type && $type === 'checkbox';
  $radio = $type && $type === 'radio';
  $attributes = array();

  if (!$checkbox && !$radio) {
    $attributes['class'][] = 'uk-form-label';
  }

  // Extract variables.
  $output = '';
  $title = !empty($element['#title']) ? filter_xss_admin($element['#title']) : '';

  // Only show the required marker if there is an actual title to display.
  if ($title && $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '') {
    $title .= ' ' . $required;
  }

  // Immediately return if the element is not a checkbox or radio and there is
  // no label to be rendered.
  if (!$checkbox && !$radio && ($display === 'none' || !$title)) {
    return '';
  }

  // Add the necessary 'for' attribute if the element ID exists.
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Checkboxes and radios must construct the label differently.
  if ($checkbox || $radio) {
    if ($display === 'before') {
      $output .= $title;
    }
    elseif ($display === 'none' || $display === 'invisible') {
      $output .= '<span class="uk-hidden">' . $title . '</span>';
    }

    // Inject the rendered checkbox or radio element inside the label.
    if (!empty($element['#children'])) {
      $output .= $element['#children'];
    }

    if ($display === 'after') {
      $output .= $title;
    }
  }
  // Otherwise, just render the title as the label.
  else {
    // Show label only to screen readers to avoid disruption in visual flows.
    if ($display === 'invisible') {
      $attributes['class'][] = 'uk-hidden';
    }

    $output .= $title;
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $output . '</label>';
}

/**
 * Implements theme_form_required_marker().
 */
function uikit_form_required_marker($variables) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();
  $attributes = array(
    'class' => array(
      'form-required',
      'uk-text-danger',
    ),
    'title' => $t('This field is required.'),
  );
  return '<sup' . drupal_attributes($attributes) . '>*</sup>';
}

/**
 * Implements theme_link().
 */
function uikit_link($variables) {
  $path = $variables['path'];
  $options = $variables['options'];
  $attributes = $variables['options']['attributes'];
  $text = $variables['options']['html'] ? $variables['text'] : check_plain($variables['text']);

  switch ($path) {
    case '<uk-nav-header>':
      return $text;

    case '<uk-parent>':
      return '<a href="#" ' . drupal_attributes($attributes) . '>' . $text . '</a>';

    case '<uk-nav-divider>':
      return '';

    default:
      return '<a href="' . check_plain(url($path, $options)) . '" ' . drupal_attributes($attributes) . '>' . $text . '</a>';
  }
}

/**
 * Implements theme_links().
 */
function uikit_links($variables) {
  global $language_url;
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  $output = '';

  if (count($links) > 0) {
    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }

      $output .= '<' . $heading['level'];

      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }

      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);

      // Add first, last and uk-active classes to the list of links to help out
      // themers.
      if ($i == 1) {
        $class[] = 'first';
      }

      if ($i == $num_links) {
        $class[] = 'last';
      }

      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
        && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'uk-active';
      }

      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for
        // adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }

        $span_attributes = '';

        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }

        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Implements theme_links() for node links.
 */
function uikit_links__node($variables) {
  $links = $variables['links'];
  $heading = $variables['heading'];
  $output = '';

  if (count($links) > 0) {
    // Treat the heading first if it is present to prepend it to the list of
    // links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading is a
        // string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul class="uk-subnav uk-subnav-divider">';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $link['attributes']['class'] = array(str_replace('_', '-', $key));

      // Add first and last classes to the list of links to help out themers.
      if ($i == 1) {
        $link['attributes']['class'][] = 'first';
      }

      if ($i == $num_links) {
        $link['attributes']['class'][] = 'last';
      }

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for
        // adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }

        $span_attributes = '';

        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }

        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
    }

    $output .= '</ul>';

  }

  return $output;
}

/**
 * Implements theme_menu_link().
 */
function uikit_menu_link(array $variables) {
  global $user;
  $element = $variables['element'];
  $sub_menu = '';

  $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();

  // Check plain title if "html" is not set, otherwise, filter for XSS attacks.
  $title = empty($options['html']) ? check_plain($element['#title']) : filter_xss_admin($element['#title']);

  // Ensure "html" is now enabled so l() doesn't double encode. This is now
  // safe to do since both check_plain() and filter_xss_admin() encode HTML
  // entities. See: https://www.drupal.org/node/2854978
  $options['html'] = TRUE;

  $href = $element['#href'];
  $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();

  $front_page = $href == '<front>' && drupal_is_front_page();
  $user_profile = $href === 'user' && $_GET['q'] === "user/$user->uid";

  if ($href == $_GET['q'] || $front_page || $user_profile) {
    $attributes['class'][] = 'uk-active';
  }

  // Remove expanded classes.
  if (isset($attributes['class']) && ($key = array_search('expanded', $attributes['class'])) !== FALSE) {
    unset($attributes['class'][$key]);
  }

  if ($element['#below']) {
    $offcanvas = in_array('menu_tree__offcanvas__below', $element['#below']['#theme_wrappers']);

    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ($offcanvas) {
      $sub_menu = drupal_render($element['#below']);

      // Generate as standard dropdown.
      $attributes['class'][] = 'uk-parent';
      $attributes['aria-expanded'] = 'false';
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Use our own theme wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<div class="uk-navbar-dropdown">';
      $sub_menu .= '<ul class="uk-nav uk-navbar-dropdown-nav">' . drupal_render($element['#below']) . '</ul>';
      $sub_menu .= '</div>';
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 2)) {
      // Use our own theme wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu .= '<ul>' . drupal_render($element['#below']) . '</ul>';
    }
  }

  switch ($href) {
    case '<uk-nav-header>':
      $attributes['class'][] = 'uk-nav-header';
      return '<li' . drupal_attributes($attributes) . '>' . $title . "</li>\n";

    case '<uk-nav-divider>':
      $attributes['class'][] = 'uk-nav-divider';
      return '<li' . drupal_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>\n";

    default:
      return '<li' . drupal_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>\n";
  }
}

/**
 * Implements theme_menu_local_action().
 */
function uikit_menu_local_action($variables) {
  $link = $variables['element']['#link'];
  $output = '<li class="uk-active">';

  if (isset($link['href'])) {
    $output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $link['title'];
  }
  else {
    $output .= check_plain($link['title']);
  }

  $output .= "</li>\n";

  return $output;
}

/**
 * Implements theme_menu_local_task().
 */
function uikit_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="uk-hidden">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }

    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));
  }

  return '<li' . (!empty($variables['element']['#active']) ? ' class="uk-active"' : '') . '>' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}

/**
 * Implements theme_menu_local_tasks().
 */
function uikit_menu_local_tasks($variables) {
  $output = '';
  $primary_attributes = $variables['primary_attributes'];
  $secondary_attributes = $variables['secondary_attributes'];

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="uk-hidden">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= "<ul$primary_attributes>";
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="uk-hidden">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= "<ul$secondary_attributes>";
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Implements theme_menu_tree().
 */
function uikit_menu_tree($variables) {
  return '<ul class="uk-list">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for the primary menu.
 */
function uikit_menu_tree__navbar($variables) {
  return '<ul class="uk-navbar-nav uk-visible@m">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for offcanvas menus.
 */
function uikit_menu_tree__offcanvas($variables) {
  return '<ul class="uk-nav uk-nav-default uk-nav-parent-icon" uk-nav>' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for offcanvas menus.
 */
function uikit_menu_tree__offcanvas__below($variables) {
  return '<ul class="uk-nav-sub" role="menu">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree() for shortcut sets.
 */
function uikit_menu_tree__shortcut_set($variables) {
  return '<ul class="menu">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_node_add_list().
 */
function uikit_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="uk-description-list-line">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output .= '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }

  return $output;
}

/**
 * Implements theme_pager().
 */
function uikit_pager($variables) {
  global $pager_page_array;
  global $pager_total;
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  $tags = $variables['tags'];

  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);

  // Current is the page we are currently paged to.
  $pager_current = $pager_page_array[$element] + 1;

  // First is the first page listed by this pager piece (re quantity).
  $pager_first = $pager_current - $pager_middle + 1;

  // Last is the last page listed by this pager piece (re quantity).
  $pager_last = $pager_current + $quantity - $pager_middle;

  // Max is the maximum page number.
  $pager_max = $pager_total[$element];

  // Prepare for generation loop.
  $i = $pager_first;

  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }

  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }

  $li_first = theme('pager_first', array(
    'text' => (isset($tags[0]) ? $tags[0] : t('First')),
    'element' => $element,
    'parameters' => $parameters,
  ));

  $li_previous = theme('pager_previous', array(
    'text' => (isset($tags[1]) ? $tags[1] : t('Previous')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));

  $li_next = theme('pager_next', array(
    'text' => (isset($tags[3]) ? $tags[3] : t('Next')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));

  $li_last = theme('pager_last', array(
    'text' => (isset($tags[4]) ? $tags[4] : t('Last')),
    'element' => $element,
    'parameters' => $parameters,
  ));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }

    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }

      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($pager_current - $i),
              'parameters' => $parameters,
            )),
          );
        }

        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current', 'uk-active'),
            'data' => '<span>' . $i . '</span>',
          );
        }

        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array(
              'text' => $i,
              'element' => $element,
              'interval' => ($i - $pager_current),
              'parameters' => $parameters,
            )),
          );
        }
      }

      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '<span>…</span>',
        );
      }
    }

    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }

    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }

    $pagination = theme('item_list', array(
      'items' => $items,
      'attributes' => array('class' => array('uk-pagination')),
      'pagination' => TRUE,
    ));

    return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . $pagination;
  }
}

/**
 * Implements theme_pager_link().
 */
function uikit_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;

    if (!isset($titles)) {
      $titles = array(
        t('First') => t('Go to first page'),
        t('Previous') => t('Go to previous page'),
        t('Next') => t('Go to next page'),
        t('Last') => t('Go to last page'),
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }

    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  return '<a' . drupal_attributes($attributes) . '>' . check_plain($text) . '</a>';
}

/**
 * Implements theme_password().
 */
function uikit_password($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'password';
  element_set_attributes($element, array('id', 'name', 'size', 'maxlength'));

  _form_set_class($element, array(
    'form-text',
    'uk-input',
    'uk-form-width-large',
  ));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_radio().
 */
function uikit_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array(
    'id',
    'name',
    '#return_value' => 'value',
  ));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio', 'uk-radio'));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Implements theme_select().
 */
function uikit_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));

  _form_set_class($element, array(
    'form-select',
    'uk-select',
  ));

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  return '<select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}

/**
 * Implements theme_status_messages().
 */
function uikit_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = 'uk-alert-';

    switch ($type) {
      case 'status':
        $class .= 'success';
        break;

      case 'error':
        $class .= 'danger';
        break;

      case 'warning':
        $class .= 'warning';
        break;

    }

    $output .= "<div id='messages' class=\"uk-alert $class\">\n";
    $output .= '<button class="uk-alert-close" type="button" uk-close></button>';

    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="uk-hidden">' . $status_heading[$type] . "</h2>\n";
    }

    if (count($messages) > 1) {
      $output .= " <ul>\n";

      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }

      $output .= " </ul>\n";
    }
    else {
      $output .= reset($messages);
    }

    $output .= "</div>\n";
  }

  return $output;
}

/**
 * Implements theme_table().
 */
function uikit_table($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }

  $output = '<div class="uk-overflow-auto">';
  $output .= '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column.
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup.
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';

        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }

        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;

    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }

    $rows[] = array(
      array(
        'data' => $empty,
        'colspan' => $header_count,
        'class' => array('empty', 'message'),
      ),
    );
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr class="uk-text-left">' : ' <tr>');

    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }

    // Using ternary operator to close the tags based on whether or not there
    // are rows.
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';

    foreach ($rows as $row) {
      // Check if we're dealing with a simple or complex row.
      if (isset($row['data'])) {
        $cells = $row['data'];
        $no_striping = isset($row['no_striping']) ? $row['no_striping'] : FALSE;

        // Set the attributes array and exclude 'data' and 'no_striping'.
        $attributes = $row;
        unset($attributes['data']);
        unset($attributes['no_striping']);
      }
      else {
        $cells = $row;
        $attributes = array();
        $no_striping = FALSE;
      }

      if (count($cells)) {
        // Add odd/even class.
        if (!$no_striping) {
          $class = $flip[$class];
          $attributes['class'][] = $class;
        }

        $attributes['class'][] = 'uk-text-left';

        // Build row.
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;

        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  $output .= "</table>\n";
  $output .= "</div>\n";
  return $output;
}

/**
 * Implements theme_textarea().
 */
function uikit_textarea($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea', 'uk-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    drupal_add_library('system', 'drupal.textarea');
    $wrapper_attributes['class'][] = 'resizable';
  }

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements theme_textfield().
 */
function uikit_textfield($variables) {
  $element = $variables['element'];

  element_set_attributes($element, array(
    'id',
    'name',
    'value',
    'size',
    'maxlength',
  ));

  _form_set_class($element, array(
    'form-text',
    'uk-input',
    'uk-form-width-large',
  ));

  $element['#attributes']['type'] = 'text';
  $extra = '';

  if ($element['#autocomplete_path'] && !empty($element['#autocomplete_input'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#autocomplete_input']['#id'];
    $attributes['value'] = $element['#autocomplete_input']['#url_value'];
    $attributes['disabled'] = 'disabled';
    $attributes['class'][] = 'autocomplete';
    $extra = '<input' . drupal_attributes($attributes) . ' />';
  }

  // Check for errors and set correct error class.
  if (form_get_error($element)) {
    $element['#attributes']['class'][] = 'uk-form-danger';
  }

  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  return $output . $extra;
}
