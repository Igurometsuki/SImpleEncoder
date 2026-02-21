/*
  RotaryEncoder.cpp - ポーリング方式ロータリーエンコーダライブラリ
*/

#include "RotaryEncoder.h"

/*
  Gray code 遷移テーブル
  
  状態 = (A << 1) | B
  正常な遷移のみ ±1
  それ以外は 0（ノイズ/不正遷移）
*/
const int8_t RotaryEncoder::_transitionTable[16] = {
   0, -1, +1,  0,
  +1,  0,  0, -1,
  -1,  0,  0, +1,
   0, +1, -1,  0
};

RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB,
                             uint16_t sampleIntervalUs,
                             uint8_t detentCount)
  : _pinA(pinA),
    _pinB(pinB),
    _sampleIntervalUs(sampleIntervalUs),
    _detentCount(detentCount),
    _value(0),
    _lastState(0),
    _accumulator(0),
    _lastSampleTime(0) {
}

void RotaryEncoder::begin() {
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);
  
  // 初期状態を読み取る
  uint8_t a = digitalRead(_pinA);
  uint8_t b = digitalRead(_pinB);
  _lastState = (a << 1) | b;
}

void RotaryEncoder::poll() {
  unsigned long now = micros();
  if (now - _lastSampleTime < _sampleIntervalUs) {
    return;
  }
  _lastSampleTime = now;
  
  // 現在の状態を読み取る
  uint8_t a = digitalRead(_pinA);
  uint8_t b = digitalRead(_pinB);
  uint8_t state = (a << 1) | b;
  
  // 遷移テーブルでデルタを取得
  uint8_t index = (_lastState << 2) | state;
  int8_t delta = _transitionTable[index];
  
  // 有効な遷移のみ処理
  if (delta != 0) {
    _accumulator += delta;
    
    // デテント到達チェック
    if (_accumulator >= _detentCount) {
      _value++;
      _accumulator = 0;
    } else if (_accumulator <= -_detentCount) {
      _value--;
      _accumulator = 0;
    }
  }
  
  _lastState = state;
}

int8_t RotaryEncoder::getValue() const {
  return _value;
}

void RotaryEncoder::reset() {
  _value = 0;
  _accumulator = 0;
}

void RotaryEncoder::setValue(int8_t value) {
  _value = value;
  _accumulator = 0;
}
