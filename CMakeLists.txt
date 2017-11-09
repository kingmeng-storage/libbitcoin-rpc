cmake_minimum_required(VERSION 3.7)
project(libbitcoin-rpc)

set(CMAKE_CXX_STANDARD 11)





aux_source_directory(. DIR_SRCS)
add_executable(libbitcoin-rpc ${DIR_SRCS})
target_link_libraries(libbitcoin-rpc bitcoin boost_system bitcoin-client bitcoin-protocol jsoncpp)