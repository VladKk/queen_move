cmake_minimum_required(VERSION 3.17)
project(queen_move LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/vlad/Qt/6.0.0/gcc_64/lib/cmake")

set(QT_VERSION 6)
set(REQUIRED_LIBS Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Widgets)

add_executable(${PROJECT_NAME}
        src/main.cpp

        src/main_win/main_win.h
        src/main_win/main_win.cpp
        src/main_win/main_win.ui

        src/board/board.h
        src/board/board.cpp

        res/resources.qrc
        )

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
