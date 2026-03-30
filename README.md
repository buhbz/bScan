# bScan
A fast, multithreaded TCP port scanner written in C++ using the Windows Winsock API. This tool scans localhost (127.0.0.1) across all ports using non-blocking sockets and concurrent threads for improved performance.

---

## 🚀 Features

- ⚡ Multithreaded scanning (50 threads)
- 🔌 Non-blocking socket connections
- 🎯 Scans full port range (1–65535)
- 🧵 Efficient workload distribution across threads
- 🛑 Graceful shutdown via user input (`exit`)
- 🖥️ Windows-native (Winsock2)

---

## 🛠️ How It Works

- The scanner divides the full port range into chunks
- Each thread scans a subset of ports
- Uses non-blocking sockets + `select()` to detect open ports quickly
- Prints open ports in real-time

---

## 📦 Requirements

- Windows OS
- C++ compiler with Winsock support (e.g., MSVC or MinGW)

---

## ⚙️ Build Instructions

### Using g++ (MinGW)

```bash
g++ main.cpp -o portscanner -lws2_32 -std=c++11
```

### Using Visual Studio (MSVC)

- Create a new C++ Console App
- Add `main.cpp`
- Ensure `ws2_32.lib` is linked (already included via pragma)

---

## ▶️ Usage

```bash
./portscanner
```

- The scanner will immediately begin scanning `127.0.0.1`
- Open ports will be printed like:

```
80 open
443 open
3306 open
```

- To stop the scanner:

```
exit
```

---

## ⚡ Example Output

```
22 open
80 open
135 open
445 open
```

---

## 🔍 Implementation Details

- Uses:
  - `socket()` with `AF_INET` and `SOCK_STREAM`
  - `ioctlsocket()` for non-blocking mode
  - `select()` for timeout-based connection checks
- Thread-safe output via `std::mutex`
- Early termination using `std::atomic<bool>`

---

## ⚠️ Limitations

- Currently scans only localhost (`127.0.0.1`)
- Windows-only (due to Winsock API)
- No service/version detection
- No UDP scanning

---

## 🔧 Possible Improvements

- Add target IP input
- Add port range customization
- Cross-platform support (POSIX sockets)
- Add banner grabbing
- Improve timeout tuning
- Output to file / JSON

---

## ⚖️ Disclaimer

This tool is intended for educational and authorized testing purposes only.  
Do not use it on networks or systems without permission.

