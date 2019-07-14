(function ($) {
    'use strict';

    // damn...

    function sett(minDate, maxDate) {
        var calType = 'persian';
        var hasInit = false;
        return {
            format: 'L',
            responsive: true,
            persianDigit: false,
            viewMode: 'day',
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
                enabled: true,
                titleFormat: "YYYY MMMM"
            },
            monthPicker: {
                enabled: true,
                titleFormat: "YYYY"
            },
            yearPicker: {
                enabled: true,
                titleFormat: "YYYY"
            },
        };
    }

    $.fn.datepicker = function (cfg) {
        var zelf = this;
        if (this.context.className.indexOf('webform') < 0) {
            return;
        }
        var set = sett(cfg.minDate.getTime(), cfg.maxDate.getTime());
        set.onSelect = function (select) {
            var y = $($(zelf.context).find('select.year'));
            var m = $($(zelf.context).find('select.month'));
            var d = $($(zelf.context).find('select.day'));
            var pd = new persianDate(select);
            y.val(pd.year());
            m.val(pd.month());
            d.val(pd.date());
            y.trigger('change');
            m.trigger('change');
            d.trigger('change');
        };
        set.onShow = function (a) {
        }
        this.pDatepicker(set);
    }
})(jQuery);
