#include <wheels/support/intrusive_list.hpp>

#include <wheels/test/test_framework.hpp>

using namespace wheels;

TEST_SUITE(IntrusiveList) {
  struct Item
      : public IntrusiveListNode<Item> {
    std::string data_;

    Item(std::string data) : data_(std::move(data)) {
    }
  };

  SIMPLE_TEST(PushThenPop) {
    Item first("hello");
    Item second("world");
    Item third("!");

    IntrusiveList<Item> items;

    ASSERT_TRUE(items.IsEmpty());

    items.PushBack(&first);
    items.PushBack(&second);
    items.PushBack(&third);

    ASSERT_FALSE(items.IsEmpty());

    Item *pop_first = items.PopFront();
    ASSERT_EQ(pop_first->data_, "hello");

    Item *pop_second = items.PopFront();
    ASSERT_EQ(pop_second->data_, "world");

    Item *pop_third = items.PopFront();
    ASSERT_EQ(pop_third->data_, "!");

    ASSERT_TRUE(items.IsEmpty());

    ASSERT_EQ(items.PopFront(), nullptr);
  }

  SIMPLE_TEST(Iterator) {
    Item first("hello");
    Item second("world");
    Item third("!");

    IntrusiveList<Item> items;

    ASSERT_EQ(items.begin(), items.end());

    items.PushBack(&first);
    items.PushBack(&second);
    items.PushBack(&third);

    auto iterator = items.begin();
    auto end = items.end();

    ASSERT_NE(iterator, end);
    ASSERT_EQ((*iterator).data_, "hello");

    ++iterator;
    ASSERT_NE(iterator, end);

    ++iterator;
    ++iterator;
    ASSERT_EQ(iterator, end);

    items.UnlinkAll();
  }

  SIMPLE_TEST(RangeBasedForLoop) {
    Item first("hello");
    Item second("world");
    Item third("!");

    IntrusiveList<Item> items;
    items.PushBack(&first);
    items.PushBack(&second);
    items.PushBack(&third);

    wheels::StringBuilder builder;
    for (auto&& item : items) {
      builder << item.data_ << " ";
    }
    std::string message = builder;
    ASSERT_EQ(message, "hello world ! ");

    items.UnlinkAll();
  }

  SIMPLE_TEST(AppendList) {
    Item first("hello");
    Item second("world");
    Item third("!");

    IntrusiveList<Item> items;

    ASSERT_EQ(items.begin(), items.end());

    items.PushBack(&first);
    items.PushBack(&second);
    items.PushBack(&third);

    Item foo("foo");
    Item bar("bar");

    IntrusiveList<Item> another_items;
    another_items.PushBack(&foo);
    another_items.PushBack(&bar);

    items.Append(another_items);

    ASSERT_TRUE(another_items.IsEmpty());

    ASSERT_EQ(items.PopFront()->data_, "hello");
    ASSERT_EQ(items.PopFront()->data_, "world");
    ASSERT_EQ(items.PopFront()->data_, "!");
    ASSERT_EQ(items.PopFront()->data_, "foo");
    ASSERT_EQ(items.PopFront()->data_, "bar");

    items.UnlinkAll();
  }

  SIMPLE_TEST(MoveCtor) {
    Item first("hello");
    Item second("world");
    Item third("!");

    IntrusiveList<Item> items;

    ASSERT_EQ(items.begin(), items.end());

    items.PushBack(&first);
    items.PushBack(&second);

    IntrusiveList<Item> another_items = std::move(items);

    ASSERT_TRUE(items.IsEmpty());
    items.PushBack(&third);
    ASSERT_FALSE(items.IsEmpty());

    another_items.UnlinkAll();
    items.UnlinkAll();
  }
}
