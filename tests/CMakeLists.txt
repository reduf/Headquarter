add_executable(test_sts test_sts.c)
target_link_libraries(test_sts PRIVATE portal utest)

add_executable(test_array test_array.c)
target_link_libraries(test_array PRIVATE common utest)
