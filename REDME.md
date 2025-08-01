# 🧠 Memory Tracker in C

## 📘 Overview

This project is a **dynamic memory tracking system written in C** that logs and monitors memory allocations and deallocations during runtime. It helps detect memory leaks by keeping track of every `malloc`/`calloc` allocation and its corresponding `free`. All active memory blocks and any leaks are logged to the console and log files.

This tool is useful for debugging memory issues in C programs — especially for detecting:
- **Memory leaks**
- **Unfreed allocations**
- **Live memory usage** (on demand)

---

## 📂 Project Structure


---

## 🎯 Features

- ✅ Menu-driven program for allocation, deallocation, inspection
- ✅ Tracks memory size and address of each allocation
- ✅ Uses **hash table** for efficient lookups
- ✅ Logs memory leaks with timestamps to `memory_log.txt`
- ✅ Option to **view live allocations** any time
- ✅ Option to **free all allocated memory at once**
- ✅ Optional prompt to **flush leaked memory on exit**
- ✅ Highlights warnings and errors with colored console output

---

## 📋 Menu Options

---
Allocate memory

Free specific memory

View current allocated memory (console + log)

Free all tracked memory

Exit (with memory leak check)
---


On exit, if memory leaks are detected:
- You'll see a red-highlighted warning
- You'll be asked whether to flush the leaked memory

---

## 🛠️ How It Works

1. Memory allocations are wrapped in functions like `tracked_malloc(size_t)` or `tracked_calloc(...)`.
2. Each allocation is recorded in a **hash table** using the address as the key.
3. On `free`, the memory is removed from the hash table.
4. On exit:
   - The program scans the hash table.
   - Any remaining entries are reported as memory leaks.
   - The user can choose to flush (free) them.

---

## 🧪 Example Log Output

