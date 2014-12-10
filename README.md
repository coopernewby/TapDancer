Tap Dancer
===========

A video mashup openframworks app that explores streaming vintage YouTube videos mixed with new dance music

Required Addons
===============
Must have openframeworks
http://www.openframeworks.cc/

Youtube-dl
http://rg3.github.io/youtube-dl/

ofxMSABPMTapper
https://github.com/memo/ofxMSABPMTapper

ofxMSATimer
https://github.com/memo/ofxMSATimer/blob/master/src/MSATimer.h

ofxUI
https://github.com/rezaali/ofxUI

Instructions
===============
you have 4 video decks. before hitting play, use 'p' to play the current video, and tap along with a section of the song with the spacebar

Keyboard Commands:
p - Play video
Left Arrow Key - next video
Right Arrow Key - previous video
Space bar - video bpm tap
s - Set video bpm + music video audio
f - toggle Fullscreen
h - toggle dashboard/ video mode

r - reverse video playback

l - double video speed
k - half video speed

, - Nudge Video Backwards
. - Nudge Video Forward

Known Issues
===========
- Copy & Paste dont work into the YouTube text box yet
- System callback to command line youtube-dl doesn't work yet, you have to run youtube-dl manually in the commandline with: youtube-dl --get-url --format 18 https://www.youtube.com/watch\?v\=OeE1amkKuBU 
