﻿//############################################################################
//      Theolizerライブラリの連携処理クラス(C#側)
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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace theolizer
{
    // ***************************************************************************
    //      各種ヘルパー
    // ***************************************************************************

    public enum SerializerType
    {
        None,               // シリアライザ無し
        Binary,             // Binary
        Json                // Json
    }

    // ***************************************************************************
    //      C++DLL用連携処理統括クラス
    //          DLLの場合、通常１つしかインスタンス不要なのでシングルトンとする
    // ***************************************************************************

    sealed class DllIntegrator : IDisposable
    {
        private static DllIntegrator sInstance = new DllIntegrator();

        public static DllIntegrator get()
        {
            return sInstance;
        }

        //----------------------------------------------------------------------------
        //      管理領域
        //----------------------------------------------------------------------------

        [StructLayout(LayoutKind.Sequential)]
        public struct Streams
        {
            public IntPtr   mRequest;           // C#->Cpp要求用ストリーム
            public IntPtr   mResponse;          // Cpp->C#応答用ストリーム
            public IntPtr   mNotify;            // Cpp->C#通知用ストリーム
        }

        Streams     mStreams = new Streams();

        // 内部ストリーム（現在は暫定的にStreamWriter/StreamReader)
        Stream          mRequestStream;
        StreamWriter    mRequestWriter;
        public StreamWriter RequestWriter
        {
            get { return mRequestWriter; }
        }

        Stream          mResponseStream;
        StreamReader    mResponseReader;
        public StreamReader ResponseReader
        {
            get { return mResponseReader; }
        }

        Stream          mNotifyStream;
        StreamReader    mNotifyReader;
        public StreamReader NotifyReader
        {
            get { return mNotifyReader; }
        }

        //----------------------------------------------------------------------------
        //      コンストラクタ
        //----------------------------------------------------------------------------

        [DllImport(Constants.CppDllName)]
        extern static void CppInitialize(out Streams oStreams);

        private DllIntegrator()
        {
            CppInitialize(out mStreams);
Debug.WriteLine("mRequest  = {0:X16}", (ulong)mStreams.mRequest);
Debug.WriteLine("mResponse = {0:X16}", (ulong)mStreams.mResponse);
Debug.WriteLine("mNotify   = {0:X16}", (ulong)mStreams.mNotify);

            mRequestStream = new CppOStream(mStreams.mRequest);
            mRequestWriter = new StreamWriter(mRequestStream, new UTF8Encoding(false));

            mResponseStream = new CppIStream(mStreams.mResponse);
            mResponseReader = new StreamReader(mResponseStream, new UTF8Encoding(false));

            mNotifyStream = new CppIStream(mStreams.mNotify);
            mNotifyReader = new StreamReader(mNotifyStream, new UTF8Encoding(false));
        }

        //----------------------------------------------------------------------------
        //      破棄
        //----------------------------------------------------------------------------

        ~DllIntegrator()
        {
            Dispose(false);
        }

        protected void Dispose(bool disposing)
        {
            if (mRequestSerializer != null)     mRequestSerializer.Dispose();
            if (mResponseSerializer != null)    mResponseSerializer.Dispose();
            if (mNotifySerializer != null)      mNotifySerializer.Dispose();
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        //----------------------------------------------------------------------------
        //      シリアライザ
        //----------------------------------------------------------------------------

        BaseSerializer   mRequestSerializer;
        public BaseSerializer RequestSerializer
        {
            get { return mRequestSerializer; }
        }

        BaseSerializer   mResponseSerializer;
        BaseSerializer   mNotifySerializer;

        public void setupSerializer(SerializerType iSerializerType)
        {
            switch(iSerializerType)
            {
            case SerializerType.None:
            case SerializerType.Binary:
                if (mRequestSerializer != null)
                {
                    mRequestSerializer.Dispose();
                    mRequestSerializer = null;
                }
                if (mResponseSerializer != null)
                {
                    mResponseSerializer.Dispose();
                    mResponseSerializer = null;
                }
                if (mNotifySerializer != null)
                {
                    mNotifySerializer.Dispose();
                    mNotifySerializer = null;
                }
                break;

            case SerializerType.Json:
                mRequestSerializer = new JsonOSerializer(mRequestStream);
                break;
            }
        }
    }
}

