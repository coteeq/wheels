#include <wheels/intrusive/list.hpp>

#include <wheels/test/framework.hpp>

using namespace wheels;

TEST_SUITE(IntrusiveList) {
  struct Item : IntrusiveListNode<Item> {
    std::string data;

    Item(std::string _data) : data(std::move(_data)) {
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

    Item* pop_first = items.PopFront();
    ASSERT_EQ(pop_first->data, "hello");

    Item* pop_second = items.PopFront();
    ASSERT_EQ(pop_second->data, "world");

    Item* pop_third = items.PopFront();
    ASSERT_EQ(pop_third->data, "!");

    ASSERT_TRUE(items.IsEmpty());

    ASSERT_EQ(items.PopFront(), nullptr);
  }

  SIMPLE_TEST(FrontBack) {
    IntrusiveList<Item> items;

    Item a("a");
    Item b("b");
    Item c("c");
    Item d("d");

    items.PushBack(&a);
    items.PushFront(&b);
    items.PushBack(&c);
    items.PushFront(&d);

    Item* item;

    item = items.PopFront();
    ASSERT_EQ(item, &d);
    item = items.PopBack();
    ASSERT_EQ(item, &c);
    item = items.PopFront();
    ASSERT_EQ(item, &b);
    item = items.PopFront();
    ASSERT_EQ(item, &a);
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
    ASSERT_EQ((*iterator).data, "hello");

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
      builder << item.data << " ";
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

    ASSERT_EQ(items.PopFront()->data, "hello");
    ASSERT_EQ(items.PopFront()->data, "world");
    ASSERT_EQ(items.PopFront()->data, "!");
    ASSERT_EQ(items.PopFront()->data, "foo");
    ASSERT_EQ(items.PopFront()->data, "bar");

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

  SIMPLE_TEST(HasItems) {
    IntrusiveList<Item> list;

    ASSERT_TRUE(list.IsEmpty());
    ASSERT_FALSE(list.HasItems());

    Item hello("Hello");

    list.PushBack(&hello);

    ASSERT_FALSE(list.IsEmpty());
    ASSERT_TRUE(list.HasItems());

    list.PopFront();
  }
}
