//############################################################################
/*!
    @brief      ドキュメント・ファイル－仕様
    @ingroup    Documents
    @file       2.specification.h
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
    @page Specification 仕様

<br>
//############################################################################
@section Basic 1.基本仕様
//############################################################################

---

<b>まず、「シリアライズ」という用語について</b><br>
シリアライズ(Serialize)の逆はデシリアライズ(Deserialize)なのですが、この資料ではデシリアライズも含めて「シリアライズ」と呼んでいます。<br>
シリアライザも同様にデシリアライザを含めて呼んでいます。<br>
両方を含めた用語が欲しいため、このようにしました。ご了承下さい。<br>
区別したい時は、シリアライズを保存、デシリアライズを回復と呼んでいます。<br>

// ***************************************************************************
@subsection Basic11 1-1.シリアライズ可能な型
// ***************************************************************************
__① プリミティブ型__<br>
基本的な型をプリミティブ型と呼んでいます。下記がプリミティブ型です。
- C++のint型等の基本型全て<br>
bool, char, signed char, unsigned char, wchar_t, char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long, float, double, long double
- std::string, std::wstring, std::u16string, std::u32string

__② enum型__<br>
通常のenum型、および、scoped enum型の両方に対応しています。

__③ class、struct__<br>
class、および、structは同じ扱いで、両方に対応しています。

__④ C++静的配列__<br>
①～④のC++静的配列に対応しています。（多次元配列も対応しています。）

__⑤ ポインタ型__<br>
①～④へのポインタです。
デシリライズ時、アドレスのみ回復します。

__⑥ オーナー・ポインタ型__<br>
①～④へのポインタで、ポイント先メモリ領域の管理（獲得／開放）のために使われているポインタです。<br>
 シリアライズ時にポインタが指していた先のメモリ領域も、デシリアライズ時に回復します。<br>
⑤とは異なることをTheolizerへ通知するため、ソース上で指定します。これを<b>オーナー指定</b>と呼びます。下記マクロで指定します。<br>
  - THEOLIZER_ANNOTATE(FS:...<...>Owner)
  - THEOLIZER_PROCESS_OWNER

__● シリアライズできないもの__<br>
下記はシリアライズに対応していません。
- **union**<br>
内容の判定が困難なため対応を断念しました。
- **C言語文字列**<br>
C言語の文字列型(NULL終端の`char*`)はデシリアライズ処理のメモリ獲得方法を決定できないため、サポートしていません。<br>
`char*`型は1文字へのポインタとして処理します。
-   **多重ポインタ**<br>
ポインタへのポインタ等の多重ポインタは非対応です。<b>オーナー指定</b>が複雑になるため断念しました。

// ***************************************************************************
@subsection Basic12 1-2.シリアライズ可能な型についての補足事項
// ***************************************************************************
@subsubsection Basic121 1-2-1.クラスについて
クラスは下記の３種類の形式に対応しています。
- **非侵入型完全自動**<br>
クラス定義をそのままで、自動的にシリアライズ処理するクラスです。<br>
インスタンスをTHEOLIZER_PROCESS()シリーズ・マクロでシリアライズするだけで使えます。<br>
シリアライズ対象クラスを派生して、派生したクラスでシリアライズ関数を実装するため、 <b>public</b> と <b>protected</b> メンバをシリアライズできます。<br>
（ <b>private</b> メンバはシリアライズできません。）<br>
バージョン番号は1固定です。<br>
<br>

- **侵入型半自動**<br>
クラス定義の内側でシリアライズすることを指定するクラスです。クラス内部でシリアライズ関数を定義するので <b>private</b> メンバもシリアライズできます。<br>
下記マクロで指定します。<br>
  - THEOLIZER_INTRUSIVE
  - THEOLIZER_INTRUSIVE_ORDER
<br>
<br>

- **非侵入型手動**<br>
クラス定義の外側でシリアライズすることを指定し、シリライズ処理を手動でコーディングするクラスです。 <b>public</b> メンバのみアクセス可能です。<br>
標準ライブラリのコンテナ型に対応することを主目的として実装した仕組みです。<br>
<br>

<b>デシリアライズ時のメンバ対応方法</b><br>
非侵入型完全自動と侵入型半自動は、自動的にメンバ変数を枚挙して、シリアライズします。<br>
そして、デシリアライズする際にシリアライズされたデータとメンバ変数の対応を取る方法として2つ用意しました。<br>

1. **名前対応**<br>
メンバ変数名が一致するものを回復します。メンバ変数の記述順序変更や、メンバ変数の並びの途中への追加／削除を行っても旧シリアライズ・データからクラス・インスタンスを回復できます。

2. **順序対応**<br>
メンバ変数の記述順に従って回復します。メンバ変数名をシリアライズ・データ内に記録しないのでデータ・サイズを減らせますが、メンバ変数の記述順序を変更できません。また、メンバ変数の追加／削除はメンバ変数の並びの最後のみ可能です。

<b>クラス・テンプレートのシリアライズについて</b><br>
Theolizerの内部的には（標準コンテナに対応するため）クラス・テンプレートにも対応しています。<br>
しかし、使い方が難しいだけでなく、自動テストの組み合わせも膨大になりそうです。そのため、当面はクラス・テンプレートは非公開と致します。<br>
将来的に自動テストを記述した後、公開したいと考えています。


@subsubsection Basic122 1-2-2. enum型について
enum型をシリアライズする際にどんな形式で保存するのか選択できます。<br>
例えば下記のようなenum型を想定します。

@code
enum Foo : long
{
    SymbolA=10;
    SymbolB=20;
};
@endcode

- シンボル名を保存する<br>
シリアライズ・データへ"SymbolA"や"SymbolB"と言うシンボル名を「文字列」で記録します。<br>
回復する時は、Theolizerが自動生成するenum型シンボル・リストと比較して、一致する文字列のシンボル値を当該enum型のインスタンスへ設定します。<br>
<br>

- シンボル値を保存する<br>
シリアライズ・データへ 10 や 20 と言うシンボル値を「long型」で記録します。<br>
回復する時は、Theolizerが自動生成するenum型シンボル・リストと比較して、一致するものがあればそれを当該enum型のインスタンスへ設定します。<br>

どちらの場合も一致する文字列や値が無い場合は、警告を出力します。

また、enum型は下記の２種類の形式に対応しています。
- **非侵入型完全自動**<br>
enum定義に対して何も指定しないでシリアライズ処理するenum型です。バージョン番号は1固定です。<br>
指定できるオプションはありません。（シンボル名保存固定です。）<br>
<br>

- **非侵入型半自動**<br>
enum型定義の外側でシリアライズすることを指定します。バージョン番号指定できます。<br>
更に下記オプションを指定できます。
  - 保存内容指定（シンボル名保存 or シンボル値保存）
  - シンボル毎に旧シンボル名、旧シンボル値（シンボル名やシンボル値を変更した時に用います）

@subsubsection Basic123 1-2-3.参照について

「参照」は、参照先インスタンスの別名として取り扱います。<br>
通常のインスタンスと同様に保存／回復可能です。<br>

また、ポリモーフィズムに対応しています。<br>
派生クラスのインスタンスを基底クラスで参照されていた場合、派生クラスを保存／回復します。ただし、保存時と異なる派生クラスを参照している場合はエラーとします。参照はその参照先を初期設定後、変更出来ないというC++仕様によるものです。

「@ref Basic24 」参照。

// ***************************************************************************
@subsection Basic13 1-3.保存先指定
// ***************************************************************************
クラスのメンバ変数に保存先を指定し、１つのクラスのメンバ変数を異なるファイルに保存したり、別途通信回線で送信したりできます。

事前にメンバ変数１つ毎に保存先を複数指定しておきます。<br>
そして、シリアライザ・オブジェクトのコンストラクト時も保存先を指定します。<br>
シリアライザに指定された保存先がメンバ変数に指定された保存先と一致した場合、そのメンバ変数は保存されます。なお、回復（デシリアライズ）は以下の通りです。
- 名前対応時は保存先が一致しなくても回復します。
- 順序対応時は保存先が一致するもののみ回復します。

上記名前対応時の仕組みにより、あるメンバ変数の保存先を異なるファイルへ変更した時でも（適切な順序でファイルから回復すれば）、当該メンバ変数を回復できます。

// ***************************************************************************
@subsection Basic14 1-4.現在対応しているフォーマット、STLなどについて
// ***************************************************************************
現在提供しているシリアライザは以下の通りです。
- **JsonOSerializer, JsonISerializer**<br>
Jsonフォーマットで保存します。<br>
std::stringにはUTF-8で記録して下さい。便利のために`theolizer::u8string`を提供しています。<br>
std::wstring, std::u16string, std::u32stringについても適切なUTF-xxコードにて記録して下さい。UTF-8へ変換して保存します。

- **BinaryOSerializer, BinaryISerializer**<br>
独自のバイナリフォーマットで保存します。文字列の文字コード変換は行わいません。そのまま保存／回復します。

- **FastOSerializer, FastISerializer**<br>
メモリ内専用のデータ・コピー用の特殊なシリアライザです。これは、クラス・インスタンスのメモリ内コピーのために用意しました。メモリ内でのコピーですので最新版のみサポートしています。また、ポインタが指す先のオブジェクトを保存しなかった場合、そのポインタの値は元の値をそのままコピーします。（@ref Tracking 参照）<br>
保存先を通常通り指定できますのでコピーするメンバとしないメンバを指定できます。更に、オーナー指定したポインタはデープ・コピーされ、そうでないポインタはシャロー・コピーされます。<br>
このシリアライザはTheolizer内部でも使用しています。

標準コンテナ（STL）は枚挙できないものを除き対応予定です。現在は下記に対応しています。
- **std::vector**  (デフォルト・アロケータのみ)
- **std::list** （デフォルト・アロケータのみ）

現在、下記のスマート・ポインタに対応しています。
- **std::unique_ptr** （デフォルト・デリータのみ）

// ***************************************************************************
@subsection Basic15 1-5.その他
// ***************************************************************************
@subsubsection Basic151 1-5-1.自動生成コードの生成場所
コンパイルしているファイル名に".theolizer.hpp"を加えた名前のファイルへ自動生成します。<br>
これをシリアライズ対象クラス定義とシリライズ処理（シリアライザのコンストラクト）の間で#includeして下さい。

また、下記マクロにシリアライザのインスタンスを渡すため、これらのマクロを使用する前にシリアライザをコンストラクトしておく必要が有ります。

- THEOLIZER_PROCESS
- THEOLIZER_PROCESS_TRACK
- THEOLIZER_PROCESS_OWNER

@subsubsection Basic152 1-5-2.シリアライズ処理を行っていないコンパイル単位について
ソース・ファイルの先頭に下記を記述することでドライバが解析処理をスキップしますので、コンパイル時間を短縮できます。

    #define THEOLIZER_NO_ANALYZE

@subsubsection Basic153 1-5-3.エラー報告について
デフォルトでは、エラーを検出したら、theolizer::ErrorInfo例外を投ます。シリアライザのコンストラクト時に、例外を投げないよう指定した場合、theolizer::ErrorInfo例外を投げません。<br>
ただし、全ての例外を投げないわけではありません。下記については必要であればユーザ側にて投げないよう設定下さい。
- iostreamが投げる例外
- 非侵入型手動のシリアライズ／デシリアライズ関数からのtheolizer::ErrorInfo以外の例外

最後に発生したエラーは、theolizer::ErrorReporter::getError()で受け取ります。

また、シリアライザはエラーが発生すると以降の処理要求は全て何もしません。頻繁にエラー・チェックしなくても実害がでないようにするためです。<br>
更に、例外禁止の時、エラー処理漏れを防ぐため、エラー状態のままシリアライザ・オブジェクトをデストラクトするとエラーを受け取っていないものとしてabort()します。<br>
abort()しないために`シリアライザのインスタンス.resetError();`でエラー状態をリセットして下さい。

@subsubsection Basic154 1-5-4.スレッド安全性
シリアライザのインスタンスはスレッド安全性を保証しません。<br>
同じシリアライザのインスタンスに対してTHEOLIZER_PROCESS等のマクロを呼び出す際の順序は、保存時と回復時で一致させておく必要が有るため、マルチ・スレッドから呼び出ししはいけないためです。

異なるシリアライザのインスタンス間ではスレッド安全性を保証しています。

@subsubsection Basic155 1-5-5.処理系間でのデータ交換について
例えばint型のサイズが異なる処理系間でint型データを確実に交換できるようにする機能はありません。<br>
これは原理的に不可能です。<br>
マルチプラットフォーム対応が必要な場合は、データ交換するプリミティブ型変数にはint32_t等のサイズが確定している型を用いて下さい。

// ***************************************************************************
@subsection Basic16 1-6.制約事項
// ***************************************************************************
@subsubsection Basic161 1-6-1.デフォルト・コンストラクタが必要なクラス
原則として、シリアライズするクラスにはデフォルト・コンストラクタは不要ですが、一部必要なものがあります。<br>
それはオーナー指定ポインタとしてシリアライズする非侵入型完全自動クラスと侵入型半自動クラスです。<br>

オーナー指定ポインタが以下のどちらかの場合に、デシリアライズする際に対象クラスをデフォルト・コンストラクタで生成するためです。・・・①
- nullptr
- シリアライズ・データ上のクラスAと異なるクラスBのインタンスをポイントしていた場合<br>
この場合は、ポイントしていたクラスBのインスタンスをdelete後にクラスAをnewします。

なお、①の条件の時、非侵入型手動についてはユーザ定義のloadClassManual()関数内でコンストラクトするため、ユーザ側にて呼び出すコンストラクタを決定できます。

@subsubsection Basic162 1-6-2.クラスのstatic変数について
これは自動シリアライズの対象としていません。

@subsubsection Basic163 1-6-3.仮想基底クラス
現在、仮想基底クラスは非対応です。<br>
オブジェクト追跡することで対応できる可能性はありますので、有用性が高い場合に対応を検討します。

@subsubsection Basic164 1-6-4.ポリモーフィズムにおける制約事項

現在のTheolizerでは[2点制約](https://github.com/yossi-tahara/Theolizer/issues/13)があります。<br>

1. 抽象クラスの非侵入型手動クラスに対応していません<br>
自動型（非侵入型完全自動クラスと侵入型半自動クラス）については抽象クラスでも問題ありません。通常通り派生してインスタンス生成すればシリアライズ可能です。<br>
しかし、非侵入型手動クラスが抽象クラスの時は派生してインスタンス生成した場合でも、シリアライズできません。対応は可能ですが、開発に時間がかかるため現時点では対応していません。<br>

2. 基底クラスを異なるクラスへ変更することはできません。<br>
基底クラスをバージョン・アップすることは可能ですが、例えば既存の別のクラスへ変更するような修正には対応していません。必要性は低いと思いますので、現時点では非対応とします。


<br>
//############################################################################
@section Tracking 2.オブジェクト追跡
//############################################################################

---

// ***************************************************************************
@subsection Basic21 2-1.オブジェクト追跡する領域について
// ***************************************************************************
ポインタは全てオブジェクト追跡します。そして、全てのポインタでない変数についてもオブジェクト追跡すれば確実にアドレス解決できるのですが、ポイントされていないものまで追跡するのは無駄が多いです。<br>
そこで、ポインタでない通常の変数は下記のように追跡指定するようにしました。

- 被ポインタ(THEOLIZER_ANNOTATE(FS:...<...>Pointee))指定されたメンバ変数、および、参照
- 被ポインタ(THEOLIZER_PROCESS_POINTEE)として保存指定された変数、および、参照

また、配列については、その1つ1つの要素をオブジェクト追跡します。配列全体は追跡しません。<br>
例えば、int foo[3];の時、foo[0], foo[1], foo[2]を追跡しますが、foo全体は追跡しません。<br>
foo[0]のアドレスとfoo全体の先頭アドレスは同じアドレスですが型が異なるため、異なるオブジェクトとしてとして取り扱うためです。

// ***************************************************************************
@subsection Basic22 2-2.オブジェクト追跡単位について
// ***************************************************************************
オブジェクト追跡はインタンス実体のアドレスを追跡しますので、ポインタの指すインスタンスが同じ<b>オブジェクト追跡単位</b>内に記録されている必要があります。<br>

その<b>オブジェクト追跡単位</b>は、下記の期間です。
- コンストラクト～clearTracking()
- clearTracking()～clearTracking()

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
<b>重要な注意事項：</b><br>
１つのオブジェクト追跡単位を保存している最中に、保存対象データを変更しないで下さい。シリアライズ処理に矛盾を生じることがあります。
</div>

// ***************************************************************************
@subsection Basic23 2-3.ポリモーフィズム対応
// ***************************************************************************
基底クラスのオーナー・ポインタ型に対して、派生クラス・インスタンスの回復が可能です。<br>
ポリモーフィズム対象の派生クラスをTHEOLIZER_REGISTER_CLASS()マクロで指定して下さい。<br>
なお、現在のところクラス・テンプレートはポリモーフィズム非対応です。<br>

// ***************************************************************************
@subsection Basic24 2-4.同じ領域を複数回シリアライズする時の動作について特記事項
// ***************************************************************************

同じインスタンスを複数回シリアライズするケースがあると思います。<br>
例えば、参照の指し示す先を動的に切り替え、かつ、実体側と参照側の両方をシリアライズする場合に発生する可能性があります。<br>

Theolizerは各インスタンスに対して、被ポインタ指定することでオブジェクト追跡しますが、オブジェクト追跡している場合、同じインスタンスであることを自動判定できます。<br>
そこで、同じオブジェクト追跡単位内で重複してシリアライズした場合、先頭の１つのみをシリアライズします。<br>

また、同じインスタンスには同じオブジェクトIDを振って保存しています。回復時、同じオブジェクトIDが振られたインスタンスは同じメモリへ回復される必要があります。それができない時はWrongUsing例外を投げます。このエラーはポインタの回復時は発生しませんが、参照の場合、参照先を変更することができないため発生する可能性かあります。<br>

動作をまとめると以下のようになります。<br>

|状況|動作|
|----|----|
|オブジェクト追跡しているインスタンスを<br>複数回シリアライズした時|2回目以降はシリアライズされない。<br>回復処理時、回復先が異なるとWrongUsing例外を投げる。|
|オブジェクト追跡していないインスタンスを<br>複数回シリアライズした時|全てシリアライズされる。|

<br>
//############################################################################
@section Modifying 3.クラスとenum型の変更対応について
//############################################################################

---

// ***************************************************************************
@subsection Basic31 3-1.クラスとenum型のアップデート
// ***************************************************************************

1. アップデートで可能なクラスの定義変更<br>
  - 名前対応の場合<br>
    メンバ変数の追加<br>
    メンバ変数の削除<br>
    メンバ変数の順序変更<br>
<br>

  - 順序対応の場合<br>
    メンバ変数の最後へ追加<br>
    メンバ変数を最後から削除<br>
    メンバ変数の順序変更は不可<br>
<br>

2. アップデートで可能なenum型の定義変更<br>
  - シンボル名保存／シンボル値保存<br>
    シンボル名の変更<br>
    シンボル値の変更<br>
    シンボルの削除<br>
    シンボルの追加<br>

なお、アップデートに関しては、新しいプログラムが保存したデータを古いプログラムが回復することをサポートしません。<br>
<br>

// ***************************************************************************
@subsection Basic32 3-2.バージョン番号管理について
// ***************************************************************************

@subsubsection Basic321 3-1-1.ローカル・バージョン番号
クラスとenum型について、バージョン番号を1から1つづ上げていくことができます。<br>
クラスは、1つ上げる毎にバージョン・ダウン処理(downVersion)、バージョン・アップ処理(upVersion)を記述できます。<br>
enum型は、古いシンボル名やシンボル値を別のシンボルへ割り当てたい時にバージョンを上げることで対応できます。<br>
クラスとenum型自身のバージョン番号を「ローカル・バージョン番号」と呼びます。

@subsubsection Basic322 3-1-2.グローバル・バージョン番号
Theolizerは旧プログラムのバージョン番号を指定してデータ保存する機能に対応していますが、その際に全てのクラスとenum型について適切なバージョン番号を指定することはたいへん困難です。<br>
関連する複数のクラスについて同時にバージョン・アップすることで定義が矛盾しないようにすることも良くあると思います。そのようなクラス群について、全て矛盾なくローカル・バージョンを指定する必要があるからです。<br>

プログラマが1つ1つ指定することは現実的ではないため、Theolizerはグローバル・バージョン番号テーブルを自動生成します。<br>
グローバル・バージョン番号は、１つ以上のクラスかenum型のローカル・バージョン番号を上げた時にインクリメントして下さい。回復対象のシリアライズ・データ内の全ローカル・バージョン番号を特定できるように上げればOKです。<br>Theolizerドライバは、グローバル・バージョン番号に対応する（完全自動型を除く）全てのローカル・バージョン番号を「グローバル・バージョン番号テーブル」へ自動的に生成します。（完全自動型のローカル・バージョン番号は１固定ですので記録する必要がありません。）

旧プログラム形式でデータ保存する時は、このグローバル・バージョン番号を指定することで、全てのローカル・バージョン番号を矛盾なく指定できます。<br>
また、シリアライズ・データにもグローバル・バージョン番号を記録します。これにより、当該シリアライズ・データを回復する際の初期ローカル・バージョン番号も全て矛盾なく特定できます。<br>
この番号を用いて、各クラス、および、enum型を回復後、カスケードにバージョン・アップ処理を行うことで最新版のデータを回復します。

// ***************************************************************************
@subsection Basic33 3-3.クラス・メンバ変数のKeep-step
// ***************************************************************************
クラスAに含まれるクラスBのメンバ変数x、および、基底クラスyについて、down/upVersion関数でx, yのメンバにアクセスしたいケースがあります。<br>
そして、クラスAをバージョン・アップする際にクラスBもバージョン・アップされることもあります。<br>
このような場合で、クラスAのdown/upVersion関数を記述する際に、クラスBの当時のバージョンのメンバ変数を使いたいこともあると思います。<br>

以下の条件を満たした基底クラスとメンバ変数については、それを含むクラスと「足並みを揃えて(Keep-step)」バージョン・ダウン／アップ処理され、各down/upVersion関数を定義した当時のメンバを提供します。

- 基底クラスの場合<br>
自動型(非侵入型完全自動、侵入型半自動)クラスの基底クラスはKeep-step処理されます。<br>

- メンバ変数の場合<br>
自動型(非侵入型完全自動、侵入型半自動)クラスとenum型のメンバ変数はKeep-step処理されます。<br>
ただし、ポインタや参照の場合を除きます。

逆にKeep-step処理しないものは以下の通りです。
- プリミティブ型(int型やstd::string型等)
- 非侵入型手動クラス
- 全てのポインタ
- 全ての参照

なお、配列については配列の基底型により上記の通りKeep-step処理を判定します。

<div style="padding: 10px; margin-bottom: 10px; border: 1px solid #333333; border-radius: 10px; background-color: #d0d0d0;">
なお、ポインタや参照が指す領域については足並みを揃える処理を行わないので、アクセスできる内容はバージョン番号が最も新しい最新版となります。また、ポイント先のデータが既に回復されていることを保証するのはプログラマの責任です。保存した順序で回復されますので、アクセスする場合は先に保存して下さい。
</div>

// ***************************************************************************
@subsection Basic34 3-4.クラスとenum型定義変更時の注意事項
// ***************************************************************************

@subsubsection Basic341 3-4-1.保存先指定等の要因で一部のメンバが回復されていない時

追加されたメンバがある場合、もしくは、保存先指定でクラス分割されている場合、旧バージョン・データから回復した時、ファイルに記録されていないメンバについては回復されません。従って、upVersion処理にて回復されていないメンバが存在する可能性があります。<br>
そこで、down/upVersion関数を記述する場合は、追加した変数をdown/upVersion関数内で使用しないこと、できるだけクラス分割しないことお勧めします。

クラス分割とdown/upVersion処理の両方が必要な場合は下記をお薦めします。

- 同じ保存先へ保存したデータを使うようにして下さい<br>
下記の時、<br>
- メンバ変数a0, a1 : 保存先A<br>
- メンバ変数b0, b1 : 保存先B<br>
メンバ変数a0, a1の値は、メンバ変数a0, a1の値だけを用いて修正し、メンバ変数b0, b1の値は、メンバ変数b0, b1の値だけを用いて修正すると安全です。<br>
例えばメンバ変数a0の値を修正するためにメンバ変数b0の値を使うと、保存A→保存先Bの順序で回復した場合、回復されていないメンバ変数b0を使って回復済のメンバ変数a0を修正することになります。

@subsubsection Basic342 3-4-2.旧バージョン処理時、メンバが回復されなかった時に発生する問題点対応

downVersion後upVersionで元に戻らないような修正を行うことは多いと思います。(例えは、downVersion関数を記述せずupVersion関数のみを記述すると該当します。)<br>
何も手当しない場合、シリアライズ・データを回復する時、回復対象でないメンバがdown/upVersion関数処理の結果、不適切に変化してしまいます。<br>

これを避けるため、以下の対策を実装しています。

- down/upVersion関数で変更可能な対象を限定<br>
プリミティブ型(int型やstd::string型)、および、enum型に限定しています。<br>

- 変更可能なものについてはdownVersion時コピーする<br>
当該変数をコピーした後で、downVersion処理することで元の変数への影響を回避します。<br>

- データを回復したかどうかを管理する<br>
データを回復した変数はupVersion時、次バージョンへ反映する必要があります。<br>
また、回復しなかった変数は元の値を変更するべきではありませんので、反映しません。<br>
この制御のため、mDoSucceedというフラグを①の管理領域に設け、trueの時次バージョンへ反映します。<br>

@subsubsection Basic343 3-4-3.down/upVersion関数で変更可能な変数と変更してはいけない変数のまとめ

少しややこしいのでまとめます。

1. down/upVersion関数で変更してよい変数<br>
  - プリミティブ型(int型やstd::string型)<br>
  - enum型<br>
<br>
2. down/upVersion関数で変更しては<b>いけない</b>変数<br>
2-1. ポインタ、および、参照が指す先のインスタンス<br>
2-2. 非侵入型手動クラスの基底クラス、および、メンバ変数<br>
2-3. ポインタそれ自身（ポインタの指す先の変更）<br>

2-1.はシリアライズ対象のインスタンスへ直接アクセスすることになります。<br>
そのため、これらをdown/upVersion関数内で修正した場合、その修正は直接ターゲットが書き換えられてしまいます。そのため、<b>保存時、および、回復処理で回復されなかった変数が変化</b>してしまいます。クラスの「外」にあるインスタンスは書き換えないようにご注意下さい。<br>

なお、2-2.と2-3.については代入演算子(operator=)をprivate定義していますので、変更しようとするとコンパイル・エラーになります。

@subsubsection Basic344 3-4-4.クラスやenum型を削除する際の注意事項

バージョン・アップに伴い、一度シリアライズ指定したクラスやenum型をソース・コードから削除したい場合の注意事項があります。<br>
そのような削除したいクラスやenum型を仮に 型X と呼びます。

<b>最新版の別のクラスの旧バージョンで 型X を使っている場合</b><br>

最新版で一切使っていなくても、旧バージョン・データから回復する際に 型X の定義が必要になりますので、最新版のソースから 型X の定義を削除しないで下さい。

<b>型X を一切使っていない場合</b><br>

この場合は原則として削除可能です。ただし、グローバル・バージョン番号テーブルに 型X が登録されています。その自動削除には対応していませんので、手で削除する必要があります。<br>
全てのコンパイル単位の *.theolizer.hpp の最後にありますので、その中の下記行を削除して下さい。

@code
THEOLIZER_INTERNAL_ADD(型X, ...);
@endcode

これを削除し忘れていた場合、コンパイル・エラー（Global Version No. Table error. Please check deleted class or enum.）となります。

*/
