if (QT4)
  set(LUPDATE ${QT_LUPDATE_EXECUTABLE})
  set(LRELEASE ${QT_LRELEASE_EXECUTABLE})
elseif (QT5)
  set(LUPDATE ${Qt5_LUPDATE_EXECUTABLE})
  set(LRELEASE ${Qt5_LRELEASE_EXECUTABLE})
endif()

set(TS_FILES cameraplus-pl.ts)
set(SRC_DIRS src declarative lib panorama qml)

# We have 2 main targets:
#  - ts-files which updates ts files from sources
#  - qm-files which generates qm files from ts files
# Each ts file has its own targets to be updated and released

# An empty target to generate ts files
add_custom_target(ts-files)

# An empty target to generate qm files
add_custom_target(qm-files ALL SOURCES ${TS_FILES})

foreach (ts ${TS_FILES})
  string(REPLACE . - target ${ts})
  string(REPLACE .ts .qm qm ${ts})

  # A target to update translation:
  add_custom_target(${target}
    COMMAND ${LUPDATE} ${SRC_DIRS} -ts ${CMAKE_SOURCE_DIR}/translations/${ts}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

  # A target to build each translation:
  add_custom_target(${target}-qm
    COMMAND ${LRELEASE} translations/${ts} -qm ${CMAKE_BINARY_DIR}/translations/${qm}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

  # qm file depends on ts file not the target so the ts file does not get updated during build
  add_dependencies(${target}-qm ${CMAKE_SOURCE_DIR}/translations/${ts})

  # add ts file to main ts target
  add_dependencies(ts-files ${target})

  # add qm file to main qm target
  add_dependencies(qm-files ${target}-qm)

  install(FILES ${CMAKE_BINARY_DIR}/translations/${qm} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cameraplus/translations)

endforeach(ts)

# A target to build translation template:
add_custom_target(ts-template
  COMMAND ${LUPDATE} ${SRC_DIRS} -ts ${CMAKE_SOURCE_DIR}/translations/cameraplus.ts
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_dependencies(ts-files ts-template)

# A target to extract plural formats:
add_custom_target(plurals
  ALL
  COMMAND ${LUPDATE} ${SRC_DIRS} -pluralonly -ts ${CMAKE_SOURCE_DIR}/translations/plurals.ts
  COMMAND ${LRELEASE} translations/plurals.ts -qm ${CMAKE_BINARY_DIR}/translations/plurals.qm
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
install(FILES ${CMAKE_BINARY_DIR}/translations/plurals.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cameraplus/translations)
