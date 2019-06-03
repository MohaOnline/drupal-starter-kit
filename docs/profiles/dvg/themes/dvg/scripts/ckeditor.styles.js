/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/*
 * This file is used/requested by the 'Styles' button.
 * The 'Styles' button is not enabled by default in DrupalFull and DrupalFiltered toolbars.
 */
if(typeof(CKEDITOR) !== 'undefined') {
  CKEDITOR.addStylesSet( 'drupal',
    [
      {
        name : 'Kop 1',
        element : 'h2',
        attributes :
          {
            'class' : ''
          }
      },
      {
        name : 'Kop 2',
        element : 'h3',
        attributes :
          {
            'class' : ''
          }
      },
      {
        name : 'Quote',
        element : 'p',
        attributes :
          {
            'class' : 'quote'
          }
      },
      {
        name : 'Attentie blok',
        element : 'p',
        attributes :
          {
            'class' : 'attention'
          }
      },
      {
        name : 'Attentie blok - groot',
        element : 'p',
        attributes :
          {
            'class' : 'attention big'
          }
      },
      {
        name : 'Normaal',
        element : 'p',
        attributes :
          {
            'class' : ''
          }
      },
      {
        name : 'Button',
        element : 'a',
        attributes :
          {
            'class' : 'btn'
          }
      }
    ]
  );
}
