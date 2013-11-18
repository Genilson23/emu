#include <gtest/gtest.h>

#include <core/common/usersFactory.hpp>
#include <core/common/hashableObject.hpp>

namespace common = eMU::core::common;

class UsersFactoryTest: public ::testing::Test
{
protected:
    UsersFactoryTest():
        maxNumberOfUsers_(2),
        usersFactory_(maxNumberOfUsers_) {}

    class FakeUser: public common::HashableObject
    {

    };

    virtual void SetUp()
    {
    }

    size_t maxNumberOfUsers_;
    common::UsersFactory<FakeUser> usersFactory_;
};

TEST_F(UsersFactoryTest, WhenMaxNumberOfUsersReachedShouldThrowException)
{
    size_t user1 = usersFactory_.create();
    size_t user2 = usersFactory_.create();

    bool exceptionThrown = false;
    try
    {
        usersFactory_.create();
    }
    catch(const common::UsersFactory<FakeUser>::MaxNumberOfUsersReachedException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);

    usersFactory_.destroy(user1);
    usersFactory_.destroy(user2);
}

TEST_F(UsersFactoryTest, WhenUserDestroyedShouldBeErasedFromFactory)
{
    size_t user1 = usersFactory_.create();
    ASSERT_EQ(1, usersFactory_.users().size());

    usersFactory_.destroy(user1);
    ASSERT_EQ(0, usersFactory_.users().size());
}

TEST_F(UsersFactoryTest, WhenInvalidHashWasGivenThenFindShouldThrowException)
{
    size_t user1 = usersFactory_.create();
    size_t user2 = usersFactory_.create();

    bool exceptionThrown = false;
    try
    {
        usersFactory_.find(12345);
    }
    catch(const common::UsersFactory<FakeUser>::UnknownUserException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);

    usersFactory_.destroy(user1);
    usersFactory_.destroy(user2);
}

TEST_F(UsersFactoryTest, find)
{
    size_t user1 = usersFactory_.create();
    size_t user2 = usersFactory_.create();

    FakeUser &foundUser1 = usersFactory_.find(user1);
    ASSERT_EQ(user1, foundUser1.getHash());

    FakeUser &foundUser2 = usersFactory_.find(user2);
    ASSERT_EQ(user2, foundUser2.getHash());

    usersFactory_.destroy(user1);
    usersFactory_.destroy(user2);
}