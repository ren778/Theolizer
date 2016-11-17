//############################################################################
/*!
    @brief      ドキュメント・ファイル－仕様
    @ingroup    Documents
    @file       2.specification.h
    @author     Yoshinori Tahara
    @date       2016/11/01 Created
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
    @page Specification 仕様

<br>
//############################################################################
@section Basic 1.基本仕様
//############################################################################

---

// ***************************************************************************
@subsection Basic11 1-1.シリアライズ可能な型
// ***************************************************************************
__① プリミティブ型__<br>
基本的な型をプリミティブ型と呼んでいます。下記がプリミティブ型です。
- C++のint型等の組込み型全て
- std::string, std::wstring, std::u16string, std::u32string

__② enum型__<br>
通常のenum型、および、scoped enum型の両方に対応しています。

__③ class、struct__<br>
class、および、structは同じ扱いです。

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
- **参照**<br>
参照は単純に別名として取り扱います。C++の仕様上アドレスを回復することができないためです。

// ***************************************************************************
@subsection Basic12 1-2.シリアライズ可能な型についての補足事項
// ***************************************************************************
@subsubsection Basic121 1-2-1.クラスについて
クラスは下記の３種類の形式に対応しています。
- **非侵入型完全自動**<br>
クラス定義側でシリアライズすることを指定しないまま、シリアライズ処理するクラスです。バージョン番号は1固定です。

- **侵入型半自動**<br>
クラス定義内部でシリアライズすることを指定したクラスです。下記マクロで指定します。<br>
  - THEOLIZER_INTRUSIVE
  - THEOLIZER_INTRUSIVE_ORDER

- **非侵入型手動**<br>
クラス定義の外側でシリアライズすることを指定したクラスです。標準ライブラリのコンテナ型に対応するために設けた仕組みです。<br>
現時点ではユーザ殿へは開放していません。将来的には開放する予定です。

非侵入型完全自動と侵入型半自動は、自動的にメンバ変数を枚挙して、シリアライズします。そして、デシリアライズする際にシリアライズされたデータとメンバ変数の対応を取る必要があります。この対応方法として2つ用意しました。<br>

1. **名前対応**<br>
メンバ変数名が一致するものを回復します。メンバ変数の記述順序変更や、メンバ変数の並びの途中への追加／削除を行っても旧シリアライズ・データからクラス・インスタンスを回復できます。

2. **順序対応**<br>
メンバ変数の記述順に従って回復します。メンバ変数名をシリアライズ・データ内に記録しないのでデータ・サイズを減らせますが、メンバ変数の記述順序を変更できません。また、メンバ変数の追加／削除はメンバ変数の並びの最後のみ可能です。

@subsubsection Basic122 1-2-2. enum型について
enum型は下記の２種類の形式に対応しています。
- **非侵入型完全自動**<br>
enum定義側で何も指定しないまま、シリアライズ処理するenum型です。バージョン番号は1固定です。<br>
指定できるオプションはありません。（シンボル名保存固定です。）

- **非侵入型半自動**<br>
enum型定義の外側でシリアライズすることを指定します。バージョン番号指定できます。更に下記オプションを指定できます。
  - 保存内容指定（シンボル名保存 or シンボル値保存）
  - シンボル毎に旧シンボル名、旧シンボル値（シンボル名やシンボル値を変更した時に用います）

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
@subsubsection Basic161 1-6-1.デフォルト・コンストラクタが必要
原則として、シリアライズするクラスにはデフォルト・コンストラクタが必要です。<br>
ただし、下記２つは例外です。<br>
- THEOLIZER_PROCESS*マクロを用いてトップ・レベルだけから保存／回復するクラス
- 非侵入型手動クラス

@subsubsection Basic162 1-6-2.クラスのstatic変数について
これは自動シリアライズの対象としていません。

@subsubsection Basic163 1-6-3.仮想基底クラス
現在、仮想基底クラスは非対応です。<br>
オブジェクト追跡することで対応できる可能性はありますので、有用性が高い場合に対応を検討します。

<br>
//############################################################################
@section Tracking 2.オブジェクト追跡
//############################################################################

---

// ***************************************************************************
@subsection Basic21 2-1.オブジェクト追跡する領域について
// ***************************************************************************
下記についてオブジェクト追跡を行いますので、これらへのポインタはアドレス解決できます。

- ポインタ型からポイントされたクラス型の全てのインスタンス
- 被ポインタ(THEOLIZER_ANNOTATE(FS:...<...>Pointee))指定されたメンバ変数
- 被ポインタ(THEOLIZER_PROCESS_POINTEE)として保存指定された変数

被ポインタ指定はプリミティブ型をオブジェクト追跡する場合に用いることを想定しています。<br>
ポイントされたプリミティブ型全てのインスタンスを追跡するとオブジェクト追跡が重くなりすぎると予想されるため、指定されたインスタンスに対してのみオブジェクト追跡します。

// ***************************************************************************
@subsection Basic22 2-2.ポインタ型のアドレス回復について
// ***************************************************************************
ポイント先が同じ<b>オブジェクト追跡単位</b>内に記録されていることが必要です。<br>
<b>オブジェクト追跡単位</b>とは、下記の期間です。
- コンストラクト～clearTracking()
- clearTracking()～clearTracking()

// ***************************************************************************
@subsection Basic23 2-3.ポリモーフィズム対応
// ***************************************************************************
基底クラスのオーナー・ポインタ型に対して、派生クラス・インスタンスの回復が可能です。<br>
ポリモーフィズム対象の派生クラスをTHEOLIZER_REGISTER_CLASS()マクロで指定して下さい。<br>
なお、現在のところクラス・テンプレートはポリモーフィズム非対応です。<br>

<br>
//############################################################################
@section Modifying 3.バージョン変更
//############################################################################

---

// ***************************************************************************
@subsection Basic31 3-1.クラス定義とenum定義の変更
// ***************************************************************************
クラス定義とenum定義の変更については、@ref Basic121 と@ref Basic122 を参照下さい。

// ***************************************************************************
@subsection Basic32 3-2.バージョン番号管理について
// ***************************************************************************
クラスとenum型について、バージョン番号を1から1つづ上げていくことができます。<br>
クラスは、1つ上げる毎に保存／回復時の初期化処理(initialize関数)、バージョン・ダウン処理(downVersion)、バージョン・アップ処理(upVersion)を記述できます。<br>
enum型は、古いシンボル名やシンボル値を別のシンボルへ割り当てたい時にバージョンを上げることで対応できます。

// ***************************************************************************
@subsection Basic33 3-3.クラス・メンバ変数のキープ・ステップ
// ***************************************************************************
クラスに含まれるクラス型のメンバ変数A、および、基底クラスBについて、down/upVersion関数でA, Bのメンバにアクセスしたいケースがあります。<br>
そのA, Bについてdown/upVersionを記述した時のクラス定義最新版(当該バージョン番号におけるメンバ定義の最新版)を、下記の場合に使えます。<br>

- 自動型(非侵入型完全自動、侵入型半自動)が直接（ポインタ経由でなく）定義されている場合<br>

この条件を満たしたメンバと基底クラスについては、それを含むクラスと「足並みを揃えて(Keep-step)」バージョン・ダウン／アップ処理されます。

なお、ポインタの指す領域については足並みを揃える処理を行わないので、アクセスできる内容はバージョン番号が最も新しい最新版となります。また、ポイント先のデータが既に回復されていることを保証するのはプログラマの責任です。保存した順序で回復されますので、アクセスする場合は先に保存して下さい。

// ***************************************************************************
@subsection Basic34 3-4.保存先指定等の要因で一部のメンバが回復されていない時
// ***************************************************************************
追加されたメンバがある場合、もしくは、保存先指定でクラス分割されている場合、旧バージョン・データから回復した時、ファイルに記録されていないメンバについては回復されません。従って、upVersion処理にて回復されていないメンバが存在する可能性があります。<br>
そこで、up/downVersion関数を記述する場合は、追加した変数をup/downVersion関数内で使用しないこと、できるだけクラス分割しないことお勧めします。

クラス分割とup/downVersion処理の両方が必要な場合は下記をお薦めします。

- 同じ保存先へ保存したデータを使うようにして下さい<br>
下記の時、<br>
- メンバ変数a0, a1 : 保存先A<br>
- メンバ変数b0, b1 : 保存先B<br>
メンバ変数a0, a1の値は、メンバ変数a0, a1の値だけを用いて修正し、メンバ変数b0, b1の値は、メンバ変数b0, b1の値だけを用いて修正すると安全です。<br>
例えばメンバ変数a0の値を修正するためにメンバ変数b0の値を使うと、保存A→保存先Bの順序で回復した場合、回復されていないメンバ変数b0を使って回復済のメンバ変数a0を修正することになります。

// ***************************************************************************
@subsection Basic35 3-5.旧バージョン処理時、メンバが回復されなかった時に発生する問題点対応
// ***************************************************************************
downVersion後upVersionで元に戻らないような修正を行うことは多いと思います。(例えは、downVersion関数を記述せずupVersion関数のみを記述すると該当します。)<br>
何も手当しない場合、シリアライズ・データを回復する時、回復対象でないメンバがup/downVersion関数処理の結果、不適切に変化してしまいます。保存時は当然全てのデータは回復されないため更に深刻です。<br>

これを避けるため、Theolizerは、downVersion時にバックアップし回復されなかったメンバについてupVersion時にリストアしています。（旧バージョン保存時は全てのデータが回復されないため、全てのデータをリストアします。）<br>
その回復されたかどうかを示すフラグとして、変数名にTheolizerSucceedを追加した名前のbool型変数を用意しています。回復されたメンバはtrueとなり、旧バージョンの値を新バージョン側へ引き継ぎます。

// ***************************************************************************
@subsection Basic36 3-6.up/downVersion関数でポインタの先を変更してはいけません
// ***************************************************************************
ポインタが指す先はクラス外のデータですので3-5のバックアップ／リストア対象ではありません。<br>
従って、もし、downVersion後upVersion処理で元の値に戻らないような修正を行と、旧バージョンを保存した際にポインタが指す先が変化してしまいます。

*/
