<?php

/**
 * Override or insert variables into the maintenance page template.
 */
function admintheme_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // admintheme_preprocess_html() also happen on the maintenance page, it has to be
  // called here.
  admintheme_preprocess_html($vars);
}

/**
 * Override or insert variables into the html template.
 */
function admintheme_preprocess_html(&$vars) {

    // Get theme folder path.
  $theme_path = drupal_get_path('theme', 'admintheme') . "/css";



  // Add conditional CSS for IE8 and below.
  drupal_add_css($theme_path . '/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'weight' => 999, 'preprocess' => FALSE));
  // Add conditional CSS for IE7 and below.
  drupal_add_css($theme_path . '/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'weight' => 999, 'preprocess' => FALSE));
  // Add conditional CSS for IE6.
  drupal_add_css($theme_path . '/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'weight' => 999, 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template.
 */
function admintheme_preprocess_page(&$vars) {


  $vars['primary_local_tasks'] = $vars['tabs'];
  unset($vars['primary_local_tasks']['#secondary']);
  $vars['secondary_local_tasks'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
}

/**
 * Display the list of available node types for node creation.
 */
function admintheme_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="clearfix">';
      $output .= '<span class="label">' . l($item['title'], $item['href'], $item['localized_options']) . '</span>';
      $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}

/**
 * Overrides theme_admin_block_content().
 *
 * Use unordered list markup in both compact and extended mode.
 */
function admintheme_admin_block($variables) {

  $block = $variables['block'];
  $output = '';

  // Don't display the block if it has no content to display.
  if (empty($block['show'])) {
    return $output;
  }

  if (!empty($block['path'])) {
    $output .= '<div class="admin-panel col-xs-12 ' . check_plain(str_replace("/", " ", $block['path'])) . ' ">';
  }
  elseif (!empty($block['title'])) {
    $output .= '<div class="admin-panel col-xs-12 ' . check_plain(strtolower($block['title'])) . '">';
  }
  else {
    $output .= '<div class="admin-panel col-xs-12">';
  }

  if (!empty($block['title'])) {
    $output .= '<h3 class="title">' . $block['title'] . '</h3>';
  }

  if (!empty($block['content'])) {
    $output .= '<div class="body">' . $block['content'] . '</div>';
  }
  else {
    $output .= '<div class="description">' . $block['description'] . '</div>';
  }

  $output .= '</div>';

  return $output;
}
function admintheme_admin_block_content($variables) {

  $content = $variables['content'];
  $output = '';
  if (!empty($content)) {
    $output = system_admin_compact_mode() ? '<ul class="admin-list compact">' : '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="leaf">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (isset($item['description']) && !system_admin_compact_mode()) {
        $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Override of theme_tablesort_indicator().
 *
 * Use our own image versions, so they show up as black and not gray on gray.
 */
function admintheme_tablesort_indicator($variables) {
  $style = $variables['style'];
  $theme_path = drupal_get_path('theme', 'admintheme');
  if ($style == 'asc') {
    return theme('image', array('path' => $theme_path . '/images/arrow-asc.png', 'alt' => t('sort ascending'), 'width' => 13, 'height' => 13, 'title' => t('sort ascending')));
  }
  else {
    return theme('image', array('path' => $theme_path . '/images/arrow-desc.png', 'alt' => t('sort descending'), 'width' => 13, 'height' => 13, 'title' => t('sort descending')));
  }
}

/**
 * Implements hook_css_alter().
 */
function admintheme_css_alter(&$css) {
  if (isset($css['misc/ui/jquery.ui.theme.css'])) {
    $css['misc/ui/jquery.ui.theme.css']['data'] = drupal_get_path('theme', 'admintheme') . '/jquery.ui.theme.css';
    $css['misc/ui/jquery.ui.theme.css']['type'] = 'file';
  }
}
function admintheme_menu_tree($variables) {
 $menu_name=$variables['theme_hook_original'];

   $menu_name=$variables['theme_hook_original'];
    switch($menu_name)
    {

     case 'menu_tree__management':

         $output = '<ul id="management-menu" class="sidebar-menu">'. $variables['tree'] .'</ul>';
       break;
      default:
        $output = '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
    }
  return $output;
}
function admintheme_menu_link(array $variables) {

  global $base_url;

  $menu_name = $variables['theme_hook_original'];

  $element = $variables['element'];
  $sub_menu = '';


if ( current_path() == $element['#href']) {
    $element['#attributes']['class'][] = 'submenu-active treeview menu-' . $element['#original_link']['mlid'];

  } else {

    $element['#attributes']['class'][] = 'treeview menu-' . $element['#original_link']['mlid'];
  }



    if ($element['#below']) {
      $sub_menu = drupal_render($element['#below']);
    }

    $test = '<span>' .$element['#title'] .'</span>';

    $full_menu_url = $base_url ."/" .$element['#href'];
    $menu_extra_class = "admin_menu_" . strtolower($element['#title']) ." ". $element['#original_link']['mlid'];

    $menu_title_html = '<i class="fa '. $menu_extra_class .' "></i><span>' . $element['#title'] . '</span>';
    $output = l($menu_title_html, $element['#href'], array('html' => TRUE));



    return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/*******************Form Alter Hook******************************/

function admintheme_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'node_admin_content') {

    $form['filter']['filters']['status']['filters']['status']['#attributes']['class'][] = 'form-control';
    $form['filter']['filters']['status']['filters']['type']['#attributes']['class'][] = 'form-control';
    $form['filter']['filters']['status']['actions']['submit']['#attributes']['class'][] = 'btn btn-primary';
    $form['admin']['options']['#attributes']['class'][0] = 'content-list';
    $form['admin']['options']['operation']['#attributes']['class'][0] = 'form-control';
    $form['admin']['options']['submit']['#attributes']['class'][0] = 'btn btn-primary';
    $form['admin']['nodes']['#attributes']['class'][0] = "";
    $form['filter']['filters']['status']['filters']['#attributes']['class'][] = "col-xs-12";
    $form['filter']['filters']['status']['actions']['#attributes']['class'][] = "col-xs-12";
  }

}
function admintheme_table($variables) {

  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'table table-bordered table-striped responsive resourceTable dataTable no-footer tb-responsive';
  }

  $output = '<div class="table-responsive">';
  $output .= '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column
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

      // Build colgroup
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
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
        ++$header_count;
      }
    }
    $rows[] = array(array(
      'data' => $empty,
      'colspan' => $header_count,
      'class' => array('empty', 'message'),
    ));
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there are rows
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array(
      'even' => 'odd',
      'odd' => 'even',
    );
    $class = 'even';
    foreach ($rows as $number => $row) {
      // Check if we're dealing with a simple or complex row
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
        // Add odd/even class
        if (!$no_striping) {
          $class = $flip[$class];
          $attributes['class'][] = $class;
        }

        // Build row
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

/*********** theme_pager**************/
function admintheme_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);

  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;


  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;

  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;

  // max is the maximum page number
  $pager_max = $pager_total[$element];

  // End of marker calculations.
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

  // End of generation loop preparation.
  $li_first = theme('pager_first', array(
    'text' => isset($tags[0]) ? $tags[0] : t('First'),
    'element' => $element,
    'parameters' => $parameters,
  ));
  $li_previous = theme('pager_previous', array(
    'text' => isset($tags[1]) ? $tags[1] : t('Previous'),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));
  $li_next = theme('pager_next', array(
    'text' => isset($tags[3]) ? $tags[3] : t('Next'),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters,
  ));
  $li_last = theme('pager_last', array(
    'text' => isset($tags[4]) ? $tags[4] : t('Last'),
    'element' => $element,
    'parameters' => $parameters,
  ));
  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array(
          'pager-first paginate_button ',
        ),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array(
          'pager-previous paginate_button',
        ),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array(
            'pager-ellipsis',
          ),
          'data' => '…',
        );
      }

      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array(
              'pager-item',
            ),
            'data' => theme('pager_previous', array(
              'text' => $i,
              'element' => $element,
              'interval' => $pager_current - $i,
              'parameters' => $parameters,
            )),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array(
              'pager-current paginate_button active',
            ),
            'data' =>'<a href="#"  data-dt-idx="' . $i . '" tabindex="' . $i . '">' . $i . '</a>',
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array(
              'pager-item',
            ),
            'data' => theme('pager_next', array(
              'text' => $i,
              'element' => $element,
              'interval' => $i - $pager_current,
              'parameters' => $parameters,
            )),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array(
            'pager-ellipsis',
          ),
          'data' => '…',
        );
      }
    }

    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array(
          'pager-next paginate_button',
        ),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array(
          'pager-last',
        ),
        'data' => $li_last,
      );
    }
    return '<h2 class="element-invisible">' . t('Pages') . '</h2> <div class="text-center">' . theme('item_list', array(
      'items' => $items,
      'attributes' => array(
        'class' => array(
          'pagination',
        ),
      ),
    )) . "</div>";
  }
}
