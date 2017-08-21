cmake_minimum_required(VERSION 3.8)
project(Daeum)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  Model/Song.cpp Model/Song.h Model/Subject.cpp
        Model/Subject.h Model/Playlist.cpp Model/Playlist.h Model/Album.cpp
        Model/Album.h GUI/MainFrame.cpp GUI/MainFrame.h Controller/MediaController.cpp Controller/MediaController.h
        Controller/ControllerInterface.cpp Controller/ControllerInterface.h
        GUI/MediaTimer.cpp GUI/MediaTimer.h GUI/DaeumApp.cpp GUI/DaeumApp.h GUI/Observer.cpp GUI/Observer.h
        savedSession.txt)

add_executable(Daeum ${SOURCE_FILES})

#Link wxLibrary

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/azrael/Clion/clion-2017.2/bin/cmake/share/cmake-3.8/Modules)
find_package(wxWidgets COMPONENTS core base media REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(Daeum ${wxWidgets_LIBRARIES})

#Link TagLib library (extracts metadata from audio files)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/azrael/Findcmake)
find_package(TAGLIB REQUIRED tag )
target_link_libraries(Daeum ${TAGLIB_LIBRARIES})

