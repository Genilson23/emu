#include "tcpSocketMock.hpp"

namespace eMUNetworkUT = eMU::ut::network;

eMUNetworkUT::tcp::socketMock_t::socketMock_t(ioServiceStub_t &ioService):
  rbuf_(NULL),
  rbufSize_(0),
  wbuf_(NULL),
  wbufSize_(0),
  ioService_(ioService) {
    ON_CALL(*this, async_receive(::testing::_,
                                 ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                                &socketMock_t::impl_async_receive));

    ON_CALL(*this, async_send(::testing::_,
                              ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                             &socketMock_t::impl_async_send));

    ON_CALL(*this, async_connect(::testing::_,
                                 ::testing::_)).WillByDefault(::testing::Invoke(this,
                                                                                &socketMock_t::impl_async_connect));
}

void eMUNetworkUT::tcp::socketMock_t::expectCall_async_receive() {
    EXPECT_CALL(*this, async_receive(::testing::_, ::testing::_));
}

void eMUNetworkUT::tcp::socketMock_t::expectCall_async_send() {
    EXPECT_CALL(*this, async_send(::testing::_, ::testing::_));
}

void eMUNetworkUT::tcp::socketMock_t::expectCall_shutdown(boost::asio::ip::tcp::socket::shutdown_type type) {
    EXPECT_CALL(*this, shutdown(type));
}

void eMUNetworkUT::tcp::socketMock_t::expectCall_close() {
    EXPECT_CALL(*this, close());
}

void eMUNetworkUT::tcp::socketMock_t::expectCall_async_connect(const boost::asio::ip::tcp::endpoint &endpoint) {
    EXPECT_CALL(*this, async_connect(endpoint, ::testing::_));
}

void eMUNetworkUT::tcp::socketMock_t::impl_async_receive(boost::asio::mutable_buffers_1 &buffer, const ioServiceStub_t::ioHandler_t &handler) {
    rbuf_ = boost::asio::buffer_cast<uint8*>(buffer);
    rbufSize_ = boost::asio::buffer_size(buffer);
    receiveHandler_ = handler;
}

void eMUNetworkUT::tcp::socketMock_t::impl_async_send(boost::asio::mutable_buffers_1 &buffer, const ioServiceStub_t::ioHandler_t &handler) {
    wbuf_ = boost::asio::buffer_cast<uint8*>(buffer);
    wbufSize_ = boost::asio::buffer_size(buffer);
    sendHandler_ = handler;
}

void eMUNetworkUT::tcp::socketMock_t::impl_async_connect(const boost::asio::ip::tcp::endpoint &endpoint, const connectHandler_t &handler) {
    connectHandler_ = handler;
}