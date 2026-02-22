cmake_minimum_required(VERSION 3.16)

project(updated_orderbook_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(updated_orderbook_2
    HFTAlgorithms.cpp
    HFTUtils.cpp
    MachingEngine.cpp
    OrderBook.cpp
    main.cpp
)