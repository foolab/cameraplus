MACRO(QT_ADD_TRANSLATIONS)
  if (QT4)
    QT4_ADD_TRANSLATION(QM ${TRANS})
  elseif(QT5)
    QT5_ADD_TRANSLATION(QM ${TRANS})
  endif()

  # No idea why this is needed but we don't get .qm files without it
  add_custom_target(translations "ALL" DEPENDS ${QM})

ENDMACRO(QT_ADD_TRANSLATIONS)

set(TRANS
  cameraplus-pl.ts
  )

QT_ADD_TRANSLATIONS()
