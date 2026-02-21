/*
  RotaryEncoder.h - ポーリング方式ロータリーエンコーダライブラリ
  
  概要:
    - Gray code 状態遷移（FSM）を使用
    - チャタリング（接点バウンス）や不正遷移を無視
    - ポーリング方式による高精度デコード
*/

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

class RotaryEncoder {
public:
  /*
    コンストラクタ
    
    パラメータ:
      pinA: エンコーダA相ピン
      pinB: エンコーダB相ピン
      sampleIntervalUs: サンプリング間隔（マイクロ秒）
      detentCount: 1クリックあたりの遷移数（通常は4）
  */
  RotaryEncoder(uint8_t pinA, uint8_t pinB, 
                uint16_t sampleIntervalUs = 800, 
                uint8_t detentCount = 4);
  
  // 初期化
  void begin();
  
  // エンコーダ状態を更新（loop内で呼び出す）
  void poll();
  
  // 現在の値を取得
  int8_t getValue() const;
  
  // 値をリセット
  void reset();
  
  // 値を設定
  void setValue(int8_t value);

private:
  uint8_t _pinA;
  uint8_t _pinB;
  uint16_t _sampleIntervalUs;
  uint8_t _detentCount;
  
  volatile int8_t _value;
  uint8_t _lastState;
  int8_t _accumulator;
  unsigned long _lastSampleTime;
  
  // Gray code 遷移テーブル
  static const int8_t _transitionTable[16];
};

#endif
