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
