REM Example batch file for generating a song list
REM Make sure SSPSONGDIR is set to your SongShowPlus songs directory

set SSPSONGDIR=C:\Documents and Settings\All Users\Documents\R-Technics\SongShow Plus\Songs
set SSPOUTPUT=%TEMP%\songlist.csv

songlist "%SSPSONGDIR%" "%SSPOUTPUT%"
explorer %SSPOUTPUT%