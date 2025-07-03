cmake_minimum_required(VERSION 3.25)
project(WPCEditor VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用依赖文件生成,用于增量编译
set(CMAKE_DEPENDS_IN_PROJECT_ONLY OFF)
set(CMAKE_DEPENDS_GLOBAL_WARNING OFF)

# 设置编译缓存目录
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# 启用 ccache 加速编译(如果可用)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# 编译优化设置
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# 编译器优化标志
if(MSVC)
    # Visual Studio编译器优化
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Oi /GL")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /LTCG")
else()
    # GCC/MinGW编译器优化
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -march=native")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()

# 启用多线程编译
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Unity构建设置(合并源文件以加速编译)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)

if(MINGW)
    # 设置Qt Tools MinGW工具链路径（完整版本）
    set(QT_MINGW_PATH "H:/QT/Tools/mingw1310_64")
    set(QT_RUNTIME_PATH "H:/QT/6.9.0/mingw_64")
    
    # 强制指定编译器和链接器路径
    set(CMAKE_C_COMPILER "${QT_MINGW_PATH}/bin/gcc.exe")
    set(CMAKE_CXX_COMPILER "${QT_MINGW_PATH}/bin/g++.exe")
    set(CMAKE_RC_COMPILER "${QT_MINGW_PATH}/bin/windres.exe")
    set(CMAKE_AR "${QT_MINGW_PATH}/bin/ar.exe")
    set(CMAKE_RANLIB "${QT_MINGW_PATH}/bin/ranlib.exe")
    set(CMAKE_LINKER "${QT_MINGW_PATH}/bin/ld.exe")
    
    # 设置工具链搜索路径，优先使用Qt Tools的MinGW
    set(CMAKE_FIND_ROOT_PATH "${QT_MINGW_PATH};${QT_RUNTIME_PATH}")
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    
    # 添加MinGW工具链到PATH前缀，确保优先使用
    set(CMAKE_PREFIX_PATH "${QT_MINGW_PATH}/bin;${CMAKE_PREFIX_PATH}")
    
    # 清理PATH环境变量中的MSYS2路径干扰
    string(REPLACE "C:/msys64/ucrt64/bin;" "" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")
    string(REPLACE "C:/msys64/mingw64/bin;" "" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")
    
    # MinGW链接选项
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition")
    
    # 显式链接Windows库
    link_libraries(mingw32 kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32)
    
    message(STATUS "Using Qt Tools MinGW: ${QT_MINGW_PATH}")
    message(STATUS "Using Qt Runtime: ${QT_RUNTIME_PATH}")
endif()

# 设置Qt路径
if(CMAKE_GENERATOR MATCHES "MinGW")
    set(Qt6_DIR "H:/QT/6.9.0/mingw_64/lib/cmake/Qt6")
else()
    set(Qt6_DIR "H:/QT/6.9.0/msvc2022_64/lib/cmake/Qt6")
endif()

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Widgets 
    Gui
    Network
    Sql
    Concurrent
    PrintSupport
    Svg
    Qml
)

# 启用Qt6的自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/communication
    ${CMAKE_CURRENT_SOURCE_DIR}/include/communication
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/device
    ${CMAKE_CURRENT_SOURCE_DIR}/include/machine
    ${CMAKE_CURRENT_SOURCE_DIR}/include/material
    ${CMAKE_CURRENT_SOURCE_DIR}/include/product
    ${CMAKE_CURRENT_SOURCE_DIR}/include/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/include/run
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/include/workflow
)

# 添加通讯模块子目录
add_subdirectory(communication)

# 源文件
set(SOURCES
    src/ui/tabs/workflow_executor_tab.cpp
    communication/modbus_register_map.cpp
    src/main.cpp
    src/core/application.cpp
    src/core/logger.cpp
    src/ui/mainwindow.cpp
    src/ui/canvas/workflow_canvas.cpp
    src/ui/canvas/workflow_node.cpp
    src/ui/canvas/workflow_connection.cpp
    src/ui/panels/properties_panel.cpp
    src/ui/panels/node_palette.cpp
    src/ui/panels/resource_panel.cpp
    src/ui/panels/device_panel.cpp
    src/ui/tabs/wpc_editor_tab.cpp
    src/ui/tabs/device_manager_tab.cpp
    src/ui/tabs/machine_manager_tab.cpp
    src/ui/tabs/resource_manager_tab.cpp
    src/ui/tabs/board_manager_tab.cpp
    src/ui/widgets/resource_alias_widget.cpp
    src/ui/widgets/product_main_widget.cpp
    src/ui/widgets/product_detail_widget.cpp
    src/ui/material_detail_widget.cpp
    src/ui/material_main_widget.cpp
    src/ui/dialogs/node_properties_dialog.cpp
    src/ui/dialogs/workflow_metadata_dialog.cpp
    src/ui/dialogs/time_estimation_dialog.cpp
    src/ui/dialogs/edit_product_dialog.cpp
    src/workflow/workflow_manager.cpp
    src/workflow/node_factory.cpp
    src/workflow/node_types.cpp
    src/workflow/workflow_serializer.cpp
    src/workflow/time_estimation.cpp
    src/resources/resource_manager.cpp
    src/material/material_manager.cpp
    src/device/device_manager.cpp
    src/machine/machine_manager.cpp
    src/machine/board_mapping_manager.cpp
    src/product/product_manager.cpp
    src/utils/file_utils.cpp
    src/utils/json_utils.cpp
    src/run/lad_workflow_executor.cpp
    # 删除下面这行
    # src/core/mcp_server.cpp
    # 以下文件仍然注释
    # src/run/ppc_workflow_executor.cpp 
    # src/run/ppl_workflow_executor.cpp
    # src/run/tech_workflow_executor.cpp
)

# 头文件
set(HEADERS
    include/core/application.h
    src/run/lad_workflow_executor.h
    include/core/logger.h
    include/ui/mainwindow.h
    include/ui/canvas/workflow_canvas.h
    include/ui/canvas/workflow_node.h
    include/ui/canvas/workflow_connection.h
    include/ui/panels/properties_panel.h
    include/ui/panels/node_palette.h
    include/ui/panels/resource_panel.h
    include/ui/panels/device_panel.h
    include/ui/tabs/wpc_editor_tab.h
    include/ui/tabs/device_manager_tab.h
    include/ui/tabs/machine_manager_tab.h
    include/ui/tabs/resource_manager_tab.h
    include/ui/tabs/board_manager_tab.h
    include/ui/widgets/resource_alias_widget.h
    include/ui/widgets/product_main_widget.h
    include/ui/widgets/product_detail_widget.h
    include/ui/material_detail_widget.h
    include/ui/material_main_widget.h
    include/ui/dialogs/node_properties_dialog.h
    include/ui/dialogs/workflow_metadata_dialog.h
    include/ui/dialogs/time_estimation_dialog.h
    include/ui/dialogs/edit_product_dialog.h
    include/workflow/workflow_manager.h
    include/workflow/node_factory.h
    include/workflow/node_types.h
    include/workflow/workflow_serializer.h
    include/workflow/time_estimation.h
    include/resources/resource_manager.h
    include/material/material_manager.h
    include/device/device_manager.h
    include/machine/machine_manager.h
    include/machine/board_mapping_manager.h
    include/product/product_manager.h
    include/utils/file_utils.h
    include/utils/json_utils.h
   )

# 资源文件
set(RESOURCES
    resources/wpc_resources.qrc
)

# 创建可执行文件
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS}
    ${RESOURCES}

)

# 链接Qt6库和通讯模块
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
    Qt6::Concurrent
    Qt6::PrintSupport
    Qt6::Svg
    Qt6::Qml
    communication_lib
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "WPCEditor"
)

# Windows特定设置
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # 获取Qt安装路径
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_WINDEPLOYQT_EXECUTABLE ${QT_QMAKE_EXECUTABLE} PATH)
    set(QT_WINDEPLOYQT_EXECUTABLE "${QT_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    
    # 自动部署Qt依赖
    if(EXISTS ${QT_WINDEPLOYQT_EXECUTABLE})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${QT_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Deploying Qt libraries"
        )
    else()
        # 手动复制必要的文件
        if(CMAKE_GENERATOR MATCHES "MinGW")
            set(QT_BIN_DIR "H:/QT/6.9.0/mingw_64/bin")
            set(QT_PLUGINS_DIR "H:/QT/6.9.0/mingw_64/plugins")
        else()
            set(QT_BIN_DIR "H:/QT/6.9.0/msvc2022_64/bin")
            set(QT_PLUGINS_DIR "H:/QT/6.9.0/msvc2022_64/plugins")
        endif()
        
        # 复制Qt DLL文件
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BIN_DIR}/Qt6Core.dll"
                "${QT_BIN_DIR}/Qt6Widgets.dll"
                "${QT_BIN_DIR}/Qt6Gui.dll"
                "${QT_BIN_DIR}/Qt6Network.dll"
                "${QT_BIN_DIR}/Qt6Sql.dll"
                "${QT_BIN_DIR}/Qt6Concurrent.dll"
                "${QT_BIN_DIR}/Qt6PrintSupport.dll"
                "${QT_BIN_DIR}/Qt6Svg.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying Qt DLLs"
        )
        
        # 复制平台插件
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory 
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_PLUGINS_DIR}/platforms/qwindows.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
            COMMENT "Copying Qt platform plugins"
        )
        
        # 如果是MinGW，还需要复制MinGW运行时库
        if(CMAKE_GENERATOR MATCHES "MinGW")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${QT_BIN_DIR}/libgcc_s_seh-1.dll"
                    "${QT_BIN_DIR}/libstdc++-6.dll"
                    "${QT_BIN_DIR}/libwinpthread-1.dll"
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMENT "Copying MinGW runtime libraries"
            )
        endif()
    endif()
endif()

# 安装配置
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# 调试信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Qt6 version: ${Qt6_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")