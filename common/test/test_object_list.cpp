/*
 *  ========== Copyright (c) Valve Corporation. All rights reserved. ==========
 */

#include "common/object_list.h"
#include <gtest/gtest.h>

namespace rehlds::common::test
{
    class ObjectListTest : public testing::Test
    {
      public:
        void SetUp() override
        {
            object_list_.init();
        }

        [[nodiscard]] ObjectList& instance()
        {
            return object_list_;
        }

      private:
        ObjectList object_list_{};
    };

    TEST_F(ObjectListTest, Init)
    {
        auto& object_list = instance();
        ASSERT_NO_THROW(object_list.init());
        ASSERT_NO_THROW(object_list.init());
    }

    TEST_F(ObjectListTest, Add)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        ASSERT_TRUE(object_list.add(&element1));
        ASSERT_TRUE(object_list.add(&element2));
        ASSERT_TRUE(object_list.add(&element3));
        ASSERT_TRUE(object_list.add(&element3));
        ASSERT_TRUE(object_list.add(&element2));
        ASSERT_TRUE(object_list.add(&element1));
    }

    TEST_F(ObjectListTest, Remove)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        object_list.add(&element1);
        object_list.add(&element2);
        object_list.add(&element2);
        object_list.add(&element3);

        ASSERT_TRUE(object_list.remove(&element1));
        ASSERT_FALSE(object_list.remove(&element1));

        ASSERT_TRUE(object_list.remove(&element2));
        ASSERT_TRUE(object_list.remove(&element2));
        ASSERT_FALSE(object_list.remove(&element2));

        ASSERT_TRUE(object_list.remove(&element3));
        ASSERT_FALSE(object_list.remove(&element3));

        ASSERT_FALSE(object_list.remove(&element1));
        ASSERT_FALSE(object_list.remove(&element2));
        ASSERT_FALSE(object_list.remove(&element3));
    }

    TEST_F(ObjectListTest, Clear)
    {
        auto& object_list = instance();
        ASSERT_NO_THROW(object_list.clear(false));
        ASSERT_NO_THROW(object_list.clear(false));

        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        object_list.add(&element1);
        ASSERT_NO_THROW(object_list.clear(false));

        object_list.add(&element1);
        object_list.add(&element1);
        ASSERT_NO_THROW(object_list.clear(false));

        object_list.add(&element1);
        object_list.add(&element2);
        object_list.add(&element3);
        ASSERT_NO_THROW(object_list.clear(false));

        auto* const element4 = new int{4};
        auto* const element5 = new int{5};
        auto* const element6 = new int{6};

        object_list.add(element4);
        object_list.add(element5);
        object_list.add(element6);
        ASSERT_NO_THROW(object_list.clear(true));
    }

    TEST_F(ObjectListTest, Size)
    {
        auto& object_list = instance();
        ASSERT_TRUE(0 == object_list.size());

        auto element1 = 1;
        object_list.add(&element1);
        ASSERT_TRUE(1 == object_list.size());

        object_list.add(&element1);
        ASSERT_TRUE(2 == object_list.size());

        object_list.remove(&element1);
        ASSERT_TRUE(1 == object_list.size());

        object_list.clear(false);
        ASSERT_TRUE(0 == object_list.size());
    }

    TEST_F(ObjectListTest, Contains)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;
        auto element4 = 4;

        ASSERT_FALSE(object_list.contains(&element1));

        object_list.add(&element1);
        object_list.add(&element1);
        object_list.add(&element2);
        object_list.add(&element3);

        ASSERT_TRUE(object_list.contains(&element1));
        ASSERT_TRUE(object_list.contains(&element2));
        ASSERT_TRUE(object_list.contains(&element3));
        ASSERT_FALSE(object_list.contains(&element4));
    }

    TEST_F(ObjectListTest, Empty)
    {
        auto& object_list = instance();
        ASSERT_TRUE(object_list.empty());

        auto element1 = 1;
        object_list.add(&element1);
        ASSERT_FALSE(object_list.empty());
    }

    TEST_F(ObjectListTest, AddHead)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        ASSERT_TRUE(object_list.add_head(&element1));
        ASSERT_TRUE(object_list.first() == &element1);

        ASSERT_TRUE(object_list.add_head(&element2));
        ASSERT_TRUE(object_list.first() == &element2);

        ASSERT_TRUE(object_list.add_head(&element3));
        ASSERT_TRUE(object_list.first() == &element3);

        ASSERT_TRUE(object_list.add_head(&element3));
        ASSERT_TRUE(object_list.first() == &element3);

        ASSERT_TRUE(object_list.add_head(&element2));
        ASSERT_TRUE(object_list.first() == &element2);

        ASSERT_TRUE(object_list.add_head(&element1));
        ASSERT_TRUE(object_list.first() == &element1);
    }

    TEST_F(ObjectListTest, AddTail)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        ASSERT_TRUE(object_list.add_tail(&element1));
        ASSERT_TRUE(object_list[object_list.size() - 1] == &element1);

        ASSERT_TRUE(object_list.add_tail(&element2));
        ASSERT_TRUE(object_list[object_list.size() - 1] == &element2);

        ASSERT_TRUE(object_list.add_tail(&element3));
        ASSERT_TRUE(object_list[object_list.size() - 1] == &element3);

        ASSERT_TRUE(object_list.add_tail(&element3));
        ASSERT_TRUE(object_list[object_list.size() - 1] == &element3);

        ASSERT_TRUE(object_list.add_tail(&element2));
        ASSERT_TRUE(object_list[object_list.size() - 1] == &element2);

        ASSERT_TRUE(object_list.add_tail(&element1));
        ASSERT_TRUE(object_list[object_list.size() - 1] == &element1);
    }

    TEST_F(ObjectListTest, RemoveHead)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        ASSERT_TRUE(nullptr == object_list.remove_head());

        object_list.add(&element1);
        object_list.add(&element2);
        object_list.add(&element3);

        ASSERT_TRUE(object_list.remove_head() == &element1);
        ASSERT_TRUE(object_list.remove_head() == &element2);
        ASSERT_TRUE(object_list.remove_head() == &element3);
    }

    TEST_F(ObjectListTest, RemoveTail)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        ASSERT_TRUE(nullptr == object_list.remove_tail());

        object_list.add(&element1);
        object_list.add(&element2);
        object_list.add(&element3);

        ASSERT_TRUE(object_list.remove_tail() == &element3);
        ASSERT_TRUE(object_list.remove_tail() == &element2);
        ASSERT_TRUE(object_list.remove_tail() == &element1);
    }

    TEST_F(ObjectListTest, First)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;

        ASSERT_TRUE(nullptr == object_list.first());

        object_list.add_head(&element1);
        ASSERT_TRUE(object_list.first() == &element1);

        object_list.add_head(&element2);
        ASSERT_TRUE(object_list.first() == &element2);

        object_list.add_head(&element2);
        ASSERT_TRUE(object_list.first() == &element2);

        object_list.add_head(&element3);
        ASSERT_TRUE(object_list.first() == &element3);
    }

    TEST_F(ObjectListTest, Next)
    {
        auto& object_list = instance();
        auto element1 = 1;
        auto element2 = 2;
        auto element3 = 3;
        auto element4 = 4;
        auto element5 = 5;
        auto element6 = 6;

        ASSERT_TRUE(nullptr == object_list.next());

        object_list.add(&element1);
        object_list.add(&element2);
        object_list.add(&element3);

        ASSERT_TRUE(object_list.next() == &element1);
        ASSERT_TRUE(object_list.next() == &element2);
        ASSERT_TRUE(object_list.next() == &element3);
        ASSERT_TRUE(nullptr == object_list.next());

        ASSERT_TRUE(object_list.first() == &element1);
        ASSERT_TRUE(object_list.next() == &element2);
        ASSERT_TRUE(object_list.next() == &element3);
        ASSERT_TRUE(nullptr == object_list.next());

        ASSERT_TRUE(object_list.contains(&element2));
        ASSERT_TRUE(object_list.next() == &element2);

        ASSERT_FALSE(object_list.contains(&element4));
        ASSERT_TRUE(object_list.next() == &element3);
        ASSERT_TRUE(nullptr == object_list.next());

        object_list.add(&element4);
        object_list.add(&element5);
        object_list.add(&element6);

        ASSERT_TRUE(object_list.contains(&element5));
        ASSERT_TRUE(object_list.remove(&element5));
        ASSERT_TRUE(object_list.next() == &element6);

        ASSERT_TRUE(object_list.contains(&element4));
        ASSERT_TRUE(object_list.remove(&element6));
        ASSERT_TRUE(object_list.next() == &element4);

        ASSERT_TRUE(object_list.contains(&element4));
        ASSERT_TRUE(object_list.remove(&element1));
        ASSERT_TRUE(object_list.next() == &element4);

        ASSERT_TRUE(object_list.first() == &element2);
        ASSERT_TRUE(object_list.next() == &element3);
        ASSERT_TRUE(object_list.next() == &element4);
        ASSERT_TRUE(nullptr == object_list.next());

        ASSERT_TRUE(object_list.first() == &element2);
        ASSERT_NO_THROW(object_list.clear(false));
        ASSERT_TRUE(nullptr == object_list.next());
    }
}
