# EV Charging Station Simulator (C)

A simple console program in C that simulates an electric vehicle charging station.

## Features
- View 3 chargers (7 kW, 22 kW and 50 kW) and whether each is available or occupied
- Start a charging session on a charger
- Calculate the energy used and the cost of each session
- Save every session to `sessions.csv` and view the history

## Menu
1. Show chargers
2. Start charging
3. View history
4. Exit

## How to run
Requires a C compiler such as GCC. The program uses `windows.h`, so it runs on Windows.

```
gcc main.c -o ev
./ev
```

## Files
- `main.c`: the source code
- `sessions.csv`: charging history (time, vehicle, charger, energy_kwh, cost)