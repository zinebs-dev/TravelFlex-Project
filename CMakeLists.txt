cmake_minimum_required(VERSION 3.16)

project(TravelFlex VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Qt dependencies (Widgets + Sql) ---
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# --- Source layout ---
set(CORE_SOURCES
    src/core/database.h
    src/core/database.cpp
)

set(COMMON_SOURCES
    src/common/mainwindow.h
    src/common/mainwindow.cpp
    src/common/mainwindow.ui
    src/common/welcome.h
    src/common/welcome.cpp
)

set(ADMIN_SOURCES
    src/admin/adminlogin.h
    src/admin/adminlogin.cpp
    src/admin/homeadmin.h
    src/admin/homeadmin.cpp
    src/admin/addpackage.h
    src/admin/addpackage.cpp
    src/admin/editpackage.h
    src/admin/editpackage.cpp
    src/admin/deletepackage.h
    src/admin/deletepackage.cpp
)

set(TOURIST_SOURCES
    src/tourist/welcometourist.h
    src/tourist/welcometourist.cpp
    src/tourist/registertourist.h
    src/tourist/registertourist.cpp
    src/tourist/logintourist.h
    src/tourist/logintourist.cpp
    src/tourist/hometourist.h
    src/tourist/hometourist.cpp
    src/tourist/reserverpackage.h
    src/tourist/reserverpackage.cpp
    src/tourist/paymentpackage.h
    src/tourist/paymentpackage.cpp
    src/tourist/profilepage.h
    src/tourist/profilepage.cpp
)

set(PROJECT_SOURCES
    src/main.cpp
    ${CORE_SOURCES}
    ${COMMON_SOURCES}
    ${ADMIN_SOURCES}
    ${TOURIST_SOURCES}
    resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TravelFlex
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(TravelFlex SHARED ${PROJECT_SOURCES})
    else()
        add_executable(TravelFlex ${PROJECT_SOURCES})
    endif()
endif()

# Every module still uses flat #include "xxx.h" style includes, so we expose
# each module folder on the include path instead of rewriting every include.
target_include_directories(TravelFlex PRIVATE
    src
    src/core
    src/common
    src/admin
    src/tourist
)

target_link_libraries(TravelFlex PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# --- Platform-specific bundle settings ---
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.travelflex.app)
endif()

set_target_properties(TravelFlex PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --- Copy the SQLite database next to the built executable ---
# This keeps main.cpp's relative path (applicationDirPath() + "/datab.db")
# working no matter where the project is built.
add_custom_command(TARGET TravelFlex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/database/datab.db"
        "$<TARGET_FILE_DIR:TravelFlex>/datab.db"
)

include(GNUInstallDirs)
install(TARGETS TravelFlex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES database/datab.db DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TravelFlex)
endif()
