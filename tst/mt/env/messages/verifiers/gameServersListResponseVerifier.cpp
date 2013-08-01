#include <mt/env/messages/verifiers/gameServersListResponseVerifier.hpp>
#include <mt/env/exceptions.hpp>
#include <interface/gameServersListResponse.hpp>
#include <interface/messageIds.hpp>
#include <interface/protocolIds.hpp>
#include <interface/messageTypes.hpp>

#include <gtest/gtest.h>

namespace eMU
{
namespace mt
{
namespace env
{
namespace messages
{
namespace verifiers
{

void GameServersListResponseVerifier::operator()(const core::network::Payload &payload, const std::vector<interface::GameServerInfo> &servers)
{
    if(payload.empty())
    {
        throw mt::env::exceptions::EmptyPayloadException();
    }

    const interface::GameServersListResponse *message = reinterpret_cast<const interface::GameServersListResponse*>(&payload[0]);

    ASSERT_EQ(interface::MessageType::LARGE_DECRYPTED, message->header_.typeId_);
    ASSERT_EQ(interface::ProtocolId::CONNECT_SERVER_PROTOCOL, message->header_.protocolId_);

    ASSERT_EQ(servers.size(), message->numberOfServers_);

    for(size_t i = 0; i < servers.size(); ++i)
    {
        EXPECT_EQ(servers[i].code_, message->servers_[i].code_);
        EXPECT_EQ(servers[i].load_, message->servers_[i].load_);
    }
}

}
}
}
}
}