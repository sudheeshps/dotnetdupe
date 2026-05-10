#include "pch.h"
#include "System/IO/StringReader.h"
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace IO {
            StringReader::StringReader(const String& s)
                : _s(s), _pos(0), _length(s.GetLength()) {
            }

            void StringReader::Close() {
                _pos = _length;
            }

            int StringReader::Peek() {
                if (_pos >= _length) return -1;
                return (int)_s[_pos];
            }

            int StringReader::Read() {
                if (_pos >= _length) return -1;
                return (int)_s[_pos++];
            }

            int StringReader::Read(wchar_t* buffer, int index, int count) {
                if (buffer == nullptr) return 0;
                int n = (std::min)(count, _length - _pos);
                for (int i = 0; i < n; i++) {
                    buffer[index + i] = (wchar_t)_s[_pos + i];
                }
                _pos += n;
                return n;
            }

            String StringReader::ReadLine() {
                int i = _pos;
                while (i < _length) {
                    wchar_t ch = (wchar_t)_s[i];
                    if (ch == L'\r' || ch == L'\n') {
                        String result = _s.Substring(_pos, i - _pos);
                        _pos = i + 1;
                        if (ch == L'\r' && _pos < _length && (wchar_t)_s[_pos] == L'\n') {
                            _pos++;
                        }
                        return result;
                    }
                    i++;
                }
                if (i > _pos) {
                    String result = _s.Substring(_pos, i - _pos);
                    _pos = i;
                    return result;
                }
                return String();
            }

            String StringReader::ReadToEnd() {
                String result = _s.Substring(_pos, _length - _pos);
                _pos = _length;
                return result;
            }
        }
    }
}
