add_executable(test_sts test_sts.c)
target_link_libraries(test_sts PRIVATE portal utest)

add_executable(test_array test_array.c)
target_link_libraries(test_array PRIVATE common utest)

add_executable(test_prf test_prf.c)
target_link_libraries(test_prf PRIVATE portal utest)

add_executable(test_transform test_transform.c)
target_link_libraries(test_transform PRIVATE mbedcrypto utest)

add_executable(test_sls test_sls.c)
target_link_libraries(test_sls PRIVATE portal mbedcrypto utest common)
