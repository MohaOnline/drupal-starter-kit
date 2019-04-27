AUDIOFIELD.MODULE
--------------------------------------------------------------------------------
This module allows embedding an audio file in a CCK field
This module is for adding new field that allows embedding an mp3 audio.

This module will use the built-in HTML5 MP3 Player to play your audio files by
default, but you may choose one of several other supported players:
1. WordPress Audio Player
    http://wpaudioplayer.com/download
    * Note: make sure you should download the standalone edition
2. XSPF Slim Player
    https://sourceforge.net/projects/musicplayer/files/latest/download
3. XSPF Button Player
    http://prdownloads.sourceforge.net/musicplayer/button_player-0.1.zip
4. SoundManager 2
    http://www.schillmania.com/projects/soundmanager2/
5. Flowplayer
    Install the Flowplayer module
    http://drupal.org/project/flowplayer
6. jPlayer
    Install the jPlayer module
    http://drupal.org/project/jplayer
6. wavesurfer.js 2.0
    https://github.com/katspaugh/wavesurfer.js/

The resulting folder structure should resemble the following (you may need to
rename the folders and files to match):

> The standalone WordPress player should be at:
/sites/all/libraries/player/audio-player/player.swf

> The XSPF slim player should be at:
/sites/all/libraries/player/xspf_player_slim.swf

> The XSPF button player should be at:
/sites/all/libraries/player/button/musicplayer.swf

> The Sound Manager 2 player should be at:
/sites/all/libraries/player/soundmanager2

> The jPlayer player should be at:
/sites/all/libraries/player/jplayer/jquery.jplayer.min.js

> The wavesurfer.js player should be at:
/sites/all/libraries/player/wavesurfer/dist/wavesurfer.min.js

This module gives you the ability to choose the audio player you would like to
use on your web site from many audio players, from the configuration page.

Finally you have to put any mp3 audio file at "\sites\all\libraries\player\"
and you have to name it as Sample_Track.mp3, this step just to gives the ability
to test all audio players before you choose your default audio player

API:
Originally this module supports mp3, wav, ogg, opus, and webm audio files.
Other modules can extend this support by implementing hook_audiofield_players().

DISPLAY FORMATTERS
--------------------------------------------------------------------------------
The display formatter is called "AudioField Player". You can choose which type
of player you want to use, as well as other settings in the formatter settings
located under Content -> Fields -> Manage Display.

MAINTAINERS
--------------------------------------------------------------------------------
Tamer Zoubi - <tamerzg@gmail.com>
Daniel Moberly - <daniel.moberly@gmail.com>
