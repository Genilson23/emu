#include <core/network/tcp/connectionsFactory.hpp>
#include <core/network/tcp/exceptions.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

ConnectionsFactory::~ConnectionsFactory()
{

}

Connection& ConnectionsFactory::create(size_t hash, Connection::SocketPointer socket)
{
    if(connections_.count(hash) > 0)
    {
        throw exceptions::AlreadyExistingConnectionException();
    }

    Connection::Pointer connection(new Connection(socket));
    connections_[hash] = connection;

    return *connection;
}

void ConnectionsFactory::destroy(size_t hash)
{
    if(connections_.count(hash) == 0)
    {
        throw exceptions::UnknownConnectionException();
    }

    connections_[hash].reset();
    connections_.erase(hash);
}

Connection& ConnectionsFactory::get(size_t hash)
{
    if(connections_.count(hash) == 0)
    {
        throw exceptions::UnknownConnectionException();
    }

    return *connections_[hash];
}

size_t ConnectionsFactory::getHash(Connection &connection) const
{
    for(auto &it : connections_)
    {
        if(it.second->hash() == connection.hash())
        {
            return it.first;
        }
    }

    throw exceptions::UnknownConnectionException();
}

}
}
}
}
