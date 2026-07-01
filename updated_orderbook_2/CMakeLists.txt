cmake_minimum_required(VERSION 3.16)

project(updated_orderbook_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(updated_orderbook_2
    HFTAlgorithms.cpp
    HFTUtils.cpp
    MatchingEngine.cpp
    OrderBook.cpp
    main.cpp
)

include(CTest)

if(BUILD_TESTING)
    add_executable(updated_orderbook_tests
        HFTAlgorithms.cpp
        HFTUtils.cpp
        MatchingEngine.cpp
        OrderBook.cpp
        Tests.cpp
    )

    add_test(NAME updated_orderbook_tests COMMAND updated_orderbook_tests)
endif()
