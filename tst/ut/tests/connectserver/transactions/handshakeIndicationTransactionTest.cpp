#include <gtest/gtest.h>
#include <ut/env/connectserver/messageSenderMock.hpp>
#include <connectserver/transactions/handshakeIndicationTransaction.hpp>

namespace connectserverEnv = eMU::ut::env::connectserver;

class HandshakeIndicationTransactionTest: public ::testing::Test
{
protected:
    HandshakeIndicationTransactionTest():
        hash_(12345) {}

    connectserverEnv::MessageSenderMock messageSender_;
    size_t hash_;
};

TEST_F(HandshakeIndicationTransactionTest, handle)
{
    EXPECT_CALL(messageSender_, sendHandshakeIndication(hash_));

    eMU::connectserver::transactions::HandshakeIndicationTransaction transaction(hash_, messageSender_);
    transaction.handle();
}

TEST_F(HandshakeIndicationTransactionTest, isValidShouldAlwaysReturnTrue)
{
    ASSERT_TRUE(eMU::connectserver::transactions::HandshakeIndicationTransaction(0, messageSender_).isValid());
    ASSERT_TRUE(eMU::connectserver::transactions::HandshakeIndicationTransaction(hash_, messageSender_).isValid());
}
