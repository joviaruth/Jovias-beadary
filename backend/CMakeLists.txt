cmake_minimum_required(VERSION 3.20)
project(jovias_beadary_api LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  Crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG v1.2.1
)
FetchContent_MakeAvailable(Crow)

add_executable(jovias-api src/main.cpp)
target_link_libraries(jovias-api PUBLIC Crow::Crow)
