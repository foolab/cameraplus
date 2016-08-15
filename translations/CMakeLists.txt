MACRO(QT_CREATE_TRANSLATIONS)
  set(TRANS
    cameraplus-pl.ts
    )

  set(DIRS ../src ../declarative ../lib ../panorama ../qml)

  if (QT4)
    QT4_CREATE_TRANSLATION(QM ${DIRS} ${TRANS})
  elseif(QT5)
    QT5_CREATE_TRANSLATION(QM ${DIRS} ${TRANS})
  endif()

  # No idea why this is needed but we don't get .qm files without it
  add_custom_target(translations "ALL" DEPENDS ${QM})

  install(FILES ${QM} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cameraplus/translations)
ENDMACRO(QT_CREATE_TRANSLATIONS)

QT_CREATE_TRANSLATIONS()
