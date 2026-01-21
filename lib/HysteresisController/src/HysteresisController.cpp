#include "HysteresisController.h"

HysteresisController::HysteresisController(uint8_t pin) 
    : _pin(pin), _state(false), _mode(ControllerMode::AUTOMATIC), 
    _manualState(false) {}

void HysteresisController::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void HysteresisController::update(float currentReading) {
    if (_mode == ControllerMode::MANUAL) {
        _state = _manualState;
    } else {
        // Hysteresis Logic
        if (currentReading < _targetLow) _state = true;
        else if (currentReading > _targetHigh) _state = false;
    }
    digitalWrite(_pin, _state ? HIGH : LOW);
}

void HysteresisController::setMode(ControllerMode m) { _mode = m; }
void HysteresisController::setManualPower(bool on) { _manualState = on; }
void HysteresisController::tonggleManualPower() { _manualState = !_manualState; }
void HysteresisController::setThresholds(float low, float high) {
    _targetLow = low;
    _targetHigh = high;
}

bool HysteresisController::isActive() const { return _state; }
ControllerMode HysteresisController::getMode() const { return _mode; }