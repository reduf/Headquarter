set(
  COMMON_SOURCES
  
  align.h
  array.c
  array.h
  assert.h
  common.c
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

  win32/dlfunc.c
  win32/process.c
  win32/thread.c
  win32/time.c
  win32/win32.h
)

set_source_files_properties(
  ${COMMON_SOURCES}
  PROPERTIES HEADER_FILE_ONLY TRUE
)

add_library(
  common

  common.c
  ${COMMON_SOURCES}
)

target_include_directories(
  common

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)
