// { dg-options "-std=gnu++2a" }
// { dg-do run { target c++2a } }
// { dg-additional-options "-pthread" { target pthread } }
// { dg-require-gthreads "" }
// { dg-skip-if "broken" { ! *-*-*linux } }

// Copyright (C) 2020 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <chrono>

#include <testsuite_hooks.h>

int
main ()
{
  using namespace std::literals::chrono_literals;

  std::mutex m;
  std::condition_variable cv;

  long aa;
  long bb;

  std::atomic<long*> a(nullptr);
  std::thread t([&]
		{
		  cv.notify_one();
		  a.wait(nullptr);
		  if (a.load() == &aa)
		    a.store(&bb);
		});
  std::unique_lock<std::mutex> l(m);
  cv.wait(l);
  std::this_thread::sleep_for(100ms);
  a.store(&aa);
  a.notify_one();
  t.join();
  VERIFY( a.load() == &bb);
  return 0;
}