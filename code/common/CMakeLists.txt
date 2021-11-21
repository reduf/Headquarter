set(
  COMMON_SOURCES
  
  align.h
  array.c
  array.h
  assert.h
  dlfunc.h
  endian.h
  hash.h
  list.h
  log.c
  log.h
  macro.h
  noreturn.h
  process.h
  rbtree.c
  rbtree.h
  socket.h
  thread.h
  time.h
  timer.c
  timer.h
  uuid.h
)

if (WIN32)
  list(
    APPEND COMMON_SOURCES
    win32/dlfunc.c
    win32/process.c
    win32/thread.c
    win32/time.c
    win32/win32.h
  )
else()
  list(
    APPEND COMMON_SOURCES
    linux/dlfunc.c
    linux/thread.c
    linux/time.c
  )
endif()

set_source_files_properties(
  ${COMMON_SOURCES}
  PROPERTIES HEADER_FILE_ONLY TRUE
)

add_library(
  common

  build_common.c
  ${COMMON_SOURCES}
)

target_link_libraries(common PRIVATE pthread m dl)
target_include_directories(
  common

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)
