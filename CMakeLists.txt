cmake_minimum_required(VERSION 3.24)
project(QuickNav)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -v)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(QuickNav MainApp.cpp MainFrame.cpp FolderPanel.hpp ScrollWindowNavGrid.cpp ScrollWindowNavGrid.hpp)
target_link_libraries(QuickNav ${wxWidgets_LIBRARIES})