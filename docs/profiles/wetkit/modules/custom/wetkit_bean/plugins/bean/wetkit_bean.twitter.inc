<?php
/**
 * @file
 * Listing bean plugin.
 */

class WetKitTwitterBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array(
      'twitter_settings' => array(
        'configure_twitter_widget' => 'col-md-12',
        'custom_css' => '',
        'widget_type' => '',
        'username' => '',
        'search_query' => '',
        'widget_id' => '',
        'tweet_limit' => '',
        'widget_height' => '',
      ),
    );

    return array_merge(parent::values(), $values);
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['twitter_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure Twitter Widget'),
      '#tree' => TRUE,
      '#weight' => -8,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['twitter_settings']['configure_twitter_widget'] = array(
      '#type' => 'select',
      '#title' => t('Configure Share Span'),
      '#options' => array(
        'col-md-1' => t('Medium 1'),
        'col-md-2' => t('Medium 2'),
        'col-md-3' => t('Medium 3'),
        'col-md-4' => t('Medium 4'),
        'col-md-5' => t('Medium 5'),
        'col-md-6' => t('Medium 6'),
        'col-md-7' => t('Medium 7'),
        'col-md-8' => t('Medium 8'),
        'col-md-9' => t('Medium 9'),
        'col-md-10' => t('Medium 10'),
        'col-md-11' => t('Medium 11'),
        'col-md-12' => t('Medium 12'),
      ),
      '#default_value' => isset($bean->twitter_settings['configure_twitter_widget']) ? $bean->twitter_settings['configure_twitter_widget'] : '',
    );
    $form['twitter_settings']['custom_css'] = array(
      '#type' => 'textfield',
      '#title' => t('Configure Twitter Custom CSS'),
      '#default_value' => isset($bean->twitter_settings['custom_css']) ? $bean->twitter_settings['custom_css'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );
    $form['twitter_settings']['widget_type'] = array(
      '#type' => 'select',
      '#title' => t('Twitter widget type'),
      '#description' => t('Shows the user\'s most recent tweets.'),
      '#options' => array(
        'profile' => t('Profile'),
        'search' => t('Search'),
      ),
      '#default_value' => isset($bean->twitter_settings['widget_type']) ? $bean->twitter_settings['widget_type'] : '',
      '#required' => TRUE,
    );
    $form['twitter_settings']['username'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter user name: @'),
      '#size' => 25,
      '#maxlength' => 25,
      '#default_value' => isset($bean->twitter_settings['username']) ? $bean->twitter_settings['username'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="twitter_settings[widget_type]"]' => array('value' => 'profile'),
        ),
      ),
    );
    $form['twitter_settings']['search_query'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter search query'),
      '#size' => 25,
      '#maxlength' => 25,
      '#default_value' => isset($bean->twitter_settings['search_query']) ? $bean->twitter_settings['search_query'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="twitter_settings[widget_type]"]' => array('value' => 'search'),
        ),
      ),
    );
    $form['twitter_settings']['widget_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter widget ID'),
      '#description' => t('Widget ID provided by twitter. The ID is required for the widget to work and must be created on https://twitter.com/settings/widgets'),
      '#size' => 80,
      '#maxlength' => 80,
      '#default_value' => isset($bean->twitter_settings['widget_id']) ? $bean->twitter_settings['widget_id'] : '',
      '#required' => TRUE,
    );

    $form['twitter_settings']['tweet_limit'] = array(
      '#type' => 'select',
      '#title' => t('Tweet limit'),
      '#default_value' => isset($bean->twitter_settings['tweet_limit']) ? $bean->twitter_settings['tweet_limit'] : '',
      '#options' => array('' => drupal_map_assoc(range(1, 20))),
      '#description' => t('Fix the size of a timeline to a preset number of Tweets between 1 and 20. The timeline will render the specified number of Tweets from the timeline, expanding the height of the widget to display all Tweets without scrolling.'),
    );
    
    $form['twitter_settings']['widget_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Widget Height'),
      '#default_value' => isset($bean->twitter_settings['widget_height']) ? $bean->twitter_settings['widget_height'] : '',
      '#description' => t('This is where you would select the height of your twitter widget. If widget height is selected, your tweet limit will NOT work. '),
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {

    // Settings.
    $span = isset($bean->twitter_settings['configure_twitter_widget']) ? $bean->twitter_settings['configure_twitter_widget'] : '';
    $custom_css = isset($bean->twitter_settings['custom_css']) ? $bean->twitter_settings['custom_css'] : '';
    $widget_id = isset($bean->twitter_settings['widget_id']) ? $bean->twitter_settings['widget_id'] : '';
    $widget_type = isset($bean->twitter_settings['widget_type']) ? $bean->twitter_settings['widget_type'] : 'search';
    $username = isset($bean->twitter_settings['username']) ? $bean->twitter_settings['username'] : '';
    $search_query = isset($bean->twitter_settings['search_query']) ? $bean->twitter_settings['search_query'] : '';
    $tweet_limit = isset($bean->twitter_settings['tweet_limit']) ? 'data-tweet-limit="' . $bean->twitter_settings['tweet_limit'] . '"' : '';
    $widget_height = isset($bean->twitter_settings['widget_height']) ? $bean->twitter_settings['widget_height'] : '';

    // Twitter method.
    $url = "https://twitter.com/";
    switch ($widget_type) {
      case 'profile':
        $url .= $username;
      case 'search':
        $url .= 'search?q=' . $search_query;
    }

    // Title field.
    $title = field_get_items('bean', $bean, 'title_field');
    $title = field_view_value('bean', $bean, 'title_field', $title[0], array());
    $title = render($title);

    // Reset content.
    $content = array();

    // Rendered markup.
    $markup = '<section><div class="wb-twitter ' . $custom_css . ' ' . $span . '">';
    $markup .= (empty($title)) ? '' : '<h2>' . $title . '</h2>' ;
    $markup .= '<a class="twitter-timeline" height="' . $widget_height . '" href="' . $url . '" data-widget-id="' . $widget_id . '" ';
    $markup .= (empty($widget_height)) ? $tweet_limit :  '';
    $markup .= '>Tweets</a>';
    $markup .= '</div></section>';

    $content['share_twitter']['#markup'] = $markup;

    return $content;
  }
}
