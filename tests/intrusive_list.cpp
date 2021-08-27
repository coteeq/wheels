#include <wheels/intrusive/list.hpp>
#include <wheels/intrusive/forward_list.hpp>

#include <wheels/test/test_framework.hpp>

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
}

TEST_SUITE(IntrusiveForwardList) {
  struct Item : IntrusiveForwardListNode<Item> {
    std::string data;

    Item(std::string _data) : data(std::move(_data)) {
    }
  };

  SIMPLE_TEST(EmptyList) {
    IntrusiveForwardList<Item> list;

    ASSERT_TRUE(list.IsEmpty());
    ASSERT_EQ(list.Size(), 0);
  }

  SIMPLE_TEST(PushPop) {
    IntrusiveForwardList<Item> list;

    Item hello("Hello");
    Item world("World");

    list.PushBack(&hello);

    ASSERT_FALSE(list.IsEmpty());
    ASSERT_EQ(list.Size(), 1);

    list.PushBack(&world);
    ASSERT_EQ(list.Size(), 2);

    Item* first = list.PopFront();
    ASSERT_EQ(list.Size(), 1);
    ASSERT_EQ(first->data, "Hello");

    Item* second = list.PopFront();
    ASSERT_EQ(list.Size(), 0);
    ASSERT_TRUE(list.IsEmpty());
    ASSERT_EQ(second->data, "World");

    Item* third = list.PopFront();
    ASSERT_EQ(third, nullptr);
  }

  SIMPLE_TEST(PushFront) {
    IntrusiveForwardList<Item> list;

    Item hello("Hello");
    Item world("World");

    list.PushBack(&world);
    list.PushFront(&hello);

    ASSERT_EQ(list.Size(), 2);

    Item* first = list.PopFront();
    ASSERT_EQ(first->data, "Hello");
    Item* second = list.PopFront();
    ASSERT_EQ(second->data, "World");


    ASSERT_TRUE(list.IsEmpty());
  }

  SIMPLE_TEST(ForEach) {
    IntrusiveForwardList<Item> list;

    Item hello("Hello");
    Item comma(", ");
    Item world("World");

    list.PushBack(&hello);
    list.PushBack(&comma);
    list.PushBack(&world);

    std::string message;
    list.ForEach([&message](Item* item) {
      message += item->data;
    });

    list.PopFront();
    list.PopFront();
    list.PopFront();

    ASSERT_TRUE(list.IsEmpty());

    ASSERT_EQ(message, "Hello, World");
  }

  SIMPLE_TEST(MoveConstructable) {
    IntrusiveForwardList<Item> list1;

    Item hello("Hello");
    Item world("World");

    list1.PushBack(&hello);
    list1.PushBack(&world);

    IntrusiveForwardList<Item> list2(std::move(list1));

    ASSERT_TRUE(list1.IsEmpty());

    ASSERT_EQ(list2.Size(), 2);
    Item* first = list2.PopFront();
    ASSERT_EQ(first->data, "Hello");
    Item* second = list2.PopFront();
    ASSERT_EQ(second->data, "World");
  }
}
