Media Development profile
-------------------------

== What does it do ==

The profile currently gets your D7 install configured to insert images into a text area via ckeditor.
Do you have suggestions for extra/new things that should happen on 
installation? Please file an issue in media_dev's issue queue.

== How to install ==

1. Download drush: http://drupal.org/project/drush

2. Then, execute the following from the command line:

  'drush make http://is.gd/KoSSpW --prepare-install media_dev'
alternatively from a clone of the repo and from the branch
  'drush make --prepare-install build-media_dev.make media_dev_build'

to download everything (core, the media profile, contrib modules & libraries)
into a folder called 'media_dev'.

3. Install the site as usual, using the 'Media development profile' as the
site's profile.

Another option is to do it manually:

1. Get the modules

git clone --branch 7.x-2.x http://git.drupal.org:project/media.git
git clone --branch 7.x-1.x http://git.drupal.org:project/media_youtube.git
git clone --branch 7.x-1.x http://git.drupal.org:project/plupload.git
git clone --branch 7.x-1.x http://git.drupal.org:project/wysiwyg.git
... (for every other module that is needed).

2. Download Ckeditor, Plupload & MediaElement in sites/all/libraries

3. Install the site as usual, using the 'Media development profile'
as the site's profile.
