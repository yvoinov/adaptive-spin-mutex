#ifndef ADAPTIVE_SPIN_MUTEX_H
#define ADAPTIVE_SPIN_MUTEX_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

/* Improved AdaptiveSpinMutex */
class AdaptiveSpinMutex {
public:
	AdaptiveSpinMutex(std::atomic<bool>& v_lock) : m_lock(v_lock) {};
	AdaptiveSpinMutex(const AdaptiveSpinMutex&) = delete;
	~AdaptiveSpinMutex() = default;

	void lock() noexcept {
		std::size_t v_spin_count { 0 };

		while (m_lock.load(std::memory_order_relaxed) || m_lock.exchange(true, std::memory_order_acquire)) {
			++v_spin_count;
			if (v_spin_count < m_spin_pred << 1) continue;	/* m_spin_pred << 1 is eq m_spin_pred * 2 */
			std::unique_lock<std::mutex> tlock(m_conditional_mutex);
			m_conditional_lock.wait_for(tlock, std::chrono::nanoseconds(1), [this]() { return !m_lock.load(std::memory_order_relaxed); });
		}

		m_spin_pred += (v_spin_count - m_spin_pred) >> 3;	/* x >> 3 is eq x / 8 */
	}

	void unlock() noexcept {
		std::unique_lock<std::mutex> tlock(m_conditional_mutex);
		m_lock.store(false, std::memory_order_relaxed);
		m_conditional_lock.notify_one();
	}

private:
	std::atomic<bool>& m_lock;
	std::atomic<std::size_t> m_spin_pred { 0 };
	std::mutex m_conditional_mutex;
	std::condition_variable m_conditional_lock;
};

#endif
