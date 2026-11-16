
cmake_minimum_required(VERSION 3.16)

project(StockManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

add_executable(StockManager
    main.cpp

    models/Producto.cpp
    models/Categoria.cpp
    models/Movimiento.cpp

    database/Database.cpp

    controllers/InventarioController.cpp

    views/MainWindow.cpp
    views/ProductoView.h
    views/ProductoView.cpp

    reports/Reporte.cpp
)

target_link_libraries(StockManager PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)
