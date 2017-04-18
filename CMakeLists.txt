cmake_minimum_required (VERSION 2.6)
project (Anna)

include(GNUInstallDirs)

find_package(SpClient REQUIRED)
find_package(Elementary REQUIRED)
find_package(Evas REQUIRED)
find_package(Ecore COMPONENTS file)
find_package(Edje REQUIRED)
find_package(Efreet REQUIRED)
find_package(Eet REQUIRED)
find_package(Eina REQUIRED)

set(THEME_INSTALL_DIR "${CMAKE_INSTALL_FULL_DATADIR}/${CMAKE_PROJECT_NAME}/")

add_subdirectory(./src)
