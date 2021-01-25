#include <wheels/test/test_framework.hpp>
#include <wheels/test/fail_handler.hpp>
#include <wheels/test/iterate.hpp>

#include <thread>

TEST_SUITE(TestFramework) {
  void ThrowRuntimeError() {
    throw std::runtime_error("error!");
  }

  SIMPLE_TEST(Asserts) {
    ASSERT_TRUE(2 * 2 == 4);
    ASSERT_FALSE(0 > 1);

    ASSERT_EQ(1 + 2, 3);
    ASSERT_NE(2 * 2, 5);

    ASSERT_GT(2, 1);
    ASSERT_LT(1, 2);

    ASSERT_GE(1 + 2, 3);
    ASSERT_LE(1 + 2, 3);

    ASSERT_THROW(ThrowRuntimeError(), std::runtime_error);
  }

  class CountingFailHandler
 : public wheels::test::ITestFailHandler {
   public:
    void Fail(wheels::test::ITestPtr /*test*/, const std::string& /*error*/) override {
      ++fail_count_;
    }

    size_t FailCount() const {
      return fail_count_;
    }

   private:
    size_t fail_count_ = 0;
  };

  SIMPLE_TEST(FiredAsserts) {
    auto counting_fail_handler = std::make_shared<CountingFailHandler>();

    wheels::test::FailHandlerSwitcher switcher(counting_fail_handler);

    ASSERT_TRUE(false);
    ASSERT_EQ(counting_fail_handler->FailCount(), 1);

    ASSERT_EQ(1, 2);
    ASSERT_EQ(counting_fail_handler->FailCount(), 2);
  }

  /*
  TEST(TimeLimit, wheels::test::TestOptions().TimeLimit(12s)) {
    std::this_thread::sleep_for(11s);
  }
  */

  // SIMPLE_TEST(Fail) { ASSERT_TRUE(false); }

  TEST(TimeLeft, wheels::test::TestOptions().TimeLimit(2s)) {
    while (wheels::test::TestTimeLeft() > 100ms) {
      // KeepRunning
      std::this_thread::sleep_for(10ms);
    }
  }

  ITERATE_TEST(Iterations, 3s) {
    std::this_thread::sleep_for(50ms);
    std::cout << "Iteration: " <<
              wheels::test::TestIteration() << std::endl;
    std::cout << "Hash: " << wheels::test::TestIterationHash() << std::endl;
  }
}
