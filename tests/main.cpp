#include <gtest/gtest.h>
#include <../include/threadPool.hpp>

TEST(ThreadPool, SuccessSingleTask)
{
    auto task = [](int a, int b)
    {
        return a + b;
    };

    auto result = moleksyn::ThreadPool::getInstance().schedule(task, 1, 2);
    EXPECT_EQ(result.get(), 3);
}

TEST(ThreadPool, SuccessSingleTaskNoReturnValue)
{
    auto task = [](int a, int b)
    {
    };

    auto result = moleksyn::ThreadPool::getInstance().schedule(task, 1, 2);
    EXPECT_NO_THROW(result.get());
}

TEST(ThreadPool, SuccessMultipleTasksBeingAddedAndExecutedAtTheSameTime)
{
    constexpr auto amountOfTasks = 1000;
    std::atomic_int64_t counter = 0;
    auto task = [&counter]()
    {
        counter++;
    };

    std::vector<std::future<void>> futures;
    for(auto i = 0; i < amountOfTasks; ++i)
    {
        futures.push_back(moleksyn::ThreadPool::getInstance().schedule(task));
    }
    EXPECT_TRUE(counter > 0 && counter < amountOfTasks);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  auto& pool = moleksyn::ThreadPool::getInstance();
  return RUN_ALL_TESTS();
}
