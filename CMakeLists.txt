cmake_minimum_required(VERSION 3.17)
project(OPE_DEMO)

set(CMAKE_CXX_STANDARD 14)

add_executable(OPE_DEMO main.cpp Server/ope_server.cpp Server/ope_server.h Server/BSTree.h Server/BSTree_test.cpp)
add_executable(BSTree_test Server/BSTree_test.cpp Client/ope_client.cpp Client/ope_client.h Util/AES.cpp Util/AES.h Util/Base64.cpp Util/Base64.h)
add_executable(ope_server Server/ope_server.cpp Util/AES.cpp Util/AES.h Util/Base64.cpp Util/Base64.h)