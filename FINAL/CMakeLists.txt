cmake_minimum_required(VERSION 3.5)

project(FINAL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 COMPONENTS Core Gui Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        client.cpp
        client.h
        server.cpp
        server.h
        database.cpp
        database.h
        mainwindow.h
        mainwindow.ui
        signupwindow.cpp
        signupwindow.h
        signupwindow.ui
        prompt.cpp
        prompt.h
        prompt.ui
        lobbyscreen.cpp
        lobbyscreen.h
        lobbyscreen.ui
        gamescreen.cpp
        gamescreen.h
        gamescreen.ui
        gameplay.cpp
        gameplay.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FINAL
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FINAL APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FINAL SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FINAL
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FINAL PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(FINAL PRIVATE Qt6::Network)
target_link_libraries(FINAL PRIVATE Qt6::Core Qt6::Gui Qt6::Sql)
set_target_properties(FINAL PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS FINAL
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FINAL)
endif()
