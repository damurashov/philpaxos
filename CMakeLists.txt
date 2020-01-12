cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 17)





# Project 

project (hl3_d)
include_directories(src)

file(GLOB_RECURSE SRCS_TRANSMISSION "src/transmission/*.cpp" "src/transmission/*.hpp" "src/transmission/*.h" "src/transmission/*.c")
add_library(hl3_transmission ${SRCS_TRANSMISSION})

file(GLOB_RECURSE SRCS_SERVER "src/server/*.cpp" "src/server/*.hpp" "src/server/*.h" "src/server/*.c")
add_library(hl3_server ${SRCS_SERVER})
target_link_libraries(hl3_server hl3_transmission)

file(GLOB_RECURSE SRCS_PAXOSPHIL "src/paxosphil/*.cpp" "src/paxosphil/*.hpp" "src/paxosphil/*.h" "src/paxosphil/*.c")
add_library(hl3 ${SRCS_PAXOSPHIL})
target_link_libraries(hl3 hl3_server hl3_transmission pthread)

#file(GLOB_RECURSE SRCS "src/*.cpp" "src/*.hpp" "src/*.h" "src/*.c")
#add_library(hl3 ${SRCS})
#target_link_libraries(hl3 pthread)





# Tests

project (hl3_test)
#add_definitions( -D_VARIADIC_MAX=10 )
#add_definitions( -D_DEBUG )
#add_definitions( -D_MT )
#add_definitions( -D_DLL )

add_subdirectory(third_party/googletest-master)
file(GLOB TEST_SRCS "test/*.cpp" "test/*.hpp" "test/*.h" "test/*.c")
include_directories(src)
include_directories(${GTEST_INCLUDE_DIRS}) 

add_executable(hl3_test_ALL ${TEST_SRCS})
target_link_libraries(hl3_test_ALL hl3 gtest gtest_main pthread)

add_executable(hl3_test_messaging "test/message_series_probe.cpp")
target_link_libraries(hl3_test_messaging hl3 gtest gtest_main pthread)

add_executable(hl3_test_receivers_senders "test/receivers_senders.cpp")
target_link_libraries(hl3_test_receivers_senders hl3 gtest gtest_main pthread)

add_executable(hl3_test_transport_addess_socket_wrappers "test/transport_addess_socket_wrappers.cpp")
target_link_libraries(hl3_test_transport_addess_socket_wrappers hl3 gtest gtest_main pthread)

add_executable(hl3_test_local_udp_server "test/test_local_udp_server.cpp")
target_link_libraries(hl3_test_local_udp_server hl3 gtest gtest_main pthread)

add_executable(hl3_test_paxos_messages_serialization "test/paxosphil_srdr_messages.cpp")
target_link_libraries(hl3_test_paxos_messages_serialization hl3 gtest gtest_main pthread)

add_executable(hl3_test_paxos_messages_transmission "test/paxos_messages_transmission.cpp")
target_link_libraries(hl3_test_paxos_messages_transmission hl3 gtest gtest_main pthread)

#add_executable(hl3_test_paxos_nodes "test/paxos_nodes.cpp")
#target_link_libraries(hl3_test_paxos_nodes hl3 gtest gtest_main pthread)





# Demonstrations 

project(hl3_prototypes)
include_directories(src)

add_executable(hl3_mock_philosopher "test_pndemos/mock4philosopher.cpp")
target_link_libraries(hl3_mock_philosopher hl3)

add_executable(hl3_mock_proposer "test_pndemos/mock4proposer.cpp")
target_link_libraries(hl3_mock_proposer hl3)

add_executable(hl3_mock_acceptor "test_pndemos/mock4acceptor.cpp")
target_link_libraries(hl3_mock_acceptor hl3)