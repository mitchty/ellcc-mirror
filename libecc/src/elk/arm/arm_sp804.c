/* Initialize the ARM SP804 dual timer for use as the system clock.
 * RICH: This should be renamed to something a little less generic.
 */

#include <time.h>
#include "kernel.h"
#include "timer.h"
#include "scheduler.h"
#include "irq.h"
#include "arm_sp804.h"

static long resolution;                     // The clock divisor.
static __elk_lock lock;
static volatile time_t monotonic_seconds;   // The monotonic timer.
static long long realtime_offset;           // The realtime timer offset.
static int timeout_active;                  // Set if a timeout is active.
static long long timeout;                   // When the next timeout occurs.

/* Get the timer resolution.
 */
long __elk_timer_getres(void)
{
  return resolution;
}

/* Get the realtime offset.
 */
long long __elk_timer_get_realtime_offset(void)
{
  return realtime_offset;
}

/** Get the monotonic timer.
 */
long long __elk_timer_get_monotonic(void)
{
  time_t secs;
  long nsecs;
  do {
    secs = monotonic_seconds;
    nsecs = 1000000000 - (*Timer2Value * resolution);
  } while (secs != monotonic_seconds);    // Take care of a seconds update.

  long long value = secs * 1000000000LL + nsecs;
  return value;
}

/** Get the realtime timer.
 */
long long __elk_timer_get_realtime(void)
{
  long long value = __elk_timer_get_monotonic();
  __elk_lock_aquire(&lock);
  value += realtime_offset;
  __elk_lock_release(&lock);
  return value;
}

/** Set the realtime timer.
 */
void __elk_timer_set_realtime(long long value)
{
  long long mt = __elk_timer_get_monotonic();
  __elk_lock_aquire(&lock);
  realtime_offset = value - mt;
  __elk_lock_release(&lock);
}

/** Check to see if a timeout interrupt is needed.
 */
static void check_timeout()
{
  if (!timeout_active) {
    return;
  }

  long long mt = __elk_timer_get_monotonic();
  if (timeout - mt > 1000000000) {
    // More than a second away.
    return;
  }

  // In the same second. Set up the interrupt.
  timeout_active = 0;

  // Set up Timer 1 as the short term timer.
  mt = timeout - mt;
  mt = mt * CLOCK / 1000000000;
  if (mt < 0) mt = 0;
  *Timer1Load = mt;
  // Enable timer, 32 bit, Divide by 1 clock, oneshot.
  *Timer1Control = TimerEn|TimerSize|IntEnable|OneShot;
}

/** This is the second timer interupt handler.
 */
static void sec_interrupt(void *arg)
{
  __elk_lock_aquire(&lock);
  monotonic_seconds++;
  check_timeout();
  __elk_lock_release(&lock);
}

/** Start the sleep timer.
 */
void __elk_timer_start(long long when)
{
  long long mt;
  do {
    mt = __elk_timer_get_monotonic();
    if (when <= mt) {
      when = __elk_timer_expired(mt);
      if (when == 0) {
        timeout_active = 0;
        return;
      }
    }
  } while (when <= mt);
  __elk_lock_aquire(&lock);
  timeout_active = 1;
  timeout = when;
  check_timeout();
  __elk_lock_release(&lock);
}

static void short_interrupt(void *arg)
{
  long long mt = __elk_timer_get_monotonic();
  mt = __elk_timer_expired(mt);
  if (mt == 0) {
    __elk_lock_aquire(&lock);
    timeout_active = 0;
    __elk_lock_release(&lock);
    return;
  }
  __elk_timer_start(mt);
}

static const IRQHandler timer_irq =
{
  .id = IRQ + 32,
  .edge = 0,
  .priority = 0,
  .cpus = 0xFFFFFFFF,         // Send to all CPUs.
  .sources = 2,
  {
    { Timer1MIS, TimerInt, Timer1IntClr, 0,
      { short_interrupt, NULL }},
    { Timer2MIS, TimerInt, Timer2IntClr, 0,
      { sec_interrupt, NULL }},
  }
};

CONSTRUCTOR()
{
  // Set up the timer.
  resolution = 1000000000 / (CLOCK / 1); 

  // Set up Timer 2 as the second timer.
  *Timer2BGLoad = CLOCK;
  // Register the interrupt handler.
  irq_register(&timer_irq);
  // Enable timer, 32 bit, Divide by 1 clock, periodic.
  *Timer2Control = TimerEn|TimerSize|TimerMode|IntEnable;
}
