//############################################################################
/*!
    @file prepare.h
    @brief  ドキュメント・ファイル－Theolizerによる開発の準備
    @author Yoshinori Tahara
    @date   2016/11/01  Created
*/
/*
    Copyright (c) 2016 Yohinori Tahara(Theoride Technology) - http://theolizer.com/

    商用ライセンス
        有効なTheolizer商用ライセンスを保持している人は、
        田原良則(Theoride Technology)と締結している商用ライセンス契約の
        使用条件に従って、このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        GPLv3のコピーをLICENSE.TXTファイルにおいてます。
*/
//############################################################################

/*!
    @page Prepare インストール／アンインストール

@section Install 1.インストール手順

---

下記2つの手順で行います。

1. Theolizerの用意
2. ドライバのリプレース

## 1-1.Theolizerの用意
プリビルド版の場合は、GitHubの[relases](https://github.com/yossi-tahara/Theolizer/releases)からダウンロードしてお好きなフォルダへ解凍して下さい。

ソース版の場合は、GitHubからソースをダウンロード、もしくは、ローカルへclone後、@ref HowToBuildLibrary の手順に従ってビルドとインストールを行って下さい。指定のフォルダへTheolizerがインストールされます。

Theolizerを解凍、もしくは、インストールしたフォルダを以下<b>Theolizerルート・フォルダ</b>と呼びます。）

## 1-2.ドライバのリプレース

<b>Theolizerルート・フォルダ</b>/binにてコマンド・プロンプト(Windows) かターミナル（linux）を起動し、下記を実行します。

    TheolizerDriver "--theolizer-replace=コンパイラのパス"

ターゲットのコンパイラが、<b>コンパイラ名RenamedByTheolizer</b>へリネームされ、TheolizerDriverが<b>コンパイラ名</b>へコピーされます。<br>
コンパイラを置き換えますが、これによる悪影響はない筈です。@ref Mechanism の「ドライバ動作の補足」をご参照下さい。<br>
（もし、悪影響か見つかった場合は、お手数ですがその再現手順を含めて当リボジトリのIssuesへご報告頂けますようお願い致します。）

例えば、<b>`C:\mingw-w64\x86_64-5.4.0-posix-seh-rt_v5-rev0\mingw64\bin`</b>にMinGW 5.4.0がインストールされている場合、下記コマンドでにてコンパイラをリプレースします。

    TheolizerDriver.exe "--theolizer-replace=C:\mingw-w64\x86_64-5.4.0-posix-seh-rt_v5-rev0\mingw64\bin\g++.exe"

<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
Visual Strudio C++については、<b>Theolizerルート・フォルダ</b>にreplace.batを用意しています。これを起動することで上記のリプレース操作を行います。
</div>

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
linuxにてコンパイラのパスとしてシンボリック・リンクを指定した場合、シンボリック・リンクを追跡してコンパイラ実体のファイルを置き換えます。
</div>


<br>
@section Uninstall 2.アンインストール手順

---

下記2つの手順で行います。

1. ドライバのリストア
2. Theolizerの削除

## 2-1.ドライバのリストア

リプレース時のreplaceをrestoreへ置き換えて操作して下さい。

    TheolizerDriver "--theolizer-restore=コンパイラのパス"

コンパイラのファイル名にてコピーされていた<b>TheolizerDriver</b>が削除され、<b>コンパイラ名RenamedByTheolizer</b>が元の<b>コンパイラ名</b>へリネームされます。また、もし、指定したコンパイラがTheolizerDriverでなかった場合は何もしません。

<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
 Visual Strudio C++については、<b>Theolizerルート・フォルダ</b>にrestore.batを用意しています。これを起動することで上記のリストア操作を行います。
</div>

## 2-2.Theolizerの削除
ドライバのリストア確認後、<b>Theolizerルート・フォルダ</b>をフォルダごと、削除して下さい。

*/
