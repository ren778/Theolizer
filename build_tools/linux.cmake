#[[###########################################################################
        Linux用ビルドCMakeスクリプト

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
#       使い方
#           当スクリプト呼び出し時に-DPROC_ALL=xxxで下記のどれかを定義する
#               省略        ビルド・フォルダを削除後、全ての処理を行う。
#               short_all   コンフィグからTheolizerのインストールまでを行う。
#               last_all    インストールされたTheolizerが使えることを確認する。
#           下記CMake変数を定義する
#               THEOLIZER_SOURCE    Theolizerのソース・ツリーのルート・パス
#               THEOLIZER_BINARY    Theolizerのビルド・フォルダのプリフィクス
#               THEOLIZER_PREFIX    ビルドしたTheolizerのインストール先のプリフィクス
#               BOOST_PREFIX        boostがインストールされているルート・パスのプリフィクス
#               LLVM                llvmがインストールされているルート・パス
#               CC32/CC64           msvc以外のコンパイラのパス
#               MAKE                MinGWでjomを使う場合にjomのパス
#               PARALLEL            makeを使う場合の並列処理数
#               SUMMARY             結果概要を記録するファイル名
#           下記でビルドする対象を定義する
#               build_by_msvc()
#               build_by_gcc()
#*****************************************************************************

#-----------------------------------------------------------------------------
#       設定するパラメータ
#-----------------------------------------------------------------------------

# Theolizerのソース・フォルダ
set(THEOLIZER_SOURCE "../source")

# Theolizerのビルド・フォルダのプリフィクス
set(THEOLIZER_BINARY "../../build/")

# Theolizerのインストール先のプリフィクス
set(THEOLIZER_PREFIX "$ENV{HOME}/install/Theolizer-")

# boostのルート・フォルダ   ${BOOST_PREFIX}${BIT_NUM}[fPIC]
#   fPICは非WindowsにおいてSharedビルドする時自動的に付加する。
#   これ以外のフォルダを指定する時は、BOOST_PREFIXを""にしてBOOST_ROOTを設定する

set(BOOST_PREFIX "$ENV{HOME}/install/boost")

# llvmのルート・フォールダ(ドライバをビルドしない時は未指定でOK)
#   msvc   : ${LLVM}/${COMPILER}x${BIT_NUM}
#   その他 : ${LLVM}/${COMPILER}x${BIT_NUM}-${CONFIG_TYPE}
#   これ以外のフォルダを指定する時は、LLVMを""にしてLLVM_ROOTを設定する

#set(LLVM "$ENV{HOME}/install/llvm390")

# LLVMのバージョン
#   複数のLLVMがシステムにインストールされている時に指定する
#set(LLVM_VERSION 3.9.0)

# 必須CMakeバージョン
set(CMAKE_VERSION 3.5.0)

# MinGW/gccのbinフォルダのパス
set(CC32 "/usr/bin")
set(CC64 "/usr/bin")

# MinGW/gcc用のMakeのパス名(未指定ならmakeを使用する)
#set(MAKE "/usr/bin/make")

# make時の並列処理数を指定(msvc、jom使用時は指定不要)
set(PARALLEL "-j8")

# 結果概要ファイル名
set(SUMMARY linux-${PROC_ALL}.log)

#-----------------------------------------------------------------------------
#       基本準備
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION ${CMAKE_VERSION})

include(tools/zz_prepare.cmake)

set(SUMMARY ${THEOLIZER_BINARY}${SUMMARY})
file(WRITE ${SUMMARY} "")

set(ENV{LC_ALL} "C")

#-----------------------------------------------------------------------------
#       各ビルド・フォルダの準備とビルド・テスト・インストール
#
#       build_by_gcc(COMPILER BIT_NUM LIB_TYPE BUILD_DRIVER BUILD_DOCUMENT RELEASE_LIST DEBUG_LIST)
#           COMPILER        msvc2015/mingw540/gcc540
#           BIT_NUM         64/32
#           LIB_TYPE        StaticWithBoost/Static/Shared
#           BUILD_DRIVER    TRUE/FALSE
#           RELEASE_LIST    CTest結果の正しいテスト数のリスト(Release用)
#           DEBUG_LIST      CTest結果の正しいテスト数のリスト(Debug用)
#-----------------------------------------------------------------------------

output_title("****** StaticWithBoost ******")
build_by_gcc(gcc540 64 StaticWithBoost TRUE TRUE "37 1" "11 1")

output_title("****** Static ******")
build_by_gcc(gcc540 64 Static FALSE FALSE "37 1" "11 1")

output_title("****** Shared ******")
build_by_gcc(gcc540 64 Shared FALSE FALSE "37 1" "11 1")

output_summary()
