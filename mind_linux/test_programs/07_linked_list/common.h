#ifndef PTR_CHASING_COMMON_H
#define PTR_CHASING_COMMON_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

#define CDF_BUCKET_NUM 512
#define SLEEP_THRES_NANOS 10


/**
 * @brief Latency into CDF buckets
 * @param lat_in_us latency in microseconds
 * @return Bucket number
 */
static int latency_to_bkt(unsigned long lat_in_us) {
  if (lat_in_us < 100)
    return (int) lat_in_us;
  else if (lat_in_us < 1000)
    return 100 + ((lat_in_us - 100) / 10);
  else if (lat_in_us < 10000)
    return 190 + ((lat_in_us - 1000) / 100);
  else if (lat_in_us < 100000)
    return 280 + ((lat_in_us - 10000) / 1000);
  else if (lat_in_us < 1000000)
    return 370 + ((lat_in_us - 100000) / 10000);
  return CDF_BUCKET_NUM - 1;    // over 1 sec
}


int GetRandom(int min, int max, unsigned int *seedp) {
  int ret = (rand_r(seedp) % (max - min)) + min;
  return ret;
}

// Core affinity

int pin_to_core(int core_id) {
  int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
  if (core_id < 0 || core_id >= num_cores)
    return -1;

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_t current_thread = pthread_self();
  return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}


// Time utility

double get_time() {
  struct timespec cur;
  clock_gettime(CLOCK_MONOTONIC, &cur);
  return (cur.tv_sec * 1e9 + cur.tv_nsec) * 1e-9;
}


static inline unsigned long calculate_dt(struct timeval *ts)
{
	unsigned long old_t = ts->tv_sec * 1000000 + ts->tv_usec;
	gettimeofday(ts, NULL);
	return ts->tv_sec * 1000000 + ts->tv_usec - old_t;

}

static inline void measure_time_start(struct timeval *ts)
{
	gettimeofday(ts, NULL);
}

static inline unsigned long measure_time_end(struct timeval *ts)
{
	return calculate_dt(ts);
}


inline void interval_between_access(long delta_time_usec) {
  if (delta_time_usec <= 0)
    return;
  else {
    struct timespec ts;
    ts.tv_nsec = (delta_time_usec << 1) / 3;
    if (ts.tv_nsec > SLEEP_THRES_NANOS) {
      ts.tv_sec = 0;
      nanosleep(&ts, NULL);
    }
  }
}


// ptr-chasing
enum backend_t {
    PAGING,
    RDMA,
    FPGA,
    LOCAL
};

#endif // PTR_CHASING_COMMON_H
