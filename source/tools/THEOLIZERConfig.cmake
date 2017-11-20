#[[###########################################################################

        Theolizer find package

    © 2016 Theoride Technology (http://theolizer.com/) All Rights Reserved.
    "Theolizer" is a registered trademark of Theoride Technology.

    "Theolizer" License
        In the case where you are in possession of a valid “Theolizer” License,
        you may use this file in accordance with the terms and conditions of 
        the use license determined by Theoride Technology.

    General Public License Version 3 ("GPLv3")
        You may use this file in accordance with the terms and conditions of 
        GPLv3 published by Free Software Foundation.
        Please confirm the contents of GPLv3 at https://www.gnu.org/licenses/gpl.txt .
        A copy of GPLv3 is also saved in a LICENSE.TXT file.

    商用ライセンス
        あなたが有効なTheolizer商用ライセンスを保持している場合、
        セオライド テクノロジーの定める使用許諾書の条件に従って、
        このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        またGPLv3のコピーをLICENSE.TXTファイルにおいてます。

]]############################################################################

#*****************************************************************************
#       THEOLIZER_DIRを設定してproject()命令後でfind_package(THEOLIZER)
#           下記を行う
#               Theolizerライブラリのインポート
#               Theolizerのルート・パス設定
#               setup_theolizer()マクロ定義
#                   Theolizerドライバへの解析指示(-DTHEOLIZER_ANALYZE)
#                   Theolizerインクルード・パス設定
#                   Theolizerライブラリ・パス設定
#                   Theolizerライブラリのリンク設定
#                   THEOLIZER_ROOTにTheolizerルートのパスを設定
#                   THEOLIZER_LIBにTheolizerライブラリのパスを設定
#*****************************************************************************

#-----------------------------------------------------------------------------
#       Theolizer root path
#-----------------------------------------------------------------------------

get_filename_component(THEOLIZER_ROOT ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
message(STATUS "THEOLIZER_ROOT=${THEOLIZER_ROOT}")

#-----------------------------------------------------------------------------
#       Theolizerライブラリのsetup
#-----------------------------------------------------------------------------

if (MSVC)
    cmake_minimum_required(VERSION 3.8.0)
    set(ORIGINAL_COMPILER "${CMAKE_CXX_COMPILER}")
    message(STATUS "ORIGINAL_COMPILER =${ORIGINAL_COMPILER}")
    set(EXECUTABLE_PATH "${THEOLIZER_ROOT}/bin")
    configure_file("${THEOLIZER_ROOT}/theolizer.props.in" "${CMAKE_CURRENT_BINARY_DIR}/theolizer.props")
    add_definitions(-D_MSC_VER=${MSVC_VERSION})
else()
    cmake_minimum_required(VERSION 3.2.2)

    set(ORIGINAL_COMPILER "${CMAKE_CXX_COMPILER}")
    set(CMAKE_CXX_COMPILER "${THEOLIZER_ROOT}/bin/TheolizerDriver")
    message(STATUS "ORIGINAL_COMPILER =${ORIGINAL_COMPILER}")
    message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
endif()

#-----------------------------------------------------------------------------
#       setup_theolizer(TARGET_NAME LIB_TYPE)定義
#
#       ${TARGET_NAME}に対して、下記を設定します
#               Theolizerドライバへの追加パラメータ設定
#               インクルード・パス
#               ライブラリ・パス
#               リンクするTheolizerライブラリ
#       ${LIB_TYPE}は下記のどれか
#               StaticWithBoost 静的リンク・ライブラリ(標準ライブラリも静的リンク)
#               Static          静的リンク・ライブラリ(標準ライブラリは動的リンク)
#               Shared          動的リンク・ライブラリ
#-----------------------------------------------------------------------------

macro(setup_theolizer TARGET_NAME LIB_TYPE)

    #Theolizerドライバへの解析要求
    add_definitions("-DTHEOLIZER_DO_PROCESS")

    #共有ライブラリ対応
    if(${LIB_TYPE} STREQUAL "Shared")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTHEOLIZER_DYN_LINK")
    endif()

    # Header
    include_directories(${THEOLIZER_ROOT}/include)

    # Library
    link_directories("${THEOLIZER_ROOT}/library/${CMAKE_BUILD_TYPE}")

    if (MSVC)
        target_link_libraries(${TARGET_NAME} optimized "${THEOLIZER_ROOT}/library/Release/TheolizerLib${LIB_TYPE}.lib")
        target_link_libraries(${TARGET_NAME} optimized "${THEOLIZER_ROOT}/library/Release/TheolizerTest${LIB_TYPE}.lib")
        target_link_libraries(${TARGET_NAME} debug     "${THEOLIZER_ROOT}/library/Debug/TheolizerLib${LIB_TYPE}.lib")
        target_link_libraries(${TARGET_NAME} debug     "${THEOLIZER_ROOT}/library/Debug/TheolizerTest${LIB_TYPE}.lib")
        set_target_properties(${TARGET_NAME} PROPERTIES VS_USER_PROPS "${CMAKE_CURRENT_BINARY_DIR}/theolizer.props")
        add_definitions("-Dtheolizer_original_compiler=${ORIGINAL_COMPILER}")
    else()
        target_link_libraries(${TARGET_NAME} "${THEOLIZER_ROOT}/library/${CMAKE_BUILD_TYPE}/libTheolizerLib${LIB_TYPE}.a")
        target_link_libraries(${TARGET_NAME} "${THEOLIZER_ROOT}/library/${CMAKE_BUILD_TYPE}/libTheolizerTest${LIB_TYPE}.a")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --theolizer_original_compiler=${ORIGINAL_COMPILER}")
    endif()

endmacro()
