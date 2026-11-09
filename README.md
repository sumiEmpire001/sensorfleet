# sensorfleet

An in-memory IoT fleet telemetry engine. Register devices with valid measurement bounds, ingest finite numeric readings with timezone-aware timestamps, detect out-of-range measurements, and calculate per-device counts, means and alert counts. It enforces strictly increasing timestamps for each device. No hardware connection, database, authentication or network server is included.

## Run tests

Python 3.10+ with no third-party packages:

```sh
python -m unittest discover -s tests -v
```

## Example

```python
from sensorfleet import Fleet
fleet = Fleet()
fleet.register("soil-1", 20, 80)
print(fleet.ingest("soil-1", 18, "2026-01-01T00:00:00Z"))
```

GitHub Actions is configured to run tests for Python 3.10, 3.12 and 3.13. CI results must be checked separately.

## C++ Arduino / ESP32 implementation

The Python fleet model is preserved. A separate embedded-friendly C++11
version lives at `arduino/SensorFleet/src/SensorFleet.h`. It tracks at most
eight registered devices using fixed-size storage and supports per-device
numeric bounds, timestamp ordering, counts, average and alert totals.

Unlike the Python library, the C++ version accepts integer seconds supplied
by the caller, rather than parsing timezone-aware ISO timestamps. Devices have
15-character IDs; no network API, authentication or persistent storage is
provided. An `IndicatorDemo` maps a potentiometer to a synthetic sensor
reading with an LED indicating an out-of-range status. Uno: A0/D8;
classic ESP32: GPIO34/GPIO18.

C++ portable tests:
```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

GitHub Actions attempts GCC/Clang and Arduino Uno/ESP32 sketch builds.
Physical boards, calibrated sensors, timestamps, wraparound and fleet
communications still require hardware testing and engineering.
