// This module is used to provide the Drupal global in development and test mode.
// Functions taken from drupal.js.

const Drupal = {
  settings: {
    campaignion_email_to_target: {
      contactPrefix: 'contact.',
      standardColumns: [
        {
          key: 'email',
          description: '',
          title: 'Email address'
        },
        {
          key: 'title',
          description: '',
          title: 'Title'
        },
        {
          key: 'first_name',
          description: '',
          title: 'First name'
        },
        {
          key: 'last_name',
          description: '',
          title: 'Last name'
        },
        {
          key: 'salutation',
          description: 'Full name and titles',
          title: 'Salutation'
        }
      ],
      validations: {
        'email': '^[^\\s@]+@[^\\s@]+\\.[^\\s@]+$', // backslashes have to be escaped!
        'first_name': '\\S+',
        'last_name': '\\S+',
        'salutation': '\\S+'
      },
      maxFieldLengths: {
        'email': 255,
        'title': 255,
        'first_name': 255,
        'last_name': 255,
        'salutation': 255,
        'display_name': 255,
        'group': 255
      },
      endpoints: {
        'e2t-api': {
          url: process.env.E2T_API_URL, // url injected by webpack.DefinePlugin
          token: process.env.E2T_API_TOKEN // token injected by webpack.DefinePlugin
        }
      }
    }
  },

  locale: {},

  /**
   * Encode special characters in a plain-text string for display as HTML.
   *
   * @ingroup sanitization
   */
  checkPlain: function (str) {
    var character, regex,
        replace = { '&': '&amp;', '"': '&quot;', '<': '&lt;', '>': '&gt;' };
    str = String(str);
    for (character in replace) {
      if (replace.hasOwnProperty(character)) {
        regex = new RegExp(character, 'g');
        str = str.replace(regex, replace[character]);
      }
    }
    return str;
  },

  /**
   * Replace placeholders with sanitized values in a string.
   *
   * @param str
   *   A string with placeholders.
   * @param args
   *   An object of replacements pairs to make. Incidences of any key in this
   *   array are replaced with the corresponding value. Based on the first
   *   character of the key, the value is escaped and/or themed:
   *    - !variable: inserted as is
   *    - @variable: escape plain text to HTML (Drupal.checkPlain)
   *    - %variable: escape text and theme as a placeholder for user-submitted
   *      content (checkPlain + Drupal.theme('placeholder'))
   *
   * @see Drupal.t()
   * @ingroup sanitization
   */
  formatString: function(str, args) {
    // Transform arguments before inserting them.
    for (var key in args) {
      switch (key.charAt(0)) {
        // Escaped only.
        case '@':
          args[key] = Drupal.checkPlain(args[key]);
        break;
        // Pass-through.
        case '!':
          break;
        // Escaped and placeholder.
        case '%':
        default:
          args[key] = Drupal.theme('placeholder', args[key]);
          break;
      }
      str = str.replace(key, args[key]);
    }
    return str;
  },

  /**
   * Translate strings to the page language or a given language.
   *
   * See the documentation of the server-side t() function for further details.
   *
   * @param str
   *   A string containing the English string to translate.
   * @param args
   *   An object of replacements pairs to make after translation. Incidences
   *   of any key in this array are replaced with the corresponding value.
   *   See Drupal.formatString().
   *
   * @param options
   *   - 'context' (defaults to the empty context): The context the source string
   *     belongs to.
   *
   * @return
   *   The translated string.
   */
  t: function (str, args, options) {
    options = options || {};
    options.context = options.context || '';

    // Fetch the localized version of the string.
    if (Drupal.locale.strings && Drupal.locale.strings[options.context] && Drupal.locale.strings[options.context][str]) {
      str = Drupal.locale.strings[options.context][str];
    }

    if (args) {
      str = Drupal.formatString(str, args);
    }
    return str;
  },

  /**
   * Format a string containing a count of items.
   *
   * This function ensures that the string is pluralized correctly. Since Drupal.t() is
   * called by this function, make sure not to pass already-localized strings to it.
   *
   * See the documentation of the server-side format_plural() function for further details.
   *
   * @param count
   *   The item count to display.
   * @param singular
   *   The string for the singular case. Please make sure it is clear this is
   *   singular, to ease translation (e.g. use "1 new comment" instead of "1 new").
   *   Do not use @count in the singular string.
   * @param plural
   *   The string for the plural case. Please make sure it is clear this is plural,
   *   to ease translation. Use @count in place of the item count, as in "@count
   *   new comments".
   * @param args
   *   An object of replacements pairs to make after translation. Incidences
   *   of any key in this array are replaced with the corresponding value.
   *   See Drupal.formatString().
   *   Note that you do not need to include @count in this array.
   *   This replacement is done automatically for the plural case.
   * @param options
   *   The options to pass to the Drupal.t() function.
   * @return
   *   A translated string.
   */
  formatPlural: function (count, singular, plural, args, options) {
    var args = args || {};
    args['@count'] = count;
    // Determine the index of the plural form.
    var index = Drupal.locale.pluralFormula ? Drupal.locale.pluralFormula(args['@count']) : ((args['@count'] == 1) ? 0 : 1);

    if (index == 0) {
      return Drupal.t(singular, args, options);
    }
    else if (index == 1) {
      return Drupal.t(plural, args, options);
    }
    else {
      args['@count[' + index + ']'] = args['@count'];
      delete args['@count'];
      return Drupal.t(plural.replace('@count', '@count[' + index + ']'), args, options);
    }
  },

  /**
   * Generate the themed representation of a Drupal object.
   *
   * All requests for themed output must go through this function. It examines
   * the request and routes it to the appropriate theme function. If the current
   * theme does not provide an override function, the generic theme function is
   * called.
   *
   * For example, to retrieve the HTML for text that should be emphasized and
   * displayed as a placeholder inside a sentence, call
   * Drupal.theme('placeholder', text).
   *
   * @param func
   *   The name of the theme function to call.
   * @param ...
   *   Additional arguments to pass along to the theme function.
   * @return
   *   Any data the theme function returns. This could be a plain HTML string,
   *   but also a complex object.
   */
  theme: function (func) {
    var args = Array.prototype.slice.apply(arguments, [1]);

    return (Drupal.theme[func] || Drupal.theme.prototype[func]).apply(this, args);
  }
}

export default Drupal
