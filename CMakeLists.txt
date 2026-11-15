
cmake_minimum_required(VERSION 3.16)

project(StockManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_add_executable(StockManager
    main.cpp

    models/Producto.cpp
    models/Categoria.cpp
    models/Movimiento.cpp

    database/Database.cpp

    controllers/InventarioController.cpp

    views/MainWindow.cpp
    views/ProductoView.cpp

    reports/Reporte.cpp
)

target_link_libraries(StockManager PRIVATE
    Qt6::Widgets
    Qt6::Sql
)
