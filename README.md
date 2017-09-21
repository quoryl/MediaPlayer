# MusicPlayer
To be able to make it work you need to have:
1. wxWidgets 3.1 ( it may work with other versions, not tested)
2. gstreamer1.0 ( in particular the bad and ugly plugins )
3. googletest (in tests directory create a folder "lib", extract googletest there)
4. TagLib for extracting metadata from audio files
    4.a You may need to download FindTagLib.cmake from google. Edit the main cmake file to find the library
