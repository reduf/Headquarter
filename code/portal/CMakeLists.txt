set(
  PORTAL_SOURCES
)

set_source_files_properties(
  ${PORTAL_SOURCES}
  PROPERTIES HEADER_FILE_ONLY TRUE
)

add_library(
  portal

  build_portal.c
  ${PORTAL_SOURCES}
)
