
Flux is a spacious, style neutral theme that is easily branded with your custom
logo and artwork. It is the default theme that ships with the Pushtape music
distribution.
See: http://drupal.org/project/pushtape

This theme is built off of the minimal Framework theme to include button styles,
fluid width % columns, and a single mobile friendly media query.
See: http://www.drupal.org/project/framework

Note that this theme is intended to be as barebones as possible. By design, things
like LESS/SASS, JS libraries, CSS grids and other bells and whistles are excluded.
It is recommended you use the Omega theme if you'd like more configuration options for your theme.
See: http://www.drupal.org/project/omega


--  This theme has a couple of unique features:
  * A large background image that covers the entire background
  * Button styles
  * A single media query for small screens
  * Fluid width % sidebars  

-- Background Image
  * Navigate to admin/appearance
  * Click settings for the Flux theme.
  * Toggle Display should be checked for "Cover Photo"
  * Scroll down to "Cover photo image settings", uncheck "Use the default cover photo"
  * Upload a new photo. Recommended 1024 x 768 or larger.

-- Button Styles
  * A universal .button class is used  and applied to many elements to improve interaction on touch devices.

-- Media Queries
  * By default with no media queries, the theme is fluid width.
  * There is a break point for a minimum and maximum width.
     @media screen and (min-width:980px)
     @media screen and (max-width:760px)
     
-- Fluid width sidebars
  Sidebars and main regions widths are calculated proportionally to the
  #container width using percentage widths and margins. It is recommended
  you set #container widths using media queries.
  
  DEFAULT VALUES:
  
  SB1 = Sidebar First width = 18%
  SB2 = Sidebar Second width = 18%
  G = Gutter between main column and sidebars = 2% 

  If you change any of the above default values, you should update the other
  properties that depend on these values to avoid breaking the layout. The
  formula are provided next to each relevant CSS property.
  
  Example:
  
  /* 3 columns */
  body.two-sidebars #main {
    float: left;
    margin: 0 0 0 20%; /* SB1 + G */ 
    padding: 0;
    width: 59.97%; /* 99.97 - (SB1 + G) - (SB2 + G) */
  }


-------
Cover photo source by Eirik Solheim :: http://www.flickr.com/photos/eirikso/3077614089/
License: http://creativecommons.org/licenses/by-sa/2.0/

-------
SUPPORT

If you have questions or problems, check the issue list before submitting a new issue: 
http://drupal.org/project/issues/flux

For general support, please refer to:
http://drupal.org/support