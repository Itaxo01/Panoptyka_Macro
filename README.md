# Panoptyka_Macro
A macro for the game Panoptyka. It just (hopefully) scans the screen on background and perform some simple actions on the game 

The code in this project will be entirely coded by myself, in order do learn a bit of OpenCV for the image manipulation and processing and also system inputs with X11 and the other one for windows.

The ambition is to get a executable capable of running on background on both linux and windows very fast.

### Next steps
Run on background with Virtual X Server

### Windows modifications
just to save the needed modifications when migrating to windows. 



### Log
(25/02/2026) Just set the initial project organization, and made the WindowManager to gather the window of the application using X11 (For now, I will develop it just for the linux, later on I think about the microsoft windows too). Wayland could be an option but it would be a lot more troublesome, but I got it that it's better in terms of security (which is a thing I don't need and definitely don't want as I need my program to spy on others).

Now there is a basic .ppm plot to check if the selected screen is correct. 

(26/02/2026) Using openCV I'm now able to select the list of points that have a perfect matching between the game screen and a pattern png, saved locally. There isn't a resolution fix as that would be more troublesome than just using the same resolution after saving the pattern png. That works already

(27/02/2026) Now with XTest I can already perform actions with those informations. This works. The next step is making this run on background with Virtual X Server.

I will now try to remove the include of the X11 on the coordinateFinder to diminish the trouble later on windows.