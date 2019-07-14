/**
 * @file
 * Attaches the calendar behavior to all date-popup-enabled fields
 */

(function ($) {
    'use strict';
    function sett(minDate, maxDate, hasDay, hasMonth, calType, hasInit) {
        return {
            format: 'L',
            responsive: true,
            persianDigit: false,
            viewMode: hasDay ? 'day' : (hasMonth ? 'month' : 'year'),
            initialValue: hasInit,
            initialValueType: 'persian',
            minDate: minDate,
            maxDate: maxDate,
            autoClose: true,
            position: "auto",
            calendarType: calType,
            inputDelay: 800,
            observer: true,
            calendar: {
                persian: {
                    locale: "en",
                    showHint: true,
                    leapYearMode: "algorithmic"
                },
                gregorian: {
                    locale: "en",
                    showHint: true
                }
            },
            navigator: {
                enabled: true,
                scroll: {
                    enabled: true
                },
                text: {
                    btnNextText: "<",
                    btnPrevText: ">"
                }
            },
            toolbox: {
                enabled: true,
                calendarSwitch: {
                    enabled: false,
                    format: "MMMM"
                },
                todayButton: {
                    enabled: true,
                    text: {
                        fa: "امروز",
                        en: "Today"
                    }
                },
                submitButton: {
                    enabled: true,
                    text: {
                        fa: "تایید",
                        en: "Submit"
                    }
                },
                text: {
                    btnToday: "امروز"
                }
            },
            timePicker: {
                enabled: false,
            },
            dayPicker: {
                enabled: hasDay,
                titleFormat: "YYYY MMMM"
            },
            monthPicker: {
                enabled: hasMonth,
                titleFormat: "YYYY"
            },
            yearPicker: {
                enabled: true,
                titleFormat: "YYYY"
            },
        };
    }

    function time_sett(settings, has_minute) {
        return {
            responsive: true,
            persianDigit: false,
            initialValue: false,
            autoClose: true,
            position: "auto",
            onlyTimePicker: true,
            calendarType: 'gregorian',
            inputDelay: 800,
            observer: true,
            navigator: {
                enabled: false,
                scroll: {
                    enabled: true
                },
                text: {
                    btnNextText: "<",
                    btnPrevText: ">"
                }
            },
            toolbox: {
                enabled: true,
                calendarSwitch: {
                    enabled: false,
                },
                todayButton: {
                    enabled: false,
                },
                submitButton: {
                    enabled: true,
                },
            },
            timePicker: {
                enabled: true,
                hour: {
                    enabled: true,
                    step: settings.timeSteps[0]
                },
                minute: {
                    enabled: has_minute,
                    step: settings.timeSteps[1]
                },
                second: {
                    enabled: settings.showSeconds,
                    step: settings.timeSteps[2]
                },
                meridian: {
                    enabled: false
                }
            },
            format: settings.showSeconds ? 'H:m:s' : (has_minute ? 'H:m' : 'H'),
        };
    }

    function process_date(who, settings, id) {
        var min = parseInt(who.attr('data-datex-min'));
        var max = parseInt(who.attr('data-datex-max'));
        var hd = who.attr('data-datex-has-day') === "1";
        var hm = who.attr('data-datex-has-month') === "1";
        var init = who.attr('data-datex-has-init') === "1";
        var tp = who.attr('data-datex-tp');
        var cfg = sett(min, max, hd, hm, tp, init);
        var pd = who.pDatepicker(cfg);
    }

    function process_time(who, settings, id) {
        var hasMin = who.attr('data-datex-has-minute') === "1";
        var cfg = time_sett(settings, hasMin);
        var pd = who.pDatepicker(cfg);
        // var init = who.attr('data-datex-init');
    }

    Drupal.behaviors.datex = {
        attach: function (ctx) {
            for (var id in Drupal.settings.datePopup) {
                if (!Drupal.settings.datePopup.hasOwnProperty(id)) {
                    continue;
                }
                var data = Drupal.settings.datePopup[id];
                var find = $('#' + id);
                if (data.func === 'datepicker') {
                    process_date(find, data.settings, id)
                }
                else {
                    process_time(find, data.settings, id)
                }
            }
        }
    };

})(jQuery);
