set(
  PORTAL_SOURCES

  login.c
  login.h
  sts.c
  sts.h
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

target_link_libraries(portal PRIVATE common)
target_include_directories(
  portal

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

