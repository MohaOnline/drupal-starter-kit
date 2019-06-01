(function($) {
  window.Shareaholic = window.Shareaholic || {};
  window.shareaholic_debug = true;

    Shareaholic.bind_button_clicks = function (click_object, off) {
        if (off) {
            $(click_object.selector).off('click.app_settings');
        }

        $(click_object.selector).off('click.app_settings').on('click.app_settings', function (e) {
            var button = this;
            e.preventDefault();
            url = click_object.url(this);
            if (click_object.selector == '#general_settings') {
                window.open(url);
                return false;
            } else {
                $frame = $('<iframe>', { src:url }).appendTo('#iframe_container');
                if (click_object.callback) {
                    click_object.callback(this);
                }
                $('#editing_modal').reveal({
                    topPosition:90,
                    close:function () {
                        if (click_object.close) {
                            click_object.close(button);
                        }
                        $frame.remove();
                    }
                });
            }
        });
    }

    Shareaholic.click_objects = {
        'app_settings': {
            selector: '#app_settings button',
            url: function(button) {
                id = $(button).data('location_id');
                app = $(button).data('app')
                url = first_part_of_url + $(button).data('href') + '?embedded=true&'
                    + 'verification_key=' + verification_key;
                url = url.replace(/{{id}}/, id);
                return url;
            },
            callback: function(button) {
                $modal = $('.reveal-modal');
                $modal.addClass('has-shortcode');
            },
            close: function(button) {
                $('#shortcode_container').remove();
                $('.reveal-modal').removeClass('has-shortcode');
            }
        },

        'general_settings': {
            selector: '#general_settings',
            url: function(button) {
                return first_part_of_url + 'websites/edit/'
                    + '?verification_key=' + verification_key;
            }
        },

        'app_wide_settings': {
            selector: '.app_wide_settings',
            url: function(button) {
                url = first_part_of_url + $(button).data('href') + '?embedded=true&'
                    + 'verification_key=' + verification_key;
                return url
            }
        }
    }

    Shareaholic.titlecase = function(string) {
        return string.charAt(0).toUpperCase() + string.replace(/_[a-z]/g, function(match) {
            return match.toUpperCase().replace(/_/, ' ');
        }).slice(1);
    }

    Shareaholic.disable_buttons = function() {
        $('#app_settings button').each(function() {
            if (!$(this).data('location_id')  && !this.id == 'app_wide_settings') {
                $(this).attr('disabled', 'disabled');
            } else {
                $(this).removeAttr('disabled');
            }
        });
    }

   Shareaholic.Utils.PostMessage.receive('settings_saved', {
       success: function(data) {
           $('input[type="submit"]').click();
       },
       failure: function(data) {
           console.log(data);
       }
   });

    Shareaholic.create_new_location = function($input) {
      var $button = $input.siblings('button').eq(0);
      var app = $button.data('app');
      var location_id = $button.data('location_id');
      var location_name = /.*\[(.*)\]/.exec($input.attr('name'))[1];
      var data = {};
      if (location_id) {
        return;
      }
      $input.prop('disabled', true);
      data['configuration_' + app + '_location'] = {
        name: location_name
      };

      $button.text('Creating...');

      $.ajax({
        url: first_part_of_url + app + '/locations.json',
        type: 'POST',
        data: data,
        success: function(data, status, jqxhr) {
          Shareaholic.new_location_success_callback($input, $button, app, location_name, data['location']['id']);
        },
        error: function() {
          Shareaholic.get_publisher_configurations($input, $button, app, location_name);
        },
        xhrFields: {
          withCredentials: true
        }
      });
    }

    Shareaholic.new_location_success_callback = function($input, $button, app, location_name, location_id) {
      $button.data('location_id', location_id);
      $button.text('Customize');
      Shareaholic.disable_buttons();
      $input.prop('disabled', false);
      $('#' + app + '_' + location_name + '_location_id').val(location_id);
    };

    Shareaholic.get_publisher_configurations = function($input, $button, app, location_name) {
      Shareaholic.dispatcher.add_once('on_load_publisher_configuration', function() {
        var locations = Shareaholic.publisher_configuration.apps[app].locations;
        var isFound = false;
        $.each(locations, function(location_id, location) {
          console.log(location);
          console.log(location_name);
          if(location.location_id && location.name && location.name === location_name) {
            Shareaholic.new_location_success_callback($input, $button, app, location_name, location_id);
            isFound = true;
            return;
          }
        });
        if(!isFound) {
          $button.text('Creation Failed');
          $input.prop('disabled', false);
        }
      });
      Shareaholic.SDK.load_publisher_configuration(window.shareaholic_api_key);
    };

  $(document).ready(function() {
    Shareaholic.disable_buttons();

    Shareaholic.bind_button_clicks(Shareaholic.click_objects['app_settings']);
    Shareaholic.bind_button_clicks(Shareaholic.click_objects['general_settings']);
    Shareaholic.bind_button_clicks(Shareaholic.click_objects['app_wide_settings']);
    if (Shareaholic.click_objects['unverified_general_settings']) {
        Shareaholic.bind_button_clicks(Shareaholic.click_objects['unverified_general_settings'], true);
    }

    $('#terms_of_service_modal').reveal({
      closeonbackgroundclick: false,
      closeonescape: false,
      topPosition: 90
    });

    $('#failed_to_create_api_key').reveal({
      closeonbackgroundclick: false,
      closeonescape: false,
      topPosition: 90
    });

    $('input[type=checkbox]').click(function() {
      var $input = $(this);
      if($input.is(':checked')) {
        Shareaholic.create_new_location($input);
      }
    });

  });
})(sQuery);
