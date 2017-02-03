cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

# we want to use C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
    ImageModel.h
)

set(SOURCE_FILES
    Main.cpp
    ImageModel.cpp
)

set(QML_FILES
    data/qml/Main.qml
    data/qml/components/qmldir
    data/qml/components/Point.qml
    data/qml/components/SpritePoint.qml
    data/qml/components/InputCoord.qml
)

set(QRC_FILES
    data/ressource.qrc
    data/qml/qml.qrc
)

# set(QtModules Core Gui Widgets Qml)
find_package(Qt5 COMPONENTS Core Gui Quick Qml Widgets REQUIRED PATHS "/opt/Qt/5.7/gcc_64/lib/cmake/Qt5")

qt5_add_resources(RESOURCE_FILES ${QRC_FILES})
qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${QML_FILES} ${RESOURCE_FILES} ${MOC_FILES})

#================ Link with libraries ================#
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::Qml)
