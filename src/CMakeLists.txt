include_directories (
    ${SPCLIENT_INCLUDE_DIRS}
    ${ELEMENTARY_INCLUDE_DIRS}
    ${ECORE_INCLUDE_DIRS}
    ${ECORE_FILE_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(THEME_FILE ${THEME_INSTALL_DIR}/default.edc.edj)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_custom_command(
     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/default.edc.edj
     COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/images ${CMAKE_SOURCE_DIR}/data/default.edc ${CMAKE_CURRENT_BINARY_DIR}/default.edc.edj
     DEPENDS ${CMAKE_SOURCE_DIR}/data/default.edc
)

set(ANNA-CONTENT
    main.c
    main.h
    ui.h
    ui.c
    config_screen.h
    config_screen.c
    config.h
    ${CMAKE_CURRENT_BINARY_DIR}/default.edc.edj
)

add_executable(anna-greeter ${ANNA-CONTENT})

target_link_libraries (anna-greeter
    ${ELEMENTARY_LIBRARIES}
    ${EVAS_LIBRARIES}
    ${SPCLIENT_LIBRARIES}
    ${ECORE_LIBRARIES}
    ${ECORE_FILE_LIBRARIES}
    ${EDJE_LIBRARIES}
    ${EFREET_LIBRARIES}
    ${EET_LIBRARIES}
    ${EINA_LIBRARIES}
)

install(TARGETS anna-greeter DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/default.edc.edj
  DESTINATION ${THEME_INSTALL_DIR})
install(FILES ${CMAKE_SOURCE_DIR}/data/images/entrance_background.jpg
  DESTINATION ${THEME_INSTALL_DIR})
