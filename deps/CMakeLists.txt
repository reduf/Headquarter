set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable mbedtls programs")
add_subdirectory(mbedtls-3.0.0)

if (HQ_ENABLE_TESTING)
  add_subdirectory(utest)
endif()
