import os
import glob
import re

headers_map = {
    'System/Array.h': [r'\bArray\b'],
    'System/String.h': [r'\bString\b'],
    'System/IO/File.h': [r'\bFile\b'],
    'System/IO/FileStream.h': [r'\bFileStream\b'],
    'System/Environment.h': [r'\bEnvironment\b'],
    'System/Buffer.h': [r'\bBuffer\b'],
    'System/Char.h': [r'\bChar\b'],
    'System/Path.h': [r'\bPath\b'],
    'System/Random.h': [r'\bRandom\b'],
    'System/Uri.h': [r'\bUri\b', r'\bUriBuilder\b', r'\bUriParser\b'],
    'System/Version.h': [r'\bVersion\b'],
    'System/OperatingSystem.h': [r'\bOperatingSystem\b'],
    'System/BitConverter.h': [r'\bBitConverter\b'],
    'System/Convert.h': [r'\bConvert\b'],
    'System/TimeProvider.h': [r'\bTimeProvider\b'],
    'System/Diagnostics/Stopwatch.h': [r'\bStopwatch\b'],
    'System/Diagnostics/Process.h': [r'\bProcess\b', r'\bProcessStartInfo\b'],
    'System/Threading/ThreadPool.h': [r'\bThreadPool\b'],
    'System/Threading/Tasks/Task.h': [r'\bTask\b'],
    'System/Text/StringBuilder.h': [r'\bStringBuilder\b'],
    'System/Text/Json/JsonSerializer.h': [r'\bJsonSerializer\b', r'\bJsonElement\b', r'\bJsonValueKind\b'],
    'System/InvalidOperationException.h': [r'\bInvalidOperationException\b'],
    'System/Net/Sockets/Socket.h': [r'\bSocket\b'],
    'System/Net/Sockets/NetworkStream.h': [r'\bNetworkStream\b'],
    'System/Net/Sockets/TcpClient.h': [r'\bTcpClient\b'],
    'System/Net/Sockets/TcpListener.h': [r'\bTcpListener\b'],
    'System/Net/Sockets/SocketException.h': [r'\bSocketException\b'],
    'System/Net/Http/HttpRequestException.h': [r'\bHttpRequestException\b'],
    'System/Net/Sockets/UdpClient.h': [r'\bUdpClient\b'],
    'System/Net/Dns.h': [r'\bDns\b'],
    'System/Net/Http/HttpClient.h': [r'\bHttpClient\b'],
    'System/Net/Http/HttpRequestMessage.h': [r'\bHttpRequestMessage\b'],
    'System/Net/Http/HttpResponseMessage.h': [r'\bHttpResponseMessage\b'],
    'System/Net/Http/HttpContent.h': [r'\bHttpContent\b'],
    'System/Net/Security/SslStream.h': [r'\bSslStream\b'],
    'System/Security/Cryptography/X509Certificates/X509Certificate2.h': [r'\bX509Certificate2\b'],
    'System/IdentityModel/Tokens/Jwt/JWTToken.h': [r'\bJWTToken\b'],
    'System/IServiceProvider.h': [r'\bIServiceProvider\b'],
    'Extensions/DependencyInjection/ServiceCollection.h': [r'\bServiceCollection\b'],
    'Extensions/DependencyInjection/ServiceProvider.h': [r'\bServiceProvider\b'],
    'System/Collections/Generic/List.h': [r'\bList\b'],
    'System/Collections/Generic/Dictionary.h': [r'\bDictionary\b'],
    'System/Guid.h': [r'\bGuid\b'],
    'System/Console.h': [r'\bConsole\b'],
    'System/TimeZone.h': [r'\bTimeZone\b'],
    'System/TimeZoneInfo.h': [r'\bTimeZoneInfo\b'],
    'System/IO/StringReader.h': [r'\bStringReader\b'],
    'System/IO/StringWriter.h': [r'\bStringWriter\b'],
    'System/Threading/Thread.h': [r'\bThread\b'],
    'System/Threading/ManualResetEvent.h': [r'\bManualResetEvent\b'],
    'System/Threading/AutoResetEvent.h': [r'\bAutoResetEvent\b'],
    'System/Threading/Mutex.h': [r'\bMutex\b'],
    'System/Threading/Semaphore.h': [r'\bSemaphore\b'],
    'System/Threading/SemaphoreSlim.h': [r'\bSemaphoreSlim\b'],
    'System/Threading/CriticalSection.h': [r'\bCriticalSection\b'],
    'System/Threading/Interlocked.h': [r'\bInterlocked\b'],
    'System/Threading/Lock.h': [r'\bLock\b', r'\bCriticalSectionLock\b'],
    'System/Threading/SemaphoreFullException.h': [r'\bSemaphoreFullException\b'],
    'System/TimeoutException.h': [r'\bTimeoutException\b'],
}

namespaces_map = {
    'using namespace DotNetDupe::System;': [r'\bConsole\b', r'\bString\b', r'\bGuid\b', r'\bVersion\b', r'\bEnvironment\b', r'\bConvert\b', r'\bBitConverter\b', r'\bTimeProvider\b', r'\bOperatingSystem\b'],
    'using namespace DotNetDupe::System::IO;': [r'\bFile\b', r'\bFileStream\b', r'\bPath\b', r'\bStringReader\b', r'\bStringWriter\b', r'\bMemoryStream\b'],
    'using namespace DotNetDupe::System::Diagnostics;': [r'\bStopwatch\b', r'\bProcess\b', r'\bProcessStartInfo\b'],
    'using namespace DotNetDupe::System::Text;': [r'\bStringBuilder\b', r'\bEncoding\b'],
    'using namespace DotNetDupe::System::Collections::Generic;': [r'\bList\b', r'\bDictionary\b'],
    'using namespace DotNetDupe::System::Threading;': [r'\bThread\b', r'\bThreadPool\b', r'\bTask\b', r'\bManualResetEvent\b', r'\bAutoResetEvent\b', r'\bMutex\b', r'\bSemaphore\b', r'\bSemaphoreSlim\b', r'\bCriticalSection\b', r'\bInterlocked\b', r'\bLock\b'],
    'using namespace DotNetDupe::System::Net::Sockets;': [r'\bSocket\b', r'\bTcpClient\b', r'\bTcpListener\b', r'\bUdpClient\b', r'\bNetworkStream\b'],
}

def clean_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    # Find the boundary between headers/namespaces and actual code.
    # The actual code block starts with '#include "Demos.h"' or function declarations.
    boundary_idx = -1
    for idx, line in enumerate(lines):
        if '#include "Demos.h"' in line:
            boundary_idx = idx
            break

    if boundary_idx == -1:
        print(f"Skipping {filepath} (no boundary found)")
        return

    header_section = lines[:boundary_idx]
    code_section = lines[boundary_idx:]

    code_str = "".join(code_section)

    # Filter includes
    new_headers = []
    for line in header_section:
        line_strip = line.strip()
        if line_strip.startswith('#include'):
            # Extract header path
            match = re.match(r'#include\s+["<]([^">]+)[">]', line_strip)
            if match:
                header_path = match.group(1)
                # Check if this header is mapped
                if header_path in headers_map:
                    # Check if any pattern matches the code
                    patterns = headers_map[header_path]
                    is_used = False
                    for pat in patterns:
                        if re.search(pat, code_str):
                            is_used = True
                            break
                    if is_used:
                        new_headers.append(line)
                else:
                    # Keep unmapped/system headers like <iostream>, <vector>, etc.
                    new_headers.append(line)
        elif line_strip.startswith('using namespace'):
            if line_strip in namespaces_map:
                patterns = namespaces_map[line_strip]
                is_used = False
                for pat in patterns:
                    if re.search(pat, code_str):
                        is_used = True
                        break
                if is_used:
                    new_headers.append(line)
            else:
                new_headers.append(line)
        elif line_strip == "":
            # Keep empty lines if they are not consecutive
            if not new_headers or new_headers[-1] != "\n":
                new_headers.append(line)
        else:
            new_headers.append(line)

    # Write cleaned content back
    cleaned_content = "".join(new_headers).strip() + "\n\n" + "".join(code_section).strip() + "\n"
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(cleaned_content)
    print(f"Cleaned {filepath}")

def clean_all():
    demo_files = glob.glob('*Demo.cpp')
    for f in demo_files:
        clean_file(f)

if __name__ == '__main__':
    clean_all()
