//****************************************************************
//
// ポーズメニューの情報[PauseUI.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 名前空間で囲っておく
namespace PauseMenuConfig {
    // メニューの種類の列挙型
    enum class MENUTYPE : int
    {
        CONTINUE,   // 続ける
        RETRY,      // やり直す
        QUIT,       // やめる
        MAX         // 最大数
    };
    // 列挙子のコンペア
    inline bool operator==(int Othor, MENUTYPE My) {
        // インクリメントするためにイント型にキャスト
        int _Value = static_cast<int>(My);
        return _Value == Othor;
    }
    // 列挙子のコンペア
    inline bool operator==(MENUTYPE My, int Othor) {
        // インクリメントするためにイント型にキャスト
        int _Value = static_cast<int>(My);
        return _Value == Othor;
    }
    // 列挙子の前置インクリメント
    inline MENUTYPE& operator++(MENUTYPE& Value) {
        // インクリメントするためにイント型にキャスト
        int _Value = static_cast<int>(Value);
        _Value++;
        // インクリメントした結果をenum classにキャスト
        Value = static_cast<MENUTYPE>(_Value);
        return Value;
    }
    // 列挙子の前置デクリメント
    inline MENUTYPE& operator--(MENUTYPE& Value) {
        // デクリメントするためにイント型にキャスト
        int _Value = static_cast<int>(Value);
        _Value--;
        // インクリメントした結果をenum classにキャスト
        Value = static_cast<MENUTYPE>(_Value);
        return Value;
    }

    // 列挙子の後置インクリメント
    inline MENUTYPE operator++(MENUTYPE& Value, int) {
        // 別の変数としてコピー
        MENUTYPE Cpy = Value;
        // インクリメントするために元の値をキャスト
        int V = static_cast<int>(Value);
        V++;
        // 引数に結果を反映するためにキャストし代入
        Value = static_cast<MENUTYPE>(V);
        // 元の値を返す
        return Cpy;
    }
    // 列挙子の後置デクリメント
    inline MENUTYPE operator--(MENUTYPE& Value, int) {
        // 別の変数としてコピー
        MENUTYPE Cpy = Value;
        // デクリメントするために元の値をキャスト
        int V = static_cast<int>(Value);
        V--;
        // 引数に結果を反映するためにキャストし代入
        Value = static_cast<MENUTYPE>(V);
        // 元の値を返す
        return Cpy;
    }

    // 列挙子の足し算
    inline MENUTYPE& operator+(MENUTYPE& Value, int Othor) {
        // 足し算をするためにイント型にキャスト
        int V = static_cast<int>(Value);
        V += Othor;
        // 結果を引数に反映させる
        Value = static_cast<MENUTYPE>(V);
        return Value;
    }
    // 列挙子の引き算
    inline MENUTYPE& operator-(MENUTYPE& Value, int Othor) {
        // 足し算をするためにイント型にキャスト
        int V = static_cast<int>(Value);
        V -= Othor;
        // 結果を引数に反映させる
        Value = static_cast<MENUTYPE>(V);
        return Value;
    }

    // ポーズメニューのテクスチャパス一覧
    const std::string PathList[static_cast<uint8_t>(PauseMenuConfig::MENUTYPE::MAX)] = {
    "data\\TEXTURE\\Pause\\continue.png",
    "data\\TEXTURE\\Pause\\retry.png",
    "data\\TEXTURE\\Pause\\quit.png",
    };
}