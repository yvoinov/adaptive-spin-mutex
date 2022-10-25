# Adaptive Spin Mutex
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://github.com/yvoinov/adaptive-spin-mutex/blob/master/LICENSE)

## Introduction

This improved version of the adaptive spin mutex is inspired by an [article by Foster Brereton](https://hackernoon.com/building-a-c-hybrid-spin-mutex-f98de535b4ac).

It has been field-tested for many years in real-world applications and has been slightly improved to achieve better performance.

## Installation

Since the class is implemented as a header module (C++11 is required), it is enough to throw it into your project and include it with compiler directive.

## Usage

Since the class is implemented with an interface that is fully compatible with mutex wrapper classes, it is easily used as a replacement for standard mutexes:

```cpp
	AdaptiveSpinMutex spmux(m_lock);
	std::lock_guard<AdaptiveSpinMutex> lock(spmux);
	...
	}
private:
	std::atomic<bool>& m_lock;
...
```

## Notes

Class optimized for any C++11 compliant compiler, tested on GCC, Clang, Oracle Developer Studio.
