#include "pch.h"
#include "System/Net/Sockets/NetworkStream.h"
#include "System/ArgumentNullException.h"
#include "System/IOException.h"

namespace DotNetDupe {
    namespace System {
        namespace Net {
            namespace Sockets {

                NetworkStream::NetworkStream(const SmartPointer<Socket>& socket)
                    : m_pSocket(socket), m_bOwnsSocket(false), m_bDisposed(false) {
                    if (socket.IsNull()) {
                        throw ArgumentNullException("Socket cannot be null.");
                    }
                }

                NetworkStream::NetworkStream(const SmartPointer<Socket>& socket, bool bOwnsSocket)
                    : m_pSocket(socket), m_bOwnsSocket(bOwnsSocket), m_bDisposed(false) {
                    if (socket.IsNull()) {
                        throw ArgumentNullException("Socket cannot be null.");
                    }
                }

                NetworkStream::~NetworkStream() {
                    Dispose();
                }

                bool NetworkStream::CanRead() const {
                    return !m_bDisposed;
                }

                bool NetworkStream::CanSeek() const {
                    return false;
                }

                bool NetworkStream::CanWrite() const {
                    return !m_bDisposed;
                }

                long NetworkStream::GetLength() const {
                    throw IO::IOException("NetworkStream does not support seeking.");
                }

                long NetworkStream::GetPosition() const {
                    throw IO::IOException("NetworkStream does not support seeking.");
                }

                void NetworkStream::SetPosition(long value) {
                    throw IO::IOException("NetworkStream does not support seeking.");
                }

                void NetworkStream::Flush() {
                }

                int NetworkStream::Read(char* buffer, int offset, int count) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    return m_pSocket->Receive(buffer, offset, count);
                }

                long NetworkStream::Seek(long offset, int origin) {
                    throw IO::IOException("NetworkStream does not support seeking.");
                }

                void NetworkStream::SetLength(long value) {
                    throw IO::IOException("NetworkStream does not support seeking.");
                }

                void NetworkStream::Write(const char* buffer, int offset, int count) {
                    if (m_bDisposed) throw IO::IOException("Stream is disposed.");
                    m_pSocket->Send(buffer, offset, count);
                }

                void NetworkStream::Dispose() {
                    if (!m_bDisposed) {
                        m_bDisposed = true;
                        if (m_bOwnsSocket && !m_pSocket.IsNull()) {
                            m_pSocket->Close();
                        }
                    }
                }

                void NetworkStream::Close() {
                    Dispose();
                }

            }
        }
    }
}
