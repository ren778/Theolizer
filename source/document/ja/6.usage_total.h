﻿//############################################################################
/*!
    @brief      ドキュメント・ファイル－使用方法（全体）
    @ingroup    Documents
    @file       6.usage_total.h
    @author     Yoshinori Tahara
    @date       2016/11/01 Created
*/
/*
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
*/
//############################################################################

/*!
    @page UsageTotal 使用方法（全体）

ここでは、Theolizerの使い方を説明します。

TheolizerはAPIの機能テストをできるだけ自動化してます。<br>
そして、APIの機能テストは詳細仕様を規定することでもあります。複雑なプログラムでは文書として読み取ることは困難ですが、単純なプログラムであれば、文書としての機能を果たせるのではないかと考えています。

そこで、各機能についてそれをテストするための多数のテスト群の内、代表的なものを使って説明し、詳細テストの関連部分のソースも提示します。できるだけ読み取るのに苦労しない形式でプログラムを記述し、詳細仕様書として有用であることを目指します。

まず、1～3節で全体的な説明を行い、4節以降で各機能について説明します。

<br>
//############################################################################
@section Naming 1.名前の付け方
//############################################################################

TheolizerのAPIは名前空間に入れています。また、マクロは決まったプリフィクスを付けています。
少し意味を持たせていますので説明します。

// ***************************************************************************
@subsection Namespace 1-1.名前空間
// ***************************************************************************

ほぼ全てのシンボルを <b>theolizer</b> 名前空間へ入れています。<br>
一部、入れると実装が難しいものについては、シンボルに <b>Theolizer</b> を含みます。<br>
これにより既存のコードと被ることはまずない筈です。

次に、内部的に使用するシンボルは <b>internal</b> 名前空間へ入れています。<br>
これらのシンボルが付けられたクラスや関数等は、Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。

// ***************************************************************************
@subsection MacroName 1-2.マクロ名
// ***************************************************************************

全てのマクロは <b>THEOLIZER_</b> で始めています。<br>
これにより既存のコードと被ることはまずない筈です。

次に、内部的に使用するマクロは、 <b>THEOLIZER_INTERNAL_</b> で始めています。<br>
また、Theolizerが自動生成するマクロを、 <b>THEOLIZER_GENERATED_</b>  で始めています。<br>
これらのマクロは、Theolizerのアップデート時、上位互換性を考慮しませんので使用しないようお願いします。
<br>
//############################################################################
@section BasicUsage 2.基本的な使い方
//############################################################################

Theolizerをインストールした後、あなたのデータをTheolizerでシリアライズするために、あなたのプログラムを下記の順序を守るようにして下さい。

1. Theolizerヘッダをインクルード
2. シリアライズするクラスとenum型の定義
3. Theolizerの自動生成ファイルをインクルード
4. シリアライズ処理

// ***************************************************************************
@subsection TheolizerHeader 2-1.Theolizerヘッダをインクルード
// ***************************************************************************

一部例外(*1)はありますが原則として、<b>シリアライズするクラスとenum型の定義</b> 前にシリアライザのヘッダをインクルードして下さい。<br>

<b>Json形式シリアライザ・ヘッダのインクルードと型定義の例：(source/samples/example/example.h）</b>

@dontinclude example.h
@skip ********
@until };
@until };

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
(*1) 例外：<b>非侵入型</b>(@ref Basic12)のクラスやenum型の<b>非侵入型完全自動</b>については、<b>シリアライザのヘッダをインクルード</b>する前でも定義できます。<br>
</div>

現在、サポートしているシリアライザはJson形式、独自Binary形式、メモリ内専用のFast形式の3種類です。
それぞれ、下記ヘッダをインクールドして下さい。


|形式|インクルードするヘッダ|注意事項|
|----|----------------------|--------|
|Json形式|<theolizer/serializer_json.h>|fstreamはテキスト・モードでオープンする|
|独自Binary形式|<theolizer/serializer_binary.h>|fstreamは<b>バイナリ・モード(std::ios_base::binary)</b>でオープンする|
|メモリ内専用Fast形式|上記のどちから、もしくは、<theolizer/serializer.h>|fstreamは<b>バイナリ・モード(std::ios_base::binary)</b>でオープンする|
<br>
<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
@anchor バイナリ・モード
<b>バイナリ形式における注意事項：</b><br>
バイナリ形式のシリアライザをfstreamで用いる時は、必ずバイナリ・モード（std::ios_base::binary）でfstreamをオープンして下さい。<br>
fstreamがテキスト・モードでオープンされている場合、ファイル・ストリームへ数値26(0x1A)が出力されると、これはEOFコードなので回復時にEOFエラーになります。また、Windowsで数値10(0x0A)が出力されるとCR LFへ展開されてしまい、適切に回復できません。<br>
このような事態をさけるため、Theolizer側でエラーにしたいのですが、iostreamではそのオープン・モードを確認できないためチェックが困難なのです。<br>
</div>
<br>
// ***************************************************************************
@subsection DefineClassEnum 2-2.シリアライズするクラスとenum型の定義
// ***************************************************************************

<b>シリアライズするクラスとenum型の定義</b>後に、<b>Theolizerの自動生成ファイルをインスクルード</b> します。この順序に例外はありません。<br>

なお、シリアライズ対象の型を含まないヘッダ・ファイルは、Theolizerに関する順序制限はありません。

<b>クラスとenum型定義例：(source/samples/example/example.h）</b>

// ***************************************************************************
@subsection IncludeAutoGeneratedFile 2-3.Theolizerの自動生成ファイルをインクルード
// ***************************************************************************

@ref Mechanism で説明したようにTheolizerはシリアライズに必要なソース・コードを自動生成します。<br>
このファイルはコンパイル単位（通常は.cppファイル）毎に生成され、当該.cppファイルと同じフォルダへ自動的に生成されます。<br>
Theolizerは、バージョンを上げた時に古いクラス定義やenum型定義をここに残します。これにより古いシリアライズ・データを回復できます。<br>
そこで、このファイルをお使いのバージョン管理システム（gitやsvn等）へ登録することをお勧めします。

自動生成するファイルのファイル名は、そのコンパイル単位のファイル名に".theolzier.hpp"を繋げたものです。<br>
例えば、<b>example.cpp</b> の場合は、<b>example.cpp.theolizer.hpp</b> となります。

<b>例：(source/samples/example/example.cpp）</b>

@dontinclude example.cpp
@skip include
@until example.cpp.theolizer.hpp

// ***************************************************************************
@subsection SerializingProcess 2-4.シリアライズ処理
// ***************************************************************************

シリアライズは下記の３つで行います。
  - シリアライザ・インスタンスの生成
  - シリアライズ処理要求（シリアライズするインスタンスを指定する）
  - シリアライザ・インスタンスの破棄

@subsubsection ConstructSerializer 2-4-1.シリアライザ・インスタンスの生成
その際、シリアライズ先のデータ・ストリームを指定します。ファイル・ストリームや[TCP/IPストリーム](http://localhost/theolizer/tcp_ip_sample/)を指定して下さい。<br>
保存や送信時はstd::ostreamを、回復や受信時はstd::istreamを与えて下さい。

 - <b>保存（送信用）シリアライザ</b> <br>
    theolizer::JsonOSerialzier<br>
    theolizer::BinaryOSerialzier<br>
    theolizer::FastOSerialzier<br>

 - <b>回復（受信用）シリアライザ（デシリアライザ）</b> <br>
    theolizer::JsonISerialzier<br>
    theolizer::BinaryISerializer<br>
    theolizer::FastISerialzier<br>

@subsubsection Request 2-4-2.シリアライズ処理要求
下記マクロでシリアライズします。保存用シリアライザを指定すると保存、回復用シリアライザを指定すると回復動作となります。<br>
これはいつくでも記述して良いです。

<b>THEOLIZER_PROCESS(dSerializer, dInstance)</b>
  - dSerializer : シリアライザのインスタンスを指定します。<br>
  - dInstance   : 保存／回復するインスタンスを指定します。<br>

dInstanceを保存／回復します。<br>
ポインタ型を指定した場合は、アドレス回復のためにポイント先のオブジェクト追跡を行います。<br>

<b>THEOLIZER_PROCESS_POINTEE(dSerializer, dInstance)</b>
  - dSerializer : シリアライザのインスタンスを指定します。<br>
  - dInstance   : 保存／回復する被ポインタでもあるインスタンスを指定します。<br>

<b>THEOLIZER_PROCESS_OWNER(dSerializer, dInstance)</b>
  - dSerializer : シリアライザのインスタンスを指定します。<br>
  - dInstance   : 保存／回復する所有権を持つインスタンスへのポインタを指定します。<br>

@see @ref ObjectTracking

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
このようにシリアライザを問わず、全て同じマクロでシリアライズ処理できます。<br>
xmlも将来的にサポートする予定ですが、その際も同じマクロで対応できる筈です。<br>
（マクロなので、\#演算によりインスタンス名をC言語文字列として取り出して用います。）
</div>

@subsubsection DestructSerializer 2-4-3.シリアライザ・インスタンスの破棄
最後にシリアライザ・インスタンスを破棄することでシリアライズ処理を完了します。<br>
生成時に例外発生を禁止していた場合に、エラー状態をリセットしないまま破棄すると、プロセスをアボートします。これはエラー状態の見逃しを回避するための仕様です。<br>
例外発生を禁止している場合は、破棄する前には必ずエラー情報をチェクした上で、エラー状態をリセットして下さい。<br>
具体的手順は@ref ErrorReport にて説明します。

<b>ファイルへの保存例：(source/samples/example/example.cpp）</b>

@dontinclude example.cpp
@skip try
@skip {
@until }

<b>ファイルからの回復例：(source/samples/example/example.cpp）</b>

@skip try
@skip {
@until }

<br>
//############################################################################
@section Serializers 3.各シリアライザの説明
//############################################################################
現在、サポートしているシリアライザはJson形式、独自Binary形式、メモリ内専用のFast形式の3種類です。<br>
ここではそれぞれの使い方を説明します。

<br>
// ***************************************************************************
@subsection Serializer 3-1.共通事項
// ***************************************************************************

全てのシリアライザについて共通な事項について説明します。

@subsubsection CheckMode 3-1-1.型チェック・モード
Theolizerのシリアライザは回復時に型が一致していることをチェックできます。<br>
その方法としてシリライズ・データ内に「型名を保存する方法」と「型に割り当てたインデックス番号（TypeIndex）を保存する方法」の２種類を用意しています。

theolizer::CheckMode
|列挙値|意味|
|------|----|
|NoTypeCheck|型チェック無し|
|TypeCheck|型名による型チェック|
|TypeCheckByIndex|TypeIndexによる型チェック|

NoTypeCheckは型情報をシリライズ・データに含みませんのでデータ量が少ない場合の効率は良いです。しかし、メンバ名をヘッダではなくデータ側に含むためデータ量が多くなると効率は悪化します。<br>
TypeCheckはテキスト型の場合、データを目視確認し易いです。データ量が少ない時の効率はNoTypeCheckの次に良いです。<br>
TypeCheckByIndexはデータ量が多い時は３種のCheckModeの中で最大の効率を発揮します。

型情報等の管理データを下記のように記録します。
  - <b>シリアライズ・データのヘッダ部</b><br>
    <b>NoTypeCheck：</b>型情報を記録しません。<br>
    <b>TypeCheck：</b>クラスについて、各メンバのメンバ名（@link Basic121 名前対応@endlink時）と型名を記録します。<br>
    <b>TypeCheckByIndex：</b>全ての型について、TypeIndexに対応する型名を記録します。
    更にクラスについては、各メンバのメンバ名（@link Basic121 名前対応@endlink時）とTypeIndexを記録します。<br>
<br>

  - <b>シリアライズ・データの各データ部</b><br>
    <b>NoTypeCheck：</b>クラスについて、各メンバのデータとメンバ名（@link Basic121 名前対応@endlink時）をセットで記録します。<br>
    <b>TypeCheck：</b>THEOLIZER_PROCESS()マクロで指定したデータと共にその型名を記録します。<br>
    <b>TypeCheckByIndex：</b>THEOLIZER_PROCESS()マクロで指定したデータと共にそのTypeIndexを記録します。<br>

回復時に回復先の変数の型と上記の情報と照らし合わせることで型チェックを行います。<br>

@subsubsection MemberFunctions 3-1-2.メンバ関数
幾つかの制御のため、各シリアライザは下記のメンバ関数を公開しています。

|メンバ名|意味|
|--------|----|
|unsigned getGlobalVersionNo() const;|処理中のグローバル・バージョン番号を返却します。|
|void clearTracking() ;|オブジェクト追跡の区切り（@ref ObjectTracking 参照）|
|bool getRequireClearTracking() const;|clearTracking()が必要な時trueを返却します。|
|theolizer::CheckMode getCheckMode() const;|現在のCheckModeを返却します。|
|void setCharIsMultiByte();|Windowsにおいて、EncodedStringがtrueのシリアライザにおいて<br>std::string変数の文字エンコードをMultiByte文字列として処理するかどうかを指定します。|
|theolizer::ErrorInfo const& getErrorInfo() const;|エラー情報を返却します。|
|bool isError() const;|エラーが発生している時trueを返却します。|
|void resetError();|エラー状態を解除します。（@ref ErrorReport3 参照）|

@subsubsection Property 3-1-3.プロパティ
各シリアライザは、その属性をプロバティとして提供しています。

|プロバティ名|意味|Json|Binary|Fast|
|------------|----|----|------|----|
|IsSaver|保存処理用ならtrue、回復処理用ならfalse|--|--|--|
|EncodedString|文字列のエンコードを処理する|true|false|false|
|SupportModifying|クラスやenum型の定義変更に対応する|true|true|false|

プロパテイは以下の構文で受け取ります。

@code
bool property = <シリアライザ・クラス>::hasProperty(theolizer::Property::<プロパティ名>);
@endcode
<br>

@subsubsection EncodedString 3-1-4.EncodedStringについて補足
テキスト型のシリアライザは、文字列を読める形式で記録されます。<br>
そのため、例えばJsonフォーマットはデフォルトではUTF-8でエンコードすると規定されています。<br>
そして、C++の各`std::string`シリーズもUnicodeでエンコードされることが期待されます。（そうしないことも可能です。）

そこで、TheolizerのEncodedStringプロパティをサポートしたシリアライザ（現在はJsonのみ）は、下記のように`std::string`シリーズを処理します。

- <b>setCharIsMultiByte(false)</b>（デフォルト）<br>
全ての文字列を特定のUnicodeエンコードで記録する。(JsonシリアライザはUTF-8)<br>
`std::string`はUnicodeでエンコードされているものとして処理する。（そのまま保存／回復する。）<br>
以下は全てUnicodeへ変換して保存し、読み出し後当該エンコードへ変換する。<br>
`std::wstring`はUTF-16かUTF-32(wchar_tのサイズによる）でエンコードされているものとして処理する。<br>
`std::u16string`はUTF-16でエンコードされているものとして処理する。<br>
`std::u32string`はUTF-32でエンコードされているものとして処理する。<br>
<br>

- <b>setCharIsMultiByte(true)</b><br>
std::stringはMultiByte文字列としてエンコードされているものとして処理する。(Unicodeへ変換して保存／回復する。）<br>
その他は、デフォルトと同じ。<br>

つまり、例えば、UTF-8でエンコードされた`std::string`型の変数を保存し、それをUTF-16でエンコードされた`std::u16string`型の変数へ回復できます。

<b>サンプル・ソース（source/reference_and_test/basic/test_basic_process.cpp）</b><br>

@dontinclude test_basic_process.cpp
@skip #if defined(_WIN32)
@until #endif
@skip void tutoriseBasic()
@until {
@skip //-
@until // char
@until THEOLIZER_PROCESS(aSerializer, aString);
@until THEOLIZER_PROCESS(aSerializer, aString);
@until THEOLIZER_PROCESS(aSerializer, aString);
@skip }
@until }
@until THEOLIZER_EQUAL(aU32String
@skip }
@until }
@skip "tutoriseBasic() end"
@skip }
@until }

<br>
// ***************************************************************************
@subsection JsonSerializer 3-2.Json形式（JsonSerializer）
// ***************************************************************************
Json形式でシリアライズする場合は、<b>theolizer/serializer_json.h</b>をインクルードして下さい。

@subsubsection JsonOSerialzier 3-2-1.保存用JsonSerialzier
@link theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,unsigned,CheckMode,bool,bool)
@copybrief theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,unsigned,CheckMode,bool,bool)
@endlink

@code
JsonOSerializer
(
    std::ostream& iOStream,
    unsigned iGlobalVersionNo=kLastGlobalVersionNo,
    CheckMode iCheckMode=CheckMode::NoTypeCheck,
    bool iNoPrettyPrint=false,
    bool iNoThrowException=false
);
@endcode

@link theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,CheckMode,bool,bool)
@copybrief theolizer::JsonOSerializer::JsonOSerializer(std::ostream&,CheckMode,bool,bool)
@endlink

@code
JsonOSerializer
(
    std::ostream& iOStream,
    CheckMode iCheckMode,
    bool iNoPrettyPrint=false,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iOStream|出力先のストリーム(ofstreamはテキスト・モードでオープンして下さい）|
|iGlobalVersionNo|保存するグローバル・バージョン番号(省略時は最新版)|
|iCheckMode|型チェック・モード（省略時はNoTypeCheck)|
|iNoPrettyPrint|整形出力しない時true（省略時はfalse)|
|iNoThrowException|例外禁止時true（省略時はfalse)|

#### 専用継承関数

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>void setCharIsMultiByte(bool iCharIsMultiByte);</b><br>
iCharIsMultiByte=trueの時、std::stringをWindowsマルチ・バイト文字コードとして取り扱います。<br>
コンストラクト直後はiCharIsMultiByte=falseです。<br>
Windowsのみ機能します。それ以外のOSではUTF-8のままです。
</div>

@subsubsection JsonISerialzier 3-2-2.回復用JsonSerialzier
@link theolizer::JsonISerializer::JsonISerializer(std::istream&,bool)
@copybrief theolizer::JsonISerializer::JsonISerializer(std::istream&,bool)
@endlink

@code
JsonISerializer
(
    std::istream& iIStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iIStream|入力元のストリーム(ofstreamならテキスト・モードでオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

#### 専用継承関数

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>void setCharIsMultiByte(bool iCharIsMultiByte);</b><br>
iCharIsMultiByte=trueの時、std::stringをWindowsマルチ・バイト文字コードとして取り扱います。<br>
コンストラクト直後はiCharIsMultiByte=falseです。<br>
Windowsのみ機能します。それ以外のOSではUTF-8のままです。
</div>

<br>
// ***************************************************************************
@subsection BinarySerializer 3-3.独自Binary形式（BinarySerializer）
// ***************************************************************************
独自Binary形式でシリアライズする場合は、<b>theolizer/serializer_binary.h</b>をインクルードして下さい。<br>
また、ファイル・ストリームは必ず@ref バイナリ・モード でオープンして下さい。

Big Endianでエンコードします。Little Endianの処理系の場合Big Endianとの間で自動変換します。<br>
整数型は値を表現するために十分なバイト数で保存します。例えば、long long型でも値が10ならタグと値で合わせて2バイトで保存します。<br>
浮動小数点型はIEEE754フォーマットのみサポートします。バイト単位でEndian変換します。<br>
long doubleは「radix==2、digits==64、max_exponent==16384」の80ビット拡張精度形式である処理系（gcc）とbinary64である処理系(msvc)に対応しています。<br>
文字コードの変換は行いません。Endianのみ変換してシリアライズします。<br>

@subsubsection BinaryOSerialzier 3-3-1.保存用BinarySerialzier
@link theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,unsigned,CheckMode,bool)
@copybrief theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,unsigned,CheckMode,bool)
@endlink

@code
BinaryOSerializer
(
    std::ostream& iOStream,
    unsigned iGlobalVersionNo=kLastGlobalVersionNo,
    CheckMode iCheckMode=CheckMode::NoTypeCheck,
    bool iNoThrowException=false
);
@endcode

@link theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,CheckMode,bool)
@copybrief theolizer::BinaryOSerializer::BinaryOSerializer(std::ostream&,CheckMode,bool)
@endlink

@code
BinaryOSerializer
(
    std::ostream& iOStream,
    CheckMode iCheckMode,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iOStream|出力先のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iGlobalVersionNo|保存するグローバル・バージョン番号(省略時は最新版)|
|iCheckMode|型チェック・モード（省略時はNoTypeCheck)|
|iNoThrowException|例外禁止時true（省略時はfalse)|

@subsubsection BinaryISerializer 3-3-2.回復用BinarySerialzier
@link theolizer::BinaryISerializer::BinaryISerializer(std::istream&,bool)
@copybrief theolizer::BinaryISerializer::BinaryISerializer(std::istream&,bool)
@endlink

@code
BinaryISerializer
(
    std::istream& iIStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iIStream|入力元のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

<br>
// ***************************************************************************
@subsection FastSerializer 3-4.メモリ内専用のFast形式（FastSerializer）
// ***************************************************************************
FastSerializerの使用目的はデータ構造のプログラム内コピーです。外部プログラムとのデータ交換は想定していません<br>

Theolizerが内部的に使用していますので、他のシリアライザのヘッダをインクルードすれば改めてヘッダをインクルードする必要はありません。<br>
もし、他のシリアライザを使用しない時は、theolizer/serializer.hをインクルードして下さい。<br>
また、ストリームはstd::stringstreamを用いることを想定していますが、もしも、ファイル・ストリームを与える場合は必ず@ref バイナリ・モード でオープンして下さい。<br>
FastSerializerはデータ変換しません。バージョンの相違にも対応していません。<br>
オーナー指定ポインタでない通常のポインタは、ポイント先をシリアライズしていない場合はシャロー・コピーになります。（ポインタ値を単純にコピーする。）<br>

@subsubsection FastOSerializer 3-4-1.保存用FastSerializer
@link theolizer::FastOSerializer::FastOSerializer(std::ostream&,bool)
@copybrief theolizer::FastOSerializer::FastOSerializer(std::ostream&,bool)
@endlink

@code
FastOSerializer
(
    std::ostream& iOStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iOStream|出力先のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

@subsubsection FastISerializer 3-4-2.回復用FastSerializer
@link theolizer::FastISerializer::FastISerializer(std::istream&,bool)
@copybrief theolizer::FastISerializer::FastISerializer(std::istream&,bool)
@endlink

@code
FastISerializer
(
    std::istream& iIStream,
    bool iNoThrowException=false
);
@endcode

|パラメータ名|意味|
|------------|----|
|iIStream|入力元のストリーム(ofstreamは<b>@ref バイナリ・モード </b>でオープンして下さい）|
|iNoThrowException|例外禁止時true（省略時はfalse)|

#### FastSerializerを用いたグローバル関数

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>template<typename tType><br>
void copySerializable(tType const& iSource, tType& oDestination);</b><br>
tType型の変数iSourceをoDestinationへコピーします。<br>
iSourceをFastSerializerでメモリ・ストリームへシリアライズし、続けてoDestinationへ回復することでコピーします。<br>
保存先指定も有効ですので、柔軟なデータ構造のコピーを容易に実装できます。
</div>

<br>
//############################################################################
@section TestProgram 4.テスト・プログラムの構造
//############################################################################

<br>
// ***************************************************************************
@subsection TestProgram41 4-1.テスト・プログラムの全体構造
// ***************************************************************************

@subsubsection TestProgram411 4-1-1.アップデートとバージョンのバリエーション

主な機能テスト・プログラムは、 <b>source/reference_and_test</b> 以下にアップデート／バージョン・アップによる変更（修正版）毎にフォルダを分けて保存しています。

Theolizerはenum型とclass/struct型について定義変更に対応しています。そのテストも行うため、以下のように分類してテストを行います。

1. 基本的なシリアライズ機能の定義変更を除く様々なバリエーションのテスト

2. バージョン番号を更新しない定義変更のテスト

3. バージョン番号を更新する定義変更のテスト

現時点では下記修正版を用意しています。

|フォルダ名|バージョン番号|説明|
|----------|--------------|----|
|basic|無し|基本的なシリアライズ機能の定義変更を除く様々なバリエーションのテスト|
|basic2|無し|同上|
|ver1a|1|最初のバージョン|
|ver1b|1|バージョン番号を変えずに可能な定義変更のテスト|
|ver1c|1|バージョン番号を変えるための準備のテスト|
|ver2a|2|バージョン番号を変更した時の定義変更のテスト|
|ver3a|3|更にバージョン番号を変更のテスト|
|ver3b|3|そしてバージョン番号を変えずに定義変更のテスト|

class/structでメンバ変数を削除した場合、１つ前のバージョンの自動生成ソースに削除されたことを反映します。なので、最新版と１つ前のバージョンと、更にもう１つ前に問題が出ないかテストするため、３つのバージョンでテストします。

各変更テスト・プログラムは他の変更テスト・プログラムが出力したデータを読み込んで回復できることやエラーを検出できることをテストします。

@subsubsection TestProgram412 4-1-2.テストの組み合わせ

下記組み合わせとなります。１行毎に各修正版プログラムが出力するデータ概要を記述しています。<br>
列に記載した各バージョンのプログラムが当該データの回復テストをする時◯印を付けています。

|プロ<br>グラ<br>ム|バー<br>ジョン<br>指定|ファイル名|basic|basic2|ver1a|ver1b|ver1c|ver2a|ver3a|ver3b|
|----------|--------------|----------|-----|-----|-----|-----|-----|-----|-----|-----|
|basic |指定無し |①-basic-② |◯   |     |     |     |     |     |     |     |
|basic |1 |①-basic-basic-②  |◯   |     |     |     |     |     |     |     |
|basic2|指定無し |①-basic2-②|     |◯   |     |     |     |     |     |     |
|basic2|1 |①-basic-basic2-② |     |◯   |     |     |     |     |     |     |
|ver1a |指定無し |①-ver1a-② |     |     |◯   |◯   |◯   |◯   |◯   |◯   |
|ver1a |1 |①-ver1a-ver1a-②  |     |     |◯   |◯   |◯   |◯   |◯   |◯   |
|ver1b |指定無し |①-ver1b-② |     |     |     |◯   |◯   |◯   |◯   |◯   |
|ver1b |1 |①-ver1b-ver1b-②  |     |     |     |◯   |◯   |◯   |◯   |◯   |
|ver1c |指定無し |①-ver1c-② |     |     |     |     |◯   |◯   |◯   |◯   |
|ver1c |1 |①-ver1c-ver1c-②  |     |     |     |     |◯   |◯   |◯   |◯   |
|ver2a |指定無し |①-ver2a-② |     |     |     |     |     |◯   |◯   |◯   |
|ver2a |1 |①-ver1c-ver2a-②  |     |     |     |     |◯   |◯   |◯   |◯   |
|ver2a |2 |①-ver2a-ver2a-②  |     |     |     |     |     |◯   |◯   |◯   |
|ver3a |指定無し |①-ver3a-② |     |     |     |     |     |     |◯   |◯   |
|ver3a |1 |①-ver1c-ver3a-②  |     |     |     |     |◯   |◯   |◯   |◯   |
|ver3a |2 |①-ver2a-ver3a-②  |     |     |     |     |     |◯   |◯   |◯   |
|ver3a |3 |①-ver3a-ver3a-②  |     |     |     |     |     |     |◯   |◯   |
|ver3b |指定無し |①-ver3b-② |     |     |     |     |     |     |     |◯   |
|ver3b |1 |①-ver1c-ver3b-②  |     |     |     |     |◯   |◯   |◯   |◯   |
|ver3b |2 |①-ver2a-ver3b-②  |     |     |     |     |     |◯   |◯   |◯   |
|ver3b |3 |①-ver3b-ver3b-②  |     |     |     |     |     |     |     |◯   |

①にはシリアライザ名と一部のオプションが入ります。

- json-np (非整形出力)
- json-pp (整形出力)
- binary
- fast

②は以下の通りです。

- 残りのオプション指定
  - jsonとbinaryの場合
    - NoTypeCheck
    - TypeCheck
    - TypeCheckByIndex
  - fastの場合は、最新版のみ対応でオプションもないため、上記表の「指定無し」のみです。
<br><br>

- 保存先指定用のサフィックス
  - 無印
  - A(保存先DestA)
  - B(保存先DestB)
  - AB(保存先DestA | DestB)
<br><br>

- 拡張子
  - jsonの拡張子はjson
  - binaryとfastの拡張子はbin

例えば、json整形出力で、ver3aのプログラムがver1cデータを型チェック無し、保存先指定無しで出力したファイル名は、"json-pp-ver1c-ver3a-NoTypeCheck.json"となります。

<br>
// ***************************************************************************
@subsection TestProgram42 4-2.テスト・プログラムの構造
// ***************************************************************************

basic、および、各変更テスト用プログラムは共通部分があります。それらはreference_and_testフォルダ直下に配置し、ビルドする時に各サブ・フォルダへコピーしています。

### 共通部

|ファイル|関数|概要|
|--------|----|----|
|disable_test.h ||各個別テストをディセーブルするシンボル定義。<br>デバッグ時の便利のために用意。|
|all_common.h ||テスト用の全バージョン共通定義。<br>アップデートとバージョン名とバージョン番号対応表のgVersionListを定義。|
|main.inc |main() |各サブ・フォルダ内のmain.cppから#includeされる。<br>コマンドライン解析を行い、パラメータが無い時は保存処理、ある時は回復処理を実行する。<br>4-1節 表の全組み合わせを生成し、callTests()を呼び出す。|
|↑|callTests()|各シリアライザのパラメータを振ってインスタンスを生成し、<br>saveBasic(), loadBasic(),callSaveDestinations(),callLoadDestinations()を呼び出す。|

### 各サブフォルダ部

|ファイル|関数|概要|
|--------|----|----|
|main.cpp ||各個別フォルダ内テスト関数を呼び出す。|
|↑|saveBasic()|自動テスト基本部の保存処理。個別テストを呼び出す。|
|↑|loadBasic()|自動テスト基本部の回復処理。個別テストを呼び出す。|
|↑|callSaveDestinations()|自動テスト保存先指定部の保存処理。個別テストを呼び出す。|
|↑|callLoadDestinations()|自動テスト保存先指定部の回復処理。個別テストを呼び出す。|

各個別テストは別途*.cppファイルを用意し、その中で定義しています。<br>
それぞれについては @ref UsageIndividual にて解説します。

<br>
// ***************************************************************************
@subsection TestProgram43 4-3.説明で用いるマクロについて
// ***************************************************************************
テスト用のマクロはtest_tool.hで定義しています。<br>
その内、使い方の説明（兼 自動テスト）で用いるマクロについてここで簡単に説明します。

1.@ref THEOLIZER_EQUAL(dLhs, dRhs, ...)

(dLhs == dRhs) ならばPASS、そうでないならFAILと判定します。<br>
PASSならば、テストの数とPASS数をインクリメントします。<br>
FAILならば、テストの数とFAIL数をインクリメントし、テストを失敗させます。<br>
また、dRhsとそれ以降のアイテム（1個以上7個まで）を標準出力へ出力します。

下記はシリアライザを使って回復したint型のaIntの値が-3000であることをチェックしています。

@dontinclude basic/test_basic_process.cpp
@skip aInt=0;
@until THEOLIZER_EQUAL

*/
