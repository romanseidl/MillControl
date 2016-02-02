# MillControl
##Arduino Coffee Grinder Control

###THIS DESCRIPTION IS A BIT OUT OF DATE I WILL UPDATE IT WHEN I GET TO IT. 
If you speak german please refer to: 
https://www.kaffee-netz.de/threads/millcontrol-arduino-muehlensteuerung-mit-timer-gewichtsautomatik-und-brew-timer.95553/

This was designed to build a timer controlled coffe grinder out of any regular machine. It was developed to be used with a Graef CM 70 but it should work fine with any brand or model.

###Requirements
 - Runs on Arduino Pro Mini & compatible systems (should be most arduino machines having a ATmega328 or better CPU, needs 32k of storage space).
 - Needs a rotary encoder with a switch and an optional second switch (mill switch). If you only have one switch comment out the define in UI.h
 - Output is to a 128*64 graphics display, comment out the orientation define in UI.h

###Dependencies:
- ClickButton - https://code.google.com/p/clickbutton/
- Encoder (v1.2) - http://www.pjrc.com/teensy/td_libs_Encoder.html
- TimerOne (v1.1) - https://github.com/PaulStoffregen/TimerOne
- U8Glib - https://github.com/olikraus/u8glib

###User Interface
####Select a Time Mode
You can select from up to 20 timer modes with different timeings for a single click, double click and long click of the mill button. Modes can be timed, start/ stop (starting the mill on a click and stopping it at the next) and hold (_with extra mill button only_).

_If there is no mill button then you can only choose from single and double click. Long click is then bound to the edit menu_

#####Portrait mode
![Select](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/hoch_select.JPG)
![Select Direct](https://github.com/romanseidl/MillControl/blob/gh-pages/pictures/hoch_select_direct.JPG)
#####Landscape Mode
![Select Landscape](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/quer_select.JPG)
![Select Direct Landscape](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/quer_select_d.JPG)

####Edit a Time Mode
There is an extensive editor allowing you to add (+), delete (x), move (<>) and edit time modes. For a time mode the respective times and the title are editable.

#####Portrait mode
![Editor](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/hoch_edit_sp.JPG)
![Char Editor](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/hoch_edit_char.JPG)
#####Landscape Mode
![Landscape Editor](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/quer_editor.JPG)

####Run!!
Finally the idea of the timer ist to run. So there is a run mode... It is started from select mode as explained above. In a timed run there is a progress bar at the bottom of the screen.

A run can be stopped any time by pressing the button used to start the run. For 5 seconds the run will pause and if you re-click it can be continued. 

#####Portrait mode
![Run Mode](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/hoch_run.JPG)
![Direct Run Mode](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/pictures/hoch_run_direct.JPG)
#####Landscape mode
![Landscape Run Mode](https://github.com/romanseidl/MillControl/blob/gh-pages/pictures/quer_run.JPG)

###Schematic
I prefer a old school schmatic style over those fanzy fritzing pictures. Still I added them below in case you prefer them. 
[![Schematic](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/sketch/schematic.png)](https://raw.githubusercontent.com/romanseidl/MillControl/gh-pages/sketch/schematic.pdf)

###(c) 2015 by Roman Seidl

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

All trademarks mentioned belong to their owners, third party brands, product names, trade names, corporate names and company names mentioned may be trademarks of their respective owners or registered trademarks of other companies and are used for purposes of explanation and to the owner's benefit, without implying a violation of copyright law. 
