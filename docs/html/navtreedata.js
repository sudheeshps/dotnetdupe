/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "DotNetDupe", "index.html", [
    [ "STL vs DotNetDupe Comparison ⚖️", "index.html", "index" ],
    [ "ActiveUserSession &amp; UserSessionInfo", "md_docs_2_active_user_session.html", [
      [ "<span class=\"tt\">UserSessionInfo</span> Struct", "md_docs_2_active_user_session.html#autotoc_md2", null ],
      [ "<span class=\"tt\">ActiveUserSession</span> Class", "md_docs_2_active_user_session.html#autotoc_md4", [
        [ "Syntax", "md_docs_2_active_user_session.html#autotoc_md5", null ]
      ] ],
      [ "Static Methods", "md_docs_2_active_user_session.html#autotoc_md7", [
        [ "<span class=\"tt\">static List&lt;UserSessionInfo&gt; GetActiveSessions()</span>", "md_docs_2_active_user_session.html#autotoc_md8", null ],
        [ "<span class=\"tt\">static List&lt;UserSessionInfo&gt; GetExpiredSessions()</span>", "md_docs_2_active_user_session.html#autotoc_md9", null ],
        [ "<span class=\"tt\">static List&lt;UserSessionInfo&gt; GetAllSessions()</span>", "md_docs_2_active_user_session.html#autotoc_md10", null ]
      ] ],
      [ "Example", "md_docs_2_active_user_session.html#autotoc_md12", null ]
    ] ],
    [ "Array&lt;T&gt;", "md_docs_2_array.html", [
      [ "Syntax", "md_docs_2_array.html#autotoc_md15", null ],
      [ "Constructors &amp; Destructors", "md_docs_2_array.html#autotoc_md17", [
        [ "<span class=\"tt\">Array()</span>", "md_docs_2_array.html#autotoc_md18", null ],
        [ "<span class=\"tt\">explicit Array(int iLength)</span>", "md_docs_2_array.html#autotoc_md19", null ],
        [ "<span class=\"tt\">Array(const T* pData, int iLength)</span>", "md_docs_2_array.html#autotoc_md20", null ],
        [ "<span class=\"tt\">Array(const std::initializer_list&lt;T&gt;&amp; vItems)</span>", "md_docs_2_array.html#autotoc_md21", null ],
        [ "<span class=\"tt\">Array(const Array&amp; other)</span> / <span class=\"tt\">Array&amp; operator=(const Array&amp; other)</span>", "md_docs_2_array.html#autotoc_md22", null ],
        [ "<span class=\"tt\">Array(Array&amp;&amp; other) noexcept</span> / <span class=\"tt\">Array&amp; operator=(Array&amp;&amp; other) noexcept</span>", "md_docs_2_array.html#autotoc_md23", null ],
        [ "<span class=\"tt\">~Array() override</span>", "md_docs_2_array.html#autotoc_md24", null ]
      ] ],
      [ "Properties &amp; Elements", "md_docs_2_array.html#autotoc_md26", [
        [ "<span class=\"tt\">int GetLength() const</span>", "md_docs_2_array.html#autotoc_md27", null ],
        [ "<span class=\"tt\">bool IsNull() const</span>", "md_docs_2_array.html#autotoc_md28", null ],
        [ "<span class=\"tt\">T* GetData()</span> / <span class=\"tt\">const T* GetData() const</span>", "md_docs_2_array.html#autotoc_md29", null ],
        [ "<span class=\"tt\">T* begin()</span> / <span class=\"tt\">T* end()</span> / <span class=\"tt\">const T* begin() const</span> / <span class=\"tt\">const T* end() const</span>", "md_docs_2_array.html#autotoc_md30", null ],
        [ "<span class=\"tt\">T&amp; operator[](int iIndex)</span> / <span class=\"tt\">const T&amp; operator[](int iIndex) const</span>", "md_docs_2_array.html#autotoc_md31", null ]
      ] ],
      [ "Searching &amp; Predicates", "md_docs_2_array.html#autotoc_md33", [
        [ "<span class=\"tt\">int IndexOf(const T&amp; value) const</span>", "md_docs_2_array.html#autotoc_md34", null ],
        [ "<span class=\"tt\">int LastIndexOf(const T&amp; value) const</span>", "md_docs_2_array.html#autotoc_md35", null ],
        [ "<span class=\"tt\">bool Exists(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md36", null ],
        [ "<span class=\"tt\">T Find(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md37", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; FindAll(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md38", null ],
        [ "<span class=\"tt\">int FindIndex(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md39", null ],
        [ "<span class=\"tt\">T FindLast(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md40", null ],
        [ "<span class=\"tt\">int FindLastIndex(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md41", null ],
        [ "<span class=\"tt\">bool TrueForAll(const Predicate&lt;T&gt;&amp; fnPredicate) const</span>", "md_docs_2_array.html#autotoc_md42", null ]
      ] ],
      [ "Operations &amp; Transformations", "md_docs_2_array.html#autotoc_md44", [
        [ "<span class=\"tt\">void Sort()</span>", "md_docs_2_array.html#autotoc_md45", null ],
        [ "<span class=\"tt\">void Reverse()</span>", "md_docs_2_array.html#autotoc_md46", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_array.html#autotoc_md47", null ],
        [ "<span class=\"tt\">void ForEach(const Action&lt;T&gt;&amp; fnAction)</span>", "md_docs_2_array.html#autotoc_md48", null ],
        [ "<span class=\"tt\">void CopyTo(Array&lt;T&gt;&amp; arrTarget, int iIndex)</span>", "md_docs_2_array.html#autotoc_md49", null ],
        [ "<span class=\"tt\">static void Copy(Array&lt;T&gt;&amp; arrSource, Array&lt;T&gt;&amp; arrDestination, int iLength)</span>", "md_docs_2_array.html#autotoc_md50", null ]
      ] ],
      [ "Example", "md_docs_2_array.html#autotoc_md52", null ]
    ] ],
    [ "Web API Authentication &amp; Authorization Guide", "md_docs_2_auth.html", null ],
    [ "AutoResetEvent", "md_docs_2_auto_reset_event.html", [
      [ "Methods", "md_docs_2_auto_reset_event.html#autotoc_md71", [
        [ "<span class=\"tt\">AutoResetEvent(bool initialState)</span>", "md_docs_2_auto_reset_event.html#autotoc_md72", null ],
        [ "<span class=\"tt\">bool WaitOne(int millisecondsTimeout)</span>", "md_docs_2_auto_reset_event.html#autotoc_md73", null ]
      ] ],
      [ "Remarks", "md_docs_2_auto_reset_event.html#autotoc_md74", null ],
      [ "Code Example", "md_docs_2_auto_reset_event.html#autotoc_md75", null ]
    ] ],
    [ "BinaryReader", "md_docs_2_binary_reader.html", [
      [ "Syntax", "md_docs_2_binary_reader.html#autotoc_md78", null ],
      [ "Constructors", "md_docs_2_binary_reader.html#autotoc_md80", [
        [ "<span class=\"tt\">BinaryReader(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)</span>", "md_docs_2_binary_reader.html#autotoc_md81", null ],
        [ "<span class=\"tt\">BinaryReader(SmartPointer&lt;Stream&gt; spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)</span>", "md_docs_2_binary_reader.html#autotoc_md82", null ]
      ] ],
      [ "Configuration &amp; Stream Inspection", "md_docs_2_binary_reader.html#autotoc_md84", null ],
      [ "Read Methods", "md_docs_2_binary_reader.html#autotoc_md86", null ],
      [ "Lifecycle &amp; Disposal", "md_docs_2_binary_reader.html#autotoc_md88", null ],
      [ "Exceptions", "md_docs_2_binary_reader.html#autotoc_md90", null ],
      [ "Example", "md_docs_2_binary_reader.html#autotoc_md92", null ]
    ] ],
    [ "BinaryWriter", "md_docs_2_binary_writer.html", [
      [ "Syntax", "md_docs_2_binary_writer.html#autotoc_md95", null ],
      [ "Constructors", "md_docs_2_binary_writer.html#autotoc_md97", [
        [ "<span class=\"tt\">BinaryWriter(Stream* pStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)</span>", "md_docs_2_binary_writer.html#autotoc_md98", null ],
        [ "<span class=\"tt\">BinaryWriter(SmartPointer&lt;Stream&gt; spStream, bool bLeaveOpen = false, bool bIsLittleEndian = true)</span>", "md_docs_2_binary_writer.html#autotoc_md99", null ]
      ] ],
      [ "Configuration &amp; Stream Navigation", "md_docs_2_binary_writer.html#autotoc_md101", null ],
      [ "Write Methods", "md_docs_2_binary_writer.html#autotoc_md103", null ],
      [ "Lifecycle &amp; Disposal", "md_docs_2_binary_writer.html#autotoc_md105", null ],
      [ "Exceptions", "md_docs_2_binary_writer.html#autotoc_md107", null ],
      [ "Example", "md_docs_2_binary_writer.html#autotoc_md109", null ]
    ] ],
    [ "BitConverter", "md_docs_2_bit_converter.html", [
      [ "Syntax", "md_docs_2_bit_converter.html#autotoc_md112", null ],
      [ "Static Fields", "md_docs_2_bit_converter.html#autotoc_md114", [
        [ "<span class=\"tt\">static const bool IsLittleEndian</span>", "md_docs_2_bit_converter.html#autotoc_md115", null ]
      ] ],
      [ "Static Methods", "md_docs_2_bit_converter.html#autotoc_md117", [
        [ "<span class=\"tt\">static Array&lt;byte&gt; GetBytes(...)</span>", "md_docs_2_bit_converter.html#autotoc_md118", null ],
        [ "Conversion from Byte Arrays", "md_docs_2_bit_converter.html#autotoc_md120", null ],
        [ "<span class=\"tt\">static bool ToBoolean(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md121", null ],
        [ "<span class=\"tt\">static char ToChar(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md122", null ],
        [ "<span class=\"tt\">static short ToInt16(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md123", null ],
        [ "<span class=\"tt\">static int ToInt32(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md124", null ],
        [ "<span class=\"tt\">static long long ToInt64(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md125", null ],
        [ "<span class=\"tt\">static float ToSingle(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md126", null ],
        [ "<span class=\"tt\">static double ToDouble(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md127", null ],
        [ "<span class=\"tt\">static unsigned short ToUInt16(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md128", null ],
        [ "<span class=\"tt\">static unsigned int ToUInt32(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md129", null ],
        [ "<span class=\"tt\">static unsigned long long ToUInt64(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md130", null ],
        [ "String Representation", "md_docs_2_bit_converter.html#autotoc_md132", null ],
        [ "<span class=\"tt\">static String ToString(Array&lt;byte&gt;&amp; arrValue)</span>", "md_docs_2_bit_converter.html#autotoc_md133", null ],
        [ "<span class=\"tt\">static String ToString(Array&lt;byte&gt;&amp; arrValue, int iStartIndex)</span>", "md_docs_2_bit_converter.html#autotoc_md134", null ],
        [ "<span class=\"tt\">static String ToString(Array&lt;byte&gt;&amp; arrValue, int iStartIndex, int iLength)</span>", "md_docs_2_bit_converter.html#autotoc_md135", null ],
        [ "Bit Level Reinterpretations", "md_docs_2_bit_converter.html#autotoc_md137", null ]
      ] ],
      [ "Example", "md_docs_2_bit_converter.html#autotoc_md139", null ]
    ] ],
    [ "BlockingCollection&lt;T&gt;", "md_docs_2_blocking_collection.html", [
      [ "Syntax", "md_docs_2_blocking_collection.html#autotoc_md142", null ],
      [ "Constructors", "md_docs_2_blocking_collection.html#autotoc_md144", [
        [ "<span class=\"tt\">BlockingCollection()</span>", "md_docs_2_blocking_collection.html#autotoc_md145", null ],
        [ "<span class=\"tt\">explicit BlockingCollection(int iBoundedCapacity)</span>", "md_docs_2_blocking_collection.html#autotoc_md146", null ]
      ] ],
      [ "Properties", "md_docs_2_blocking_collection.html#autotoc_md148", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_blocking_collection.html#autotoc_md149", null ],
        [ "<span class=\"tt\">int GetBoundedCapacity() const</span>", "md_docs_2_blocking_collection.html#autotoc_md150", null ],
        [ "<span class=\"tt\">bool IsAddingCompleted() const</span>", "md_docs_2_blocking_collection.html#autotoc_md151", null ],
        [ "<span class=\"tt\">bool IsCompleted() const</span>", "md_docs_2_blocking_collection.html#autotoc_md152", null ]
      ] ],
      [ "Producer Operations (Add)", "md_docs_2_blocking_collection.html#autotoc_md154", [
        [ "<span class=\"tt\">void Add(const T&amp; item)</span>", "md_docs_2_blocking_collection.html#autotoc_md155", null ],
        [ "<span class=\"tt\">bool TryAdd(const T&amp; item, int iTimeoutMilliseconds = 0)</span>", "md_docs_2_blocking_collection.html#autotoc_md156", null ]
      ] ],
      [ "Consumer Operations (Take)", "md_docs_2_blocking_collection.html#autotoc_md158", [
        [ "<span class=\"tt\">T Take()</span>", "md_docs_2_blocking_collection.html#autotoc_md159", null ],
        [ "<span class=\"tt\">bool TryTake(T&amp; item, int iTimeoutMilliseconds = 0)</span>", "md_docs_2_blocking_collection.html#autotoc_md160", null ]
      ] ],
      [ "Completion &amp; Utilities", "md_docs_2_blocking_collection.html#autotoc_md162", [
        [ "<span class=\"tt\">void CompleteAdding()</span>", "md_docs_2_blocking_collection.html#autotoc_md163", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_blocking_collection.html#autotoc_md164", null ]
      ] ],
      [ "Example", "md_docs_2_blocking_collection.html#autotoc_md166", null ]
    ] ],
    [ "Buffer", "md_docs_2_buffer.html", [
      [ "Syntax", "md_docs_2_buffer.html#autotoc_md169", null ],
      [ "Static Methods", "md_docs_2_buffer.html#autotoc_md171", [
        [ "<span class=\"tt\">template &lt;typename T&gt; static void BlockCopy(Array&lt;T&gt;&amp; src, int srcOffset, Array&lt;T&gt;&amp; dst, int dstOffset, int count)</span>", "md_docs_2_buffer.html#autotoc_md172", null ],
        [ "<span class=\"tt\">template &lt;typename T&gt; static int ByteLength(Array&lt;T&gt;&amp; array)</span>", "md_docs_2_buffer.html#autotoc_md174", null ],
        [ "<span class=\"tt\">template &lt;typename T&gt; static byte GetByte(Array&lt;T&gt;&amp; array, int index)</span>", "md_docs_2_buffer.html#autotoc_md176", null ],
        [ "<span class=\"tt\">template &lt;typename T&gt; static void SetByte(Array&lt;T&gt;&amp; array, int index, byte value)</span>", "md_docs_2_buffer.html#autotoc_md178", null ]
      ] ],
      [ "Example", "md_docs_2_buffer.html#autotoc_md180", null ]
    ] ],
    [ "Char", "md_docs_2_char.html", [
      [ "Syntax", "md_docs_2_char.html#autotoc_md183", null ],
      [ "Constructors &amp; Assignment", "md_docs_2_char.html#autotoc_md185", [
        [ "<span class=\"tt\">Char()</span>", "md_docs_2_char.html#autotoc_md186", null ],
        [ "<span class=\"tt\">Char(char32_t c)</span>", "md_docs_2_char.html#autotoc_md187", null ],
        [ "<span class=\"tt\">void operator=(char32_t c)</span>", "md_docs_2_char.html#autotoc_md188", null ],
        [ "<span class=\"tt\">bool Equals(char32_t c) const</span> / <span class=\"tt\">bool operator==(char32_t c) const</span>", "md_docs_2_char.html#autotoc_md189", null ],
        [ "<span class=\"tt\">char32_t GetChar() const</span>", "md_docs_2_char.html#autotoc_md190", null ]
      ] ],
      [ "Static Classification Methods", "md_docs_2_char.html#autotoc_md192", [
        [ "General Unicode Categories", "md_docs_2_char.html#autotoc_md193", null ],
        [ "ASCII Specific Methods", "md_docs_2_char.html#autotoc_md194", null ],
        [ "Range Checking", "md_docs_2_char.html#autotoc_md195", null ]
      ] ],
      [ "Static Conversion Methods", "md_docs_2_char.html#autotoc_md197", [
        [ "<span class=\"tt\">static char32_t ToLower(char32_t c)</span>", "md_docs_2_char.html#autotoc_md198", null ],
        [ "<span class=\"tt\">static char32_t ToUpper(char32_t c)</span>", "md_docs_2_char.html#autotoc_md199", null ],
        [ "<span class=\"tt\">static double GetNumericValue(char32_t c)</span>", "md_docs_2_char.html#autotoc_md200", null ],
        [ "<span class=\"tt\">String ToString() const</span> / <span class=\"tt\">static String ToString(char32_t c)</span>", "md_docs_2_char.html#autotoc_md201", null ]
      ] ],
      [ "Example", "md_docs_2_char.html#autotoc_md203", null ]
    ] ],
    [ "ConcurrentBag&lt;T&gt;", "md_docs_2_concurrent_bag.html", [
      [ "Syntax", "md_docs_2_concurrent_bag.html#autotoc_md248", null ],
      [ "Constructors", "md_docs_2_concurrent_bag.html#autotoc_md250", [
        [ "<span class=\"tt\">ConcurrentBag()</span>", "md_docs_2_concurrent_bag.html#autotoc_md251", null ]
      ] ],
      [ "Properties", "md_docs_2_concurrent_bag.html#autotoc_md253", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_concurrent_bag.html#autotoc_md254", null ],
        [ "<span class=\"tt\">bool IsEmpty() const</span>", "md_docs_2_concurrent_bag.html#autotoc_md255", null ]
      ] ],
      [ "Member Functions", "md_docs_2_concurrent_bag.html#autotoc_md257", [
        [ "<span class=\"tt\">void Add(const T&amp; item)</span>", "md_docs_2_concurrent_bag.html#autotoc_md258", null ],
        [ "<span class=\"tt\">bool TryTake(T&amp; result)</span>", "md_docs_2_concurrent_bag.html#autotoc_md259", null ],
        [ "<span class=\"tt\">bool TryPeek(T&amp; result) const</span>", "md_docs_2_concurrent_bag.html#autotoc_md260", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_concurrent_bag.html#autotoc_md261", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_concurrent_bag.html#autotoc_md262", null ]
      ] ],
      [ "Example", "md_docs_2_concurrent_bag.html#autotoc_md264", null ]
    ] ],
    [ "Concurrent Collections (<span class=\"tt\">DotNetDupe::System::Collections::Concurrent</span>)", "md_docs_2_concurrent_collections.html", [
      [ "Classes Overview", "md_docs_2_concurrent_collections.html#autotoc_md266", null ],
      [ "1. <span class=\"tt\">ConcurrentDictionary&lt;TKey, TValue&gt;</span>", "md_docs_2_concurrent_collections.html#autotoc_md268", [
        [ "Methods", "md_docs_2_concurrent_collections.html#autotoc_md269", null ],
        [ "Usage Example", "md_docs_2_concurrent_collections.html#autotoc_md270", null ]
      ] ],
      [ "2. <span class=\"tt\">ConcurrentQueue&lt;T&gt;</span>", "md_docs_2_concurrent_collections.html#autotoc_md272", [
        [ "Methods", "md_docs_2_concurrent_collections.html#autotoc_md273", null ],
        [ "Usage Example", "md_docs_2_concurrent_collections.html#autotoc_md274", null ]
      ] ],
      [ "3. <span class=\"tt\">ConcurrentStack&lt;T&gt;</span>", "md_docs_2_concurrent_collections.html#autotoc_md276", [
        [ "Methods", "md_docs_2_concurrent_collections.html#autotoc_md277", null ],
        [ "Usage Example", "md_docs_2_concurrent_collections.html#autotoc_md278", null ]
      ] ],
      [ "4. <span class=\"tt\">ConcurrentBag&lt;T&gt;</span>", "md_docs_2_concurrent_collections.html#autotoc_md280", [
        [ "Methods", "md_docs_2_concurrent_collections.html#autotoc_md281", null ]
      ] ],
      [ "5. <span class=\"tt\">BlockingCollection&lt;T&gt;</span>", "md_docs_2_concurrent_collections.html#autotoc_md283", [
        [ "Methods", "md_docs_2_concurrent_collections.html#autotoc_md284", null ],
        [ "Producer-Consumer Example", "md_docs_2_concurrent_collections.html#autotoc_md285", null ]
      ] ]
    ] ],
    [ "ConcurrentDictionary&lt;TKey, TValue&gt;", "md_docs_2_concurrent_dictionary.html", [
      [ "Syntax", "md_docs_2_concurrent_dictionary.html#autotoc_md288", null ],
      [ "Constructors", "md_docs_2_concurrent_dictionary.html#autotoc_md290", [
        [ "<span class=\"tt\">ConcurrentDictionary()</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md291", null ]
      ] ],
      [ "Properties", "md_docs_2_concurrent_dictionary.html#autotoc_md293", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md294", null ],
        [ "<span class=\"tt\">bool IsEmpty() const</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md295", null ],
        [ "<span class=\"tt\">TValue&amp; operator[](const TKey&amp; key)</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md296", null ]
      ] ],
      [ "Thread-Safe Operations", "md_docs_2_concurrent_dictionary.html#autotoc_md298", [
        [ "<span class=\"tt\">bool TryAdd(const TKey&amp; key, const TValue&amp; value)</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md299", null ],
        [ "<span class=\"tt\">bool TryGetValue(const TKey&amp; key, TValue&amp; value) const</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md300", null ],
        [ "<span class=\"tt\">bool TryRemove(const TKey&amp; key, TValue&amp; value)</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md301", null ],
        [ "<span class=\"tt\">bool ContainsKey(const TKey&amp; key) const</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md302", null ],
        [ "<span class=\"tt\">TValue GetOrAdd(const TKey&amp; key, const TValue&amp; value)</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md303", null ],
        [ "<span class=\"tt\">template &lt;typename F&gt; TValue GetOrAdd(const TKey&amp; key, F valueFactory)</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md304", null ],
        [ "<span class=\"tt\">TValue AddOrUpdate(const TKey&amp; key, const TValue&amp; addValue, const TValue&amp; updateValue)</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md305", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md306", null ],
        [ "<span class=\"tt\">Array&lt;TKey&gt; GetKeys() const</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md307", null ],
        [ "<span class=\"tt\">Array&lt;TValue&gt; GetValues() const</span>", "md_docs_2_concurrent_dictionary.html#autotoc_md308", null ]
      ] ],
      [ "Example", "md_docs_2_concurrent_dictionary.html#autotoc_md310", null ]
    ] ],
    [ "ConcurrentQueue&lt;T&gt;", "md_docs_2_concurrent_queue.html", [
      [ "Syntax", "md_docs_2_concurrent_queue.html#autotoc_md313", null ],
      [ "Constructors", "md_docs_2_concurrent_queue.html#autotoc_md315", [
        [ "<span class=\"tt\">ConcurrentQueue()</span>", "md_docs_2_concurrent_queue.html#autotoc_md316", null ]
      ] ],
      [ "Properties", "md_docs_2_concurrent_queue.html#autotoc_md318", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_concurrent_queue.html#autotoc_md319", null ],
        [ "<span class=\"tt\">bool IsEmpty() const</span>", "md_docs_2_concurrent_queue.html#autotoc_md320", null ]
      ] ],
      [ "Member Functions", "md_docs_2_concurrent_queue.html#autotoc_md322", [
        [ "<span class=\"tt\">void Enqueue(const T&amp; item)</span>", "md_docs_2_concurrent_queue.html#autotoc_md323", null ],
        [ "<span class=\"tt\">bool TryDequeue(T&amp; result)</span>", "md_docs_2_concurrent_queue.html#autotoc_md324", null ],
        [ "<span class=\"tt\">bool TryPeek(T&amp; result) const</span>", "md_docs_2_concurrent_queue.html#autotoc_md325", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_concurrent_queue.html#autotoc_md326", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_concurrent_queue.html#autotoc_md327", null ]
      ] ],
      [ "Example", "md_docs_2_concurrent_queue.html#autotoc_md329", null ]
    ] ],
    [ "ConcurrentStack&lt;T&gt;", "md_docs_2_concurrent_stack.html", [
      [ "Syntax", "md_docs_2_concurrent_stack.html#autotoc_md332", null ],
      [ "Constructors", "md_docs_2_concurrent_stack.html#autotoc_md334", [
        [ "<span class=\"tt\">ConcurrentStack()</span>", "md_docs_2_concurrent_stack.html#autotoc_md335", null ]
      ] ],
      [ "Properties", "md_docs_2_concurrent_stack.html#autotoc_md337", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_concurrent_stack.html#autotoc_md338", null ],
        [ "<span class=\"tt\">bool IsEmpty() const</span>", "md_docs_2_concurrent_stack.html#autotoc_md339", null ]
      ] ],
      [ "Member Functions", "md_docs_2_concurrent_stack.html#autotoc_md341", [
        [ "<span class=\"tt\">void Push(const T&amp; item)</span>", "md_docs_2_concurrent_stack.html#autotoc_md342", null ],
        [ "<span class=\"tt\">bool TryPop(T&amp; result)</span>", "md_docs_2_concurrent_stack.html#autotoc_md343", null ],
        [ "<span class=\"tt\">bool TryPeek(T&amp; result) const</span>", "md_docs_2_concurrent_stack.html#autotoc_md344", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_concurrent_stack.html#autotoc_md345", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_concurrent_stack.html#autotoc_md346", null ]
      ] ],
      [ "Example", "md_docs_2_concurrent_stack.html#autotoc_md348", null ]
    ] ],
    [ "Console", "md_docs_2_console.html", [
      [ "Syntax", "md_docs_2_console.html#autotoc_md351", null ],
      [ "Static Output Methods", "md_docs_2_console.html#autotoc_md353", [
        [ "<span class=\"tt\">static void Write(...)</span>", "md_docs_2_console.html#autotoc_md354", null ],
        [ "<span class=\"tt\">static void WriteLine(...)</span>", "md_docs_2_console.html#autotoc_md355", null ]
      ] ],
      [ "Static Input Methods", "md_docs_2_console.html#autotoc_md357", [
        [ "<span class=\"tt\">static int Read()</span>", "md_docs_2_console.html#autotoc_md358", null ],
        [ "<span class=\"tt\">static String ReadLine()</span>", "md_docs_2_console.html#autotoc_md359", null ]
      ] ],
      [ "Colors, Cursor &amp; Window Properties", "md_docs_2_console.html#autotoc_md361", [
        [ "<span class=\"tt\">static ConsoleColor GetForegroundColor()</span> / <span class=\"tt\">static void SetForegroundColor(ConsoleColor enumColor)</span>", "md_docs_2_console.html#autotoc_md362", null ],
        [ "<span class=\"tt\">static ConsoleColor GetBackgroundColor()</span> / <span class=\"tt\">static void SetBackgroundColor(ConsoleColor enumColor)</span>", "md_docs_2_console.html#autotoc_md363", null ],
        [ "<span class=\"tt\">static void ResetColor()</span>", "md_docs_2_console.html#autotoc_md364", null ],
        [ "<span class=\"tt\">static int GetCursorLeft()</span> / <span class=\"tt\">static void SetCursorLeft(int iLeft)</span>", "md_docs_2_console.html#autotoc_md365", null ],
        [ "<span class=\"tt\">static int GetCursorTop()</span> / <span class=\"tt\">static void SetCursorTop(int iTop)</span>", "md_docs_2_console.html#autotoc_md366", null ],
        [ "<span class=\"tt\">static void SetCursorPosition(int iLeft, int iTop)</span>", "md_docs_2_console.html#autotoc_md367", null ],
        [ "<span class=\"tt\">static bool GetCursorVisible()</span> / <span class=\"tt\">static void SetCursorVisible(bool bVisible)</span>", "md_docs_2_console.html#autotoc_md368", null ],
        [ "<span class=\"tt\">static int GetWindowWidth()</span> / <span class=\"tt\">static void SetWindowWidth(int iWidth)</span>", "md_docs_2_console.html#autotoc_md369", null ],
        [ "<span class=\"tt\">static int GetWindowHeight()</span> / <span class=\"tt\">static void SetWindowHeight(int iHeight)</span>", "md_docs_2_console.html#autotoc_md370", null ],
        [ "<span class=\"tt\">static bool GetCapsLock()</span> / <span class=\"tt\">static bool GetNumberLock()</span>", "md_docs_2_console.html#autotoc_md371", null ],
        [ "<span class=\"tt\">static String GetTitle()</span> / <span class=\"tt\">static void SetTitle(const String&amp; sTitle)</span>", "md_docs_2_console.html#autotoc_md372", null ],
        [ "<span class=\"tt\">static void Beep()</span>", "md_docs_2_console.html#autotoc_md373", null ],
        [ "<span class=\"tt\">static void Clear()</span>", "md_docs_2_console.html#autotoc_md374", null ]
      ] ],
      [ "Standard Stream Redirection &amp; Accessors", "md_docs_2_console.html#autotoc_md376", [
        [ "<span class=\"tt\">static SmartPointer&lt;IO::TextWriter&gt; Out()</span>", "md_docs_2_console.html#autotoc_md377", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;IO::TextWriter&gt; Error()</span>", "md_docs_2_console.html#autotoc_md378", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;IO::TextReader&gt; In()</span>", "md_docs_2_console.html#autotoc_md379", null ],
        [ "<span class=\"tt\">static void SetOut(const SmartPointer&lt;IO::TextWriter&gt;&amp; pOutWriter)</span>", "md_docs_2_console.html#autotoc_md380", null ],
        [ "<span class=\"tt\">static void SetError(const SmartPointer&lt;IO::TextWriter&gt;&amp; pErrorWriter)</span>", "md_docs_2_console.html#autotoc_md381", null ],
        [ "<span class=\"tt\">static void SetIn(const SmartPointer&lt;IO::TextReader&gt;&amp; pInReader)</span>", "md_docs_2_console.html#autotoc_md382", null ],
        [ "<span class=\"tt\">static void SetIn(const String&amp; sValue)</span>", "md_docs_2_console.html#autotoc_md383", null ],
        [ "<span class=\"tt\">static void ClearInputs()</span>", "md_docs_2_console.html#autotoc_md384", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; GetOutputs()</span>", "md_docs_2_console.html#autotoc_md385", null ]
      ] ],
      [ "Example", "md_docs_2_console.html#autotoc_md387", null ]
    ] ],
    [ "ControllerBase", "md_docs_2_controller_base.html", [
      [ "Syntax", "md_docs_2_controller_base.html#autotoc_md390", null ],
      [ "Protected Members &amp; Context", "md_docs_2_controller_base.html#autotoc_md392", null ],
      [ "Helper Methods", "md_docs_2_controller_base.html#autotoc_md394", [
        [ "<span class=\"tt\">template &lt;typename U&gt; String Ok(const U&amp; value)</span>", "md_docs_2_controller_base.html#autotoc_md395", null ],
        [ "<span class=\"tt\">template &lt;typename U&gt; String Created(const U&amp; value)</span>", "md_docs_2_controller_base.html#autotoc_md396", null ],
        [ "<span class=\"tt\">String NoContent()</span>", "md_docs_2_controller_base.html#autotoc_md397", null ],
        [ "<span class=\"tt\">String NotFound(const String&amp; error = \"Not Found\")</span>", "md_docs_2_controller_base.html#autotoc_md398", null ],
        [ "<span class=\"tt\">String BadRequest(const String&amp; error = \"Bad Request\")</span>", "md_docs_2_controller_base.html#autotoc_md399", null ],
        [ "<span class=\"tt\">String Unauthorized(const String&amp; error = \"Unauthorized\")</span>", "md_docs_2_controller_base.html#autotoc_md400", null ],
        [ "<span class=\"tt\">String Forbidden(const String&amp; error = \"Forbidden\")</span>", "md_docs_2_controller_base.html#autotoc_md401", null ],
        [ "<span class=\"tt\">bool Authorize(const String&amp; sSecretKey, Dictionary&lt;String, String&gt;&amp; claims)</span>", "md_docs_2_controller_base.html#autotoc_md402", null ]
      ] ],
      [ "Example", "md_docs_2_controller_base.html#autotoc_md404", null ]
    ] ],
    [ "Convert", "md_docs_2_convert.html", [
      [ "Syntax", "md_docs_2_convert.html#autotoc_md407", null ],
      [ "Static Methods", "md_docs_2_convert.html#autotoc_md409", [
        [ "Boolean Conversions", "md_docs_2_convert.html#autotoc_md410", null ],
        [ "Integer Conversions (Signed)", "md_docs_2_convert.html#autotoc_md411", null ],
        [ "Integer Conversions (Unsigned)", "md_docs_2_convert.html#autotoc_md412", null ],
        [ "Floating-Point Conversions", "md_docs_2_convert.html#autotoc_md413", null ],
        [ "Character Conversions", "md_docs_2_convert.html#autotoc_md414", null ],
        [ "Base64 Encoding &amp; Decoding", "md_docs_2_convert.html#autotoc_md415", null ],
        [ "String Conversions", "md_docs_2_convert.html#autotoc_md416", null ]
      ] ],
      [ "Example", "md_docs_2_convert.html#autotoc_md418", null ]
    ] ],
    [ "CriticalSection &amp; Lock&lt;T&gt;", "md_docs_2_critical_section.html", [
      [ "<span class=\"tt\">CriticalSection</span>", "md_docs_2_critical_section.html#autotoc_md421", [
        [ "Syntax", "md_docs_2_critical_section.html#autotoc_md422", null ],
        [ "Constructors", "md_docs_2_critical_section.html#autotoc_md423", null ],
        [ "Member Functions", "md_docs_2_critical_section.html#autotoc_md424", null ]
      ] ],
      [ "<span class=\"tt\">Lock&lt;T&gt;</span> (RAII Lock Wrapper)", "md_docs_2_critical_section.html#autotoc_md426", [
        [ "Syntax", "md_docs_2_critical_section.html#autotoc_md427", null ],
        [ "Constructors", "md_docs_2_critical_section.html#autotoc_md428", null ]
      ] ],
      [ "Example", "md_docs_2_critical_section.html#autotoc_md430", null ]
    ] ],
    [ "SqlConnection, SqlCommand, SqlDataReader &amp; SqlParameter", "md_docs_2_database.html", [
      [ "<span class=\"tt\">SqlConnection</span>", "md_docs_2_database.html#autotoc_md433", [
        [ "Constructors", "md_docs_2_database.html#autotoc_md434", null ],
        [ "Member Functions", "md_docs_2_database.html#autotoc_md435", null ]
      ] ],
      [ "<span class=\"tt\">SqlCommand</span>", "md_docs_2_database.html#autotoc_md437", [
        [ "Constructors", "md_docs_2_database.html#autotoc_md438", null ],
        [ "Member Functions", "md_docs_2_database.html#autotoc_md439", null ]
      ] ],
      [ "<span class=\"tt\">SqlDataReader</span>", "md_docs_2_database.html#autotoc_md441", [
        [ "Member Functions", "md_docs_2_database.html#autotoc_md442", null ]
      ] ],
      [ "<span class=\"tt\">SqlParameter</span>", "md_docs_2_database.html#autotoc_md444", [
        [ "Constructors", "md_docs_2_database.html#autotoc_md445", null ]
      ] ],
      [ "Example", "md_docs_2_database.html#autotoc_md447", null ]
    ] ],
    [ "DateTime", "md_docs_2_date_time.html", [
      [ "Syntax", "md_docs_2_date_time.html#autotoc_md450", null ],
      [ "Constructors", "md_docs_2_date_time.html#autotoc_md452", [
        [ "<span class=\"tt\">DateTime()</span>", "md_docs_2_date_time.html#autotoc_md453", null ],
        [ "<span class=\"tt\">DateTime(int64_t nTicks)</span>", "md_docs_2_date_time.html#autotoc_md454", null ],
        [ "<span class=\"tt\">DateTime(int64_t nTicks, DateTimeKind kind)</span>", "md_docs_2_date_time.html#autotoc_md455", null ],
        [ "<span class=\"tt\">DateTime(int year, int month, int day)</span>", "md_docs_2_date_time.html#autotoc_md456", null ],
        [ "<span class=\"tt\">DateTime(int year, int month, int day, int hour, int minute, int second)</span>", "md_docs_2_date_time.html#autotoc_md457", null ],
        [ "<span class=\"tt\">DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)</span>", "md_docs_2_date_time.html#autotoc_md458", null ],
        [ "<span class=\"tt\">DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)</span>", "md_docs_2_date_time.html#autotoc_md459", null ],
        [ "<span class=\"tt\">DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)</span>", "md_docs_2_date_time.html#autotoc_md460", null ]
      ] ],
      [ "Static Methods", "md_docs_2_date_time.html#autotoc_md462", [
        [ "<span class=\"tt\">static DateTime Now()</span>", "md_docs_2_date_time.html#autotoc_md463", null ],
        [ "<span class=\"tt\">static DateTime UtcNow()</span>", "md_docs_2_date_time.html#autotoc_md464", null ],
        [ "<span class=\"tt\">static DateTime Today()</span>", "md_docs_2_date_time.html#autotoc_md465", null ],
        [ "<span class=\"tt\">static DateTime Parse(const String&amp; s)</span>", "md_docs_2_date_time.html#autotoc_md466", null ],
        [ "<span class=\"tt\">static DateTime ParseExact(const String&amp; s, const String&amp; format)</span>", "md_docs_2_date_time.html#autotoc_md467", null ],
        [ "<span class=\"tt\">static bool TryParse(const String&amp; s, DateTime&amp; result)</span>", "md_docs_2_date_time.html#autotoc_md468", null ],
        [ "<span class=\"tt\">static bool TryParseExact(const String&amp; s, const String&amp; format, DateTime&amp; result)</span>", "md_docs_2_date_time.html#autotoc_md469", null ],
        [ "<span class=\"tt\">static int DaysInMonth(int year, int month)</span>", "md_docs_2_date_time.html#autotoc_md470", null ],
        [ "<span class=\"tt\">static bool IsLeapYear(int year)</span>", "md_docs_2_date_time.html#autotoc_md471", null ]
      ] ],
      [ "Properties &amp; Component Accessors", "md_docs_2_date_time.html#autotoc_md473", [
        [ "<span class=\"tt\">int64_t GetTicks() const</span>", "md_docs_2_date_time.html#autotoc_md474", null ],
        [ "<span class=\"tt\">DateTimeKind GetKind() const</span>", "md_docs_2_date_time.html#autotoc_md475", null ],
        [ "<span class=\"tt\">int GetYear() const</span>", "md_docs_2_date_time.html#autotoc_md476", null ],
        [ "<span class=\"tt\">int GetMonth() const</span>", "md_docs_2_date_time.html#autotoc_md477", null ],
        [ "<span class=\"tt\">int GetDay() const</span>", "md_docs_2_date_time.html#autotoc_md478", null ],
        [ "<span class=\"tt\">int GetHour() const</span>", "md_docs_2_date_time.html#autotoc_md479", null ],
        [ "<span class=\"tt\">int GetMinute() const</span>", "md_docs_2_date_time.html#autotoc_md480", null ],
        [ "<span class=\"tt\">int GetSecond() const</span>", "md_docs_2_date_time.html#autotoc_md481", null ],
        [ "<span class=\"tt\">int GetMillisecond() const</span>", "md_docs_2_date_time.html#autotoc_md482", null ],
        [ "<span class=\"tt\">int GetDayOfYear() const</span>", "md_docs_2_date_time.html#autotoc_md483", null ],
        [ "<span class=\"tt\">int GetDayOfWeek() const</span>", "md_docs_2_date_time.html#autotoc_md484", null ],
        [ "<span class=\"tt\">DateTime GetDate() const</span>", "md_docs_2_date_time.html#autotoc_md485", null ],
        [ "<span class=\"tt\">TimeSpan GetTimeOfDay() const</span>", "md_docs_2_date_time.html#autotoc_md486", null ]
      ] ],
      [ "Arithmetic &amp; Transformations", "md_docs_2_date_time.html#autotoc_md488", [
        [ "<span class=\"tt\">DateTime Add(TimeSpan value) const</span>", "md_docs_2_date_time.html#autotoc_md489", null ],
        [ "<span class=\"tt\">DateTime AddDays(double value) const</span>", "md_docs_2_date_time.html#autotoc_md490", null ],
        [ "<span class=\"tt\">DateTime AddHours(double value) const</span>", "md_docs_2_date_time.html#autotoc_md491", null ],
        [ "<span class=\"tt\">DateTime AddMinutes(double value) const</span>", "md_docs_2_date_time.html#autotoc_md492", null ],
        [ "<span class=\"tt\">DateTime AddSeconds(double value) const</span>", "md_docs_2_date_time.html#autotoc_md493", null ],
        [ "<span class=\"tt\">DateTime AddMilliseconds(double value) const</span>", "md_docs_2_date_time.html#autotoc_md494", null ],
        [ "<span class=\"tt\">DateTime AddMonths(int months) const</span>", "md_docs_2_date_time.html#autotoc_md495", null ],
        [ "<span class=\"tt\">DateTime AddYears(int value) const</span>", "md_docs_2_date_time.html#autotoc_md496", null ],
        [ "<span class=\"tt\">DateTime AddTicks(int64_t value) const</span>", "md_docs_2_date_time.html#autotoc_md497", null ],
        [ "<span class=\"tt\">DateTime ToLocalTime() const</span>", "md_docs_2_date_time.html#autotoc_md498", null ],
        [ "<span class=\"tt\">DateTime ToUniversalTime() const</span>", "md_docs_2_date_time.html#autotoc_md499", null ]
      ] ],
      [ "Formatting &amp; String Representations", "md_docs_2_date_time.html#autotoc_md501", [
        [ "<span class=\"tt\">String ToString() const</span>", "md_docs_2_date_time.html#autotoc_md502", null ],
        [ "<span class=\"tt\">String ToString(const String&amp; sFormat) const</span>", "md_docs_2_date_time.html#autotoc_md503", null ]
      ] ],
      [ "Operators", "md_docs_2_date_time.html#autotoc_md505", null ],
      [ "Example", "md_docs_2_date_time.html#autotoc_md507", null ]
    ] ],
    [ "DateTimeOffset", "md_docs_2_date_time_offset.html", [
      [ "Syntax", "md_docs_2_date_time_offset.html#autotoc_md510", null ],
      [ "Constructors", "md_docs_2_date_time_offset.html#autotoc_md512", [
        [ "<span class=\"tt\">DateTimeOffset()</span>", "md_docs_2_date_time_offset.html#autotoc_md513", null ],
        [ "<span class=\"tt\">DateTimeOffset(int64_t ticks)</span>", "md_docs_2_date_time_offset.html#autotoc_md514", null ]
      ] ],
      [ "Static Methods", "md_docs_2_date_time_offset.html#autotoc_md516", [
        [ "<span class=\"tt\">static DateTimeOffset Now()</span>", "md_docs_2_date_time_offset.html#autotoc_md517", null ],
        [ "<span class=\"tt\">static DateTimeOffset UtcNow()</span>", "md_docs_2_date_time_offset.html#autotoc_md518", null ]
      ] ],
      [ "Member Functions &amp; Operators", "md_docs_2_date_time_offset.html#autotoc_md520", [
        [ "<span class=\"tt\">int64_t GetTicks() const</span>", "md_docs_2_date_time_offset.html#autotoc_md521", null ],
        [ "<span class=\"tt\">String ToString() const</span> / <span class=\"tt\">String ToString(const String&amp; sFormat) const</span>", "md_docs_2_date_time_offset.html#autotoc_md522", null ],
        [ "<span class=\"tt\">TimeSpan operator-(const DateTimeOffset&amp; other) const</span>", "md_docs_2_date_time_offset.html#autotoc_md523", null ],
        [ "<span class=\"tt\">bool operator==(const DateTimeOffset&amp; other) const</span> / <span class=\"tt\">!=</span> / <span class=\"tt\">&lt;</span> / <span class=\"tt\">&lt;=</span> / <span class=\"tt\">&gt;</span> / <span class=\"tt\">&gt;=</span>", "md_docs_2_date_time_offset.html#autotoc_md524", null ]
      ] ],
      [ "Example", "md_docs_2_date_time_offset.html#autotoc_md526", null ]
    ] ],
    [ "DaylightTime", "md_docs_2_daylight_time.html", [
      [ "Syntax", "md_docs_2_daylight_time.html#autotoc_md529", null ],
      [ "Constructors", "md_docs_2_daylight_time.html#autotoc_md531", [
        [ "<span class=\"tt\">DaylightTime(DateTimeOffset start, DateTimeOffset end, TimeSpan delta)</span>", "md_docs_2_daylight_time.html#autotoc_md532", null ]
      ] ],
      [ "Properties", "md_docs_2_daylight_time.html#autotoc_md534", [
        [ "<span class=\"tt\">DateTimeOffset GetStart() const</span>", "md_docs_2_daylight_time.html#autotoc_md535", null ],
        [ "<span class=\"tt\">DateTimeOffset GetEnd() const</span>", "md_docs_2_daylight_time.html#autotoc_md536", null ],
        [ "<span class=\"tt\">TimeSpan GetDelta() const</span>", "md_docs_2_daylight_time.html#autotoc_md537", null ]
      ] ],
      [ "Example", "md_docs_2_daylight_time.html#autotoc_md539", null ]
    ] ],
    [ "DependencyInjection (ServiceCollection &amp; ServiceProvider)", "md_docs_2_dependency_injection.html", [
      [ "<span class=\"tt\">ServiceLifetime</span> Enum", "md_docs_2_dependency_injection.html#autotoc_md542", null ],
      [ "<span class=\"tt\">ServiceCollection</span> / <span class=\"tt\">IServiceCollection</span>", "md_docs_2_dependency_injection.html#autotoc_md544", [
        [ "Registration Methods", "md_docs_2_dependency_injection.html#autotoc_md545", [
          [ "Singleton Registrations", "md_docs_2_dependency_injection.html#autotoc_md546", null ],
          [ "Transient Registrations", "md_docs_2_dependency_injection.html#autotoc_md547", null ],
          [ "Scoped Registrations", "md_docs_2_dependency_injection.html#autotoc_md548", null ]
        ] ]
      ] ],
      [ "<span class=\"tt\">ServiceProvider</span> / <span class=\"tt\">IServiceProvider</span>", "md_docs_2_dependency_injection.html#autotoc_md550", [
        [ "Methods", "md_docs_2_dependency_injection.html#autotoc_md551", null ]
      ] ],
      [ "<span class=\"tt\">ServiceScope</span> &amp; <span class=\"tt\">ServiceScopeFactory</span>", "md_docs_2_dependency_injection.html#autotoc_md553", null ],
      [ "Example", "md_docs_2_dependency_injection.html#autotoc_md555", null ]
    ] ],
    [ "Dictionary&lt;TKey, TValue&gt;", "md_docs_2_dictionary.html", [
      [ "Syntax", "md_docs_2_dictionary.html#autotoc_md558", null ],
      [ "Constructors &amp; Assignment", "md_docs_2_dictionary.html#autotoc_md560", [
        [ "<span class=\"tt\">Dictionary()</span>", "md_docs_2_dictionary.html#autotoc_md561", null ],
        [ "<span class=\"tt\">Dictionary(const Dictionary&amp; other)</span> / <span class=\"tt\">Dictionary(Dictionary&amp;&amp; other) noexcept</span>", "md_docs_2_dictionary.html#autotoc_md562", null ],
        [ "<span class=\"tt\">Dictionary&amp; operator=(const Dictionary&amp; other)</span> / <span class=\"tt\">operator=(Dictionary&amp;&amp; other) noexcept</span>", "md_docs_2_dictionary.html#autotoc_md563", null ]
      ] ],
      [ "Properties", "md_docs_2_dictionary.html#autotoc_md565", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_dictionary.html#autotoc_md566", null ],
        [ "<span class=\"tt\">TValue&amp; operator[](const TKey&amp; key)</span>", "md_docs_2_dictionary.html#autotoc_md567", null ],
        [ "<span class=\"tt\">const TValue&amp; operator[](const TKey&amp; key) const</span>", "md_docs_2_dictionary.html#autotoc_md568", null ]
      ] ],
      [ "Member Functions", "md_docs_2_dictionary.html#autotoc_md570", [
        [ "<span class=\"tt\">void Add(const TKey&amp; key, const TValue&amp; value)</span>", "md_docs_2_dictionary.html#autotoc_md571", null ],
        [ "<span class=\"tt\">bool Remove(const TKey&amp; key)</span>", "md_docs_2_dictionary.html#autotoc_md572", null ],
        [ "<span class=\"tt\">bool ContainsKey(const TKey&amp; key) const</span>", "md_docs_2_dictionary.html#autotoc_md573", null ],
        [ "<span class=\"tt\">bool TryGetValue(const TKey&amp; key, TValue&amp; value) const</span>", "md_docs_2_dictionary.html#autotoc_md574", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_dictionary.html#autotoc_md575", null ],
        [ "<span class=\"tt\">Array&lt;TKey&gt; GetKeys() const</span>", "md_docs_2_dictionary.html#autotoc_md576", null ],
        [ "<span class=\"tt\">Array&lt;TValue&gt; GetValues() const</span>", "md_docs_2_dictionary.html#autotoc_md577", null ],
        [ "<span class=\"tt\">begin()</span> / <span class=\"tt\">end()</span>", "md_docs_2_dictionary.html#autotoc_md578", null ]
      ] ],
      [ "Example", "md_docs_2_dictionary.html#autotoc_md580", null ]
    ] ],
    [ "Directory", "md_docs_2_directory.html", [
      [ "Syntax", "md_docs_2_directory.html#autotoc_md583", null ],
      [ "Static Methods", "md_docs_2_directory.html#autotoc_md585", [
        [ "<span class=\"tt\">static bool Exists(const String&amp; sPath)</span>", "md_docs_2_directory.html#autotoc_md586", null ],
        [ "<span class=\"tt\">static void CreateDirectory(const String&amp; sPath)</span> / <span class=\"tt\">static void CreateDirectory(const String&amp; sPath, bool bRecursive)</span>", "md_docs_2_directory.html#autotoc_md587", null ],
        [ "<span class=\"tt\">static void Delete(const String&amp; sPath)</span> / <span class=\"tt\">static void Delete(const String&amp; sPath, bool bRecursive)</span>", "md_docs_2_directory.html#autotoc_md588", null ],
        [ "<span class=\"tt\">static void Move(const String&amp; sSourceDirName, const String&amp; sDestDirName)</span>", "md_docs_2_directory.html#autotoc_md589", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; GetFiles(const String&amp; sPath)</span> / <span class=\"tt\">static Array&lt;String&gt; GetFiles(const String&amp; sPath, const String&amp; sSearchPattern)</span>", "md_docs_2_directory.html#autotoc_md590", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; GetDirectories(const String&amp; sPath)</span> / <span class=\"tt\">static Array&lt;String&gt; GetDirectories(const String&amp; sPath, const String&amp; sSearchPattern)</span>", "md_docs_2_directory.html#autotoc_md591", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; GetFileSystemEntries(const String&amp; sPath)</span> / <span class=\"tt\">static Array&lt;String&gt; GetFileSystemEntries(const String&amp; sPath, const String&amp; sSearchPattern)</span>", "md_docs_2_directory.html#autotoc_md592", null ],
        [ "<span class=\"tt\">static String GetCurrentDirectory()</span>", "md_docs_2_directory.html#autotoc_md593", null ],
        [ "<span class=\"tt\">static void SetCurrentDirectory(const String&amp; sPath)</span>", "md_docs_2_directory.html#autotoc_md594", null ],
        [ "<span class=\"tt\">static String GetDirectoryRoot(const String&amp; sPath)</span>", "md_docs_2_directory.html#autotoc_md595", null ],
        [ "<span class=\"tt\">static DateTimeOffset GetCreationTime(const String&amp; sPath)</span>", "md_docs_2_directory.html#autotoc_md596", null ],
        [ "<span class=\"tt\">static DateTimeOffset GetLastWriteTime(const String&amp; sPath)</span>", "md_docs_2_directory.html#autotoc_md597", null ],
        [ "<span class=\"tt\">static DateTimeOffset GetLastAccessTime(const String&amp; sPath)</span>", "md_docs_2_directory.html#autotoc_md598", null ]
      ] ],
      [ "Example", "md_docs_2_directory.html#autotoc_md600", null ]
    ] ],
    [ "Environment", "md_docs_2_environment.html", [
      [ "Syntax", "md_docs_2_environment.html#autotoc_md603", null ],
      [ "Static Properties &amp; Methods", "md_docs_2_environment.html#autotoc_md605", [
        [ "<span class=\"tt\">static String GetMachineName()</span>", "md_docs_2_environment.html#autotoc_md606", null ],
        [ "<span class=\"tt\">static String GetUserName()</span>", "md_docs_2_environment.html#autotoc_md607", null ],
        [ "<span class=\"tt\">static String GetUserDomainName()</span>", "md_docs_2_environment.html#autotoc_md608", null ],
        [ "<span class=\"tt\">static int GetProcessorCount()</span>", "md_docs_2_environment.html#autotoc_md609", null ],
        [ "<span class=\"tt\">static String GetNewLine()</span>", "md_docs_2_environment.html#autotoc_md610", null ],
        [ "<span class=\"tt\">static String GetCurrentDirectory()</span>", "md_docs_2_environment.html#autotoc_md611", null ],
        [ "<span class=\"tt\">static String GetSystemDirectory()</span>", "md_docs_2_environment.html#autotoc_md612", null ],
        [ "<span class=\"tt\">static String GetOSVersion()</span>", "md_docs_2_environment.html#autotoc_md613", null ],
        [ "<span class=\"tt\">static OperatingSystem GetOperatingSystem()</span>", "md_docs_2_environment.html#autotoc_md614", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; GetCommandLineArgs()</span>", "md_docs_2_environment.html#autotoc_md615", null ],
        [ "<span class=\"tt\">static String ExpandEnvironmentVariables(const String&amp; sName)</span>", "md_docs_2_environment.html#autotoc_md616", null ],
        [ "<span class=\"tt\">static String GetEnvironmentVariable(const String&amp; sVariable)</span>", "md_docs_2_environment.html#autotoc_md617", null ],
        [ "<span class=\"tt\">static void SetEnvironmentVariable(const String&amp; sVariable, const String&amp; sValue)</span>", "md_docs_2_environment.html#autotoc_md618", null ],
        [ "<span class=\"tt\">static Collections::Generic::Dictionary&lt;String, String&gt; GetEnvironmentVariables()</span>", "md_docs_2_environment.html#autotoc_md619", null ],
        [ "<span class=\"tt\">static String GetFolderPath(SpecialFolder eFolder)</span>", "md_docs_2_environment.html#autotoc_md620", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; GetLogicalDrives()</span>", "md_docs_2_environment.html#autotoc_md621", null ],
        [ "<span class=\"tt\">static int64_t GetWorkingSet()</span>", "md_docs_2_environment.html#autotoc_md622", null ],
        [ "<span class=\"tt\">static void Exit(int iExitCode)</span>", "md_docs_2_environment.html#autotoc_md623", null ]
      ] ],
      [ "Example", "md_docs_2_environment.html#autotoc_md625", null ]
    ] ],
    [ "EtwLogReader &amp; EtwEvent", "md_docs_2_etw_log_reader.html", [
      [ "Data Structures &amp; Enums", "md_docs_2_etw_log_reader.html#autotoc_md628", [
        [ "<span class=\"tt\">EtwEventLevel</span>", "md_docs_2_etw_log_reader.html#autotoc_md629", null ],
        [ "<span class=\"tt\">EtwEvent</span>", "md_docs_2_etw_log_reader.html#autotoc_md630", null ],
        [ "<span class=\"tt\">EtwEventLevelCounts</span>", "md_docs_2_etw_log_reader.html#autotoc_md631", null ]
      ] ],
      [ "<span class=\"tt\">EtwLogReader</span> Class", "md_docs_2_etw_log_reader.html#autotoc_md633", [
        [ "Syntax", "md_docs_2_etw_log_reader.html#autotoc_md634", null ]
      ] ],
      [ "Member Functions (Live Subscriptions)", "md_docs_2_etw_log_reader.html#autotoc_md636", [
        [ "<span class=\"tt\">void StartListening(const String&amp; sChannelName, Action&lt;const EtwEvent&amp;&gt; fnCallback)</span>", "md_docs_2_etw_log_reader.html#autotoc_md637", null ],
        [ "<span class=\"tt\">void StopListening()</span>", "md_docs_2_etw_log_reader.html#autotoc_md638", null ],
        [ "<span class=\"tt\">bool IsListening() const</span>", "md_docs_2_etw_log_reader.html#autotoc_md639", null ],
        [ "<span class=\"tt\">String GetListeningChannel() const</span>", "md_docs_2_etw_log_reader.html#autotoc_md640", null ]
      ] ],
      [ "Static Methods", "md_docs_2_etw_log_reader.html#autotoc_md642", [
        [ "<span class=\"tt\">static List&lt;String&gt; GetEventChannels()</span>", "md_docs_2_etw_log_reader.html#autotoc_md643", null ],
        [ "<span class=\"tt\">static unsigned long long GetChannelEventCount(const String&amp; sChannelName)</span>", "md_docs_2_etw_log_reader.html#autotoc_md644", null ],
        [ "<span class=\"tt\">static EtwEventLevelCounts GetChannelEventLevelCounts(const String&amp; sChannelName)</span>", "md_docs_2_etw_log_reader.html#autotoc_md645", null ],
        [ "<span class=\"tt\">static List&lt;EtwEvent&gt; ReadEvents(...)</span>", "md_docs_2_etw_log_reader.html#autotoc_md646", null ]
      ] ],
      [ "Example", "md_docs_2_etw_log_reader.html#autotoc_md648", null ]
    ] ],
    [ "EventHandler &amp; EventArgs", "md_docs_2_event_handler.html", [
      [ "Overview", "md_docs_2_event_handler.html#autotoc_md651", null ],
      [ "Syntax", "md_docs_2_event_handler.html#autotoc_md653", null ],
      [ "EventArgs API Reference", "md_docs_2_event_handler.html#autotoc_md655", [
        [ "<span class=\"tt\">EventArgs()</span>", "md_docs_2_event_handler.html#autotoc_md656", null ],
        [ "<span class=\"tt\">static const EventArgs&amp; Empty()</span>", "md_docs_2_event_handler.html#autotoc_md657", null ]
      ] ],
      [ "EventHandler&lt;TEventArgs&gt; API Reference", "md_docs_2_event_handler.html#autotoc_md659", [
        [ "Subscriptions", "md_docs_2_event_handler.html#autotoc_md660", [
          [ "<span class=\"tt\">size_t Add(const Action&lt;const void*, const TEventArgs&amp;&gt;&amp; fnHandler)</span>", "md_docs_2_event_handler.html#autotoc_md661", null ],
          [ "<span class=\"tt\">template &lt;typename F&gt; size_t Add(F&amp;&amp; fnHandler)</span>", "md_docs_2_event_handler.html#autotoc_md662", null ],
          [ "<span class=\"tt\">template &lt;typename TClass&gt; size_t Add(TClass* pInstance, void (TClass::*pMethod)(const void*, const TEventArgs&amp;))</span>", "md_docs_2_event_handler.html#autotoc_md663", null ],
          [ "<span class=\"tt\">size_t operator+=(const Action&lt;const void*, const TEventArgs&amp;&gt;&amp; fnHandler)</span>", "md_docs_2_event_handler.html#autotoc_md664", null ],
          [ "<span class=\"tt\">template &lt;typename F&gt; size_t operator+=(F&amp;&amp; fnHandler)</span>", "md_docs_2_event_handler.html#autotoc_md665", null ]
        ] ],
        [ "Unsubscription &amp; Management", "md_docs_2_event_handler.html#autotoc_md667", [
          [ "<span class=\"tt\">bool Remove(size_t nToken)</span>", "md_docs_2_event_handler.html#autotoc_md668", null ],
          [ "<span class=\"tt\">bool operator-=(size_t nToken)</span>", "md_docs_2_event_handler.html#autotoc_md669", null ],
          [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_event_handler.html#autotoc_md670", null ]
        ] ],
        [ "Invocation &amp; Diagnostics", "md_docs_2_event_handler.html#autotoc_md672", [
          [ "<span class=\"tt\">void Invoke(const void* pSender, const TEventArgs&amp; e) const</span>", "md_docs_2_event_handler.html#autotoc_md673", null ],
          [ "<span class=\"tt\">void operator()(const void* pSender, const TEventArgs&amp; e) const</span>", "md_docs_2_event_handler.html#autotoc_md674", null ],
          [ "<span class=\"tt\">bool IsEmpty() const</span>", "md_docs_2_event_handler.html#autotoc_md675", null ],
          [ "<span class=\"tt\">size_t GetSubscriberCount() const</span>", "md_docs_2_event_handler.html#autotoc_md676", null ],
          [ "<span class=\"tt\">explicit operator bool() const</span>", "md_docs_2_event_handler.html#autotoc_md677", null ]
        ] ]
      ] ],
      [ "Real-World Example", "md_docs_2_event_handler.html#autotoc_md679", null ]
    ] ],
    [ "EventLog &amp; EventLogEntry", "md_docs_2_event_log.html", [
      [ "<span class=\"tt\">EventLogEntryType</span> Enum", "md_docs_2_event_log.html#autotoc_md682", null ],
      [ "<span class=\"tt\">EventLogEntry</span> Class", "md_docs_2_event_log.html#autotoc_md684", [
        [ "Syntax", "md_docs_2_event_log.html#autotoc_md685", null ],
        [ "Constructors", "md_docs_2_event_log.html#autotoc_md686", null ],
        [ "Properties", "md_docs_2_event_log.html#autotoc_md687", null ]
      ] ],
      [ "<span class=\"tt\">EventLog</span> Class", "md_docs_2_event_log.html#autotoc_md689", [
        [ "Syntax", "md_docs_2_event_log.html#autotoc_md690", null ]
      ] ],
      [ "Constructors", "md_docs_2_event_log.html#autotoc_md692", [
        [ "<span class=\"tt\">EventLog()</span>", "md_docs_2_event_log.html#autotoc_md693", null ],
        [ "<span class=\"tt\">EventLog(const String&amp; sLogName)</span>", "md_docs_2_event_log.html#autotoc_md694", null ],
        [ "<span class=\"tt\">EventLog(const String&amp; sLogName, const String&amp; sMachineName)</span>", "md_docs_2_event_log.html#autotoc_md695", null ],
        [ "<span class=\"tt\">EventLog(const String&amp; sLogName, const String&amp; sMachineName, const String&amp; sSource)</span>", "md_docs_2_event_log.html#autotoc_md696", null ]
      ] ],
      [ "Properties", "md_docs_2_event_log.html#autotoc_md698", null ],
      [ "Member Functions", "md_docs_2_event_log.html#autotoc_md700", [
        [ "<span class=\"tt\">List&lt;EventLogEntry&gt; GetEntries() const</span>", "md_docs_2_event_log.html#autotoc_md701", null ],
        [ "<span class=\"tt\">void WriteEntry(const String&amp; sMessage)</span>", "md_docs_2_event_log.html#autotoc_md702", null ],
        [ "<span class=\"tt\">void WriteEntry(const String&amp; sMessage, EventLogEntryType eType)</span>", "md_docs_2_event_log.html#autotoc_md703", null ],
        [ "<span class=\"tt\">void WriteEntry(const String&amp; sMessage, EventLogEntryType eType, int iEventID)</span>", "md_docs_2_event_log.html#autotoc_md704", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_event_log.html#autotoc_md705", null ],
        [ "<span class=\"tt\">void Close()</span>", "md_docs_2_event_log.html#autotoc_md706", null ]
      ] ],
      [ "Static Methods", "md_docs_2_event_log.html#autotoc_md708", [
        [ "<span class=\"tt\">static void WriteEntry(const String&amp; sSource, const String&amp; sMessage)</span>", "md_docs_2_event_log.html#autotoc_md709", null ],
        [ "<span class=\"tt\">static void WriteEntry(const String&amp; sSource, const String&amp; sMessage, EventLogEntryType eType)</span>", "md_docs_2_event_log.html#autotoc_md710", null ],
        [ "<span class=\"tt\">static void WriteEntry(const String&amp; sSource, const String&amp; sMessage, EventLogEntryType eType, int iEventID)</span>", "md_docs_2_event_log.html#autotoc_md711", null ],
        [ "<span class=\"tt\">static bool SourceExists(const String&amp; sSource)</span> / <span class=\"tt\">static bool SourceExists(const String&amp; sSource, const String&amp; sMachineName)</span>", "md_docs_2_event_log.html#autotoc_md712", null ],
        [ "<span class=\"tt\">static void CreateEventSource(const String&amp; sSource, const String&amp; sLogName)</span>", "md_docs_2_event_log.html#autotoc_md713", null ],
        [ "<span class=\"tt\">static void Delete(const String&amp; sLogName)</span> / <span class=\"tt\">static void Delete(const String&amp; sLogName, const String&amp; sMachineName)</span>", "md_docs_2_event_log.html#autotoc_md714", null ],
        [ "<span class=\"tt\">static void DeleteEventSource(const String&amp; sSource)</span> / <span class=\"tt\">static void DeleteEventSource(const String&amp; sSource, const String&amp; sMachineName)</span>", "md_docs_2_event_log.html#autotoc_md715", null ],
        [ "<span class=\"tt\">static bool Exists(const String&amp; sLogName)</span> / <span class=\"tt\">static bool Exists(const String&amp; sLogName, const String&amp; sMachineName)</span>", "md_docs_2_event_log.html#autotoc_md716", null ],
        [ "<span class=\"tt\">static List&lt;EventLog&gt; GetEventLogs()</span> / <span class=\"tt\">static List&lt;EventLog&gt; GetEventLogs(const String&amp; sMachineName)</span>", "md_docs_2_event_log.html#autotoc_md717", null ]
      ] ],
      [ "Example", "md_docs_2_event_log.html#autotoc_md719", null ]
    ] ],
    [ "EventWaitHandle", "md_docs_2_event_wait_handle.html", [
      [ "Methods", "md_docs_2_event_wait_handle.html#autotoc_md721", [
        [ "<span class=\"tt\">EventWaitHandle(bool initialState, bool manualReset)</span>", "md_docs_2_event_wait_handle.html#autotoc_md722", null ],
        [ "<span class=\"tt\">EventWaitHandle(bool initialState, bool manualReset, const String&amp; sName)</span>", "md_docs_2_event_wait_handle.html#autotoc_md723", null ],
        [ "<span class=\"tt\">EventWaitHandle(bool initialState, bool manualReset, const String&amp; sName, bool&amp; bCreatedNew)</span>", "md_docs_2_event_wait_handle.html#autotoc_md724", null ],
        [ "<span class=\"tt\">static EventWaitHandle* OpenExisting(const String&amp; sName)</span>", "md_docs_2_event_wait_handle.html#autotoc_md725", null ],
        [ "<span class=\"tt\">static bool TryOpenExisting(const String&amp; sName, EventWaitHandle*&amp; pResult)</span>", "md_docs_2_event_wait_handle.html#autotoc_md726", null ],
        [ "<span class=\"tt\">bool Set()</span>", "md_docs_2_event_wait_handle.html#autotoc_md727", null ],
        [ "<span class=\"tt\">bool Reset()</span>", "md_docs_2_event_wait_handle.html#autotoc_md728", null ],
        [ "<span class=\"tt\">bool WaitOne()</span>", "md_docs_2_event_wait_handle.html#autotoc_md729", null ],
        [ "<span class=\"tt\">bool WaitOne(int millisecondsTimeout)</span>", "md_docs_2_event_wait_handle.html#autotoc_md730", null ]
      ] ],
      [ "Code Example", "md_docs_2_event_wait_handle.html#autotoc_md731", null ]
    ] ],
    [ "Exceptions", "md_docs_2_exceptions.html", [
      [ "Complete Exception Hierarchy", "md_docs_2_exceptions.html#autotoc_md734", null ],
      [ "Base <span class=\"tt\">Exception</span> Class", "md_docs_2_exceptions.html#autotoc_md736", [
        [ "Syntax", "md_docs_2_exceptions.html#autotoc_md737", null ],
        [ "Constructors &amp; Assignment", "md_docs_2_exceptions.html#autotoc_md738", null ],
        [ "Member Functions", "md_docs_2_exceptions.html#autotoc_md739", null ]
      ] ],
      [ "System &amp; Collections Exceptions", "md_docs_2_exceptions.html#autotoc_md741", null ],
      [ "I/O Namespace Exceptions (<span class=\"tt\">DotNetDupe::System::IO</span>)", "md_docs_2_exceptions.html#autotoc_md743", null ],
      [ "Example", "md_docs_2_exceptions.html#autotoc_md745", null ]
    ] ],
    [ "File", "md_docs_2_file.html", [
      [ "Syntax", "md_docs_2_file.html#autotoc_md748", null ],
      [ "Static Methods", "md_docs_2_file.html#autotoc_md750", [
        [ "<span class=\"tt\">static bool Exists(const String&amp; sPath)</span>", "md_docs_2_file.html#autotoc_md751", null ],
        [ "<span class=\"tt\">static String ReadAllText(const String&amp; sPath)</span>", "md_docs_2_file.html#autotoc_md752", null ],
        [ "<span class=\"tt\">static void WriteAllText(const String&amp; sPath, const String&amp; sContents)</span>", "md_docs_2_file.html#autotoc_md753", null ],
        [ "<span class=\"tt\">static Array&lt;String&gt; ReadAllLines(const String&amp; sPath)</span>", "md_docs_2_file.html#autotoc_md754", null ],
        [ "<span class=\"tt\">static void WriteAllLines(const String&amp; sPath, const Array&lt;String&gt;&amp; sContents)</span>", "md_docs_2_file.html#autotoc_md755", null ],
        [ "<span class=\"tt\">static void AppendAllText(const String&amp; sPath, const String&amp; sContents)</span>", "md_docs_2_file.html#autotoc_md756", null ],
        [ "<span class=\"tt\">static void AppendAllLines(const String&amp; sPath, const Array&lt;String&gt;&amp; sContents)</span>", "md_docs_2_file.html#autotoc_md757", null ],
        [ "<span class=\"tt\">static void Copy(const String&amp; sSourceFileName, const String&amp; sDestFileName, bool bOverwrite)</span>", "md_docs_2_file.html#autotoc_md758", null ],
        [ "<span class=\"tt\">static void Move(const String&amp; sSourceFileName, const String&amp; sDestFileName)</span>", "md_docs_2_file.html#autotoc_md759", null ],
        [ "<span class=\"tt\">static void Delete(const String&amp; sPath)</span>", "md_docs_2_file.html#autotoc_md760", null ],
        [ "<span class=\"tt\">static void Create(const String&amp; sPath)</span>", "md_docs_2_file.html#autotoc_md761", null ],
        [ "<span class=\"tt\">static bool GetAttributes(const String&amp; sPath, FileAttributes&amp; attrAttributes)</span>", "md_docs_2_file.html#autotoc_md762", null ],
        [ "<span class=\"tt\">static bool SetAttributes(const String&amp; sPath, FileAttributes attrAttributes)</span>", "md_docs_2_file.html#autotoc_md763", null ]
      ] ],
      [ "Example", "md_docs_2_file.html#autotoc_md765", null ]
    ] ],
    [ "FileDownloader, DownloadProgressChangedEventArgs &amp; DownloadCompletedEventArgs", "md_docs_2_file_downloader.html", [
      [ "<span class=\"tt\">DownloadStatus</span> Enum", "md_docs_2_file_downloader.html#autotoc_md768", null ],
      [ "<span class=\"tt\">DownloadProgressChangedEventArgs</span> Class", "md_docs_2_file_downloader.html#autotoc_md770", null ],
      [ "<span class=\"tt\">DownloadCompletedEventArgs</span> Class", "md_docs_2_file_downloader.html#autotoc_md772", null ],
      [ "<span class=\"tt\">FileDownloader</span> Class", "md_docs_2_file_downloader.html#autotoc_md774", [
        [ "Syntax", "md_docs_2_file_downloader.html#autotoc_md775", null ]
      ] ],
      [ "Events", "md_docs_2_file_downloader.html#autotoc_md777", [
        [ "<span class=\"tt\">EventHandler&lt;DownloadProgressChangedEventArgs&gt; DownloadProgressChanged</span>", "md_docs_2_file_downloader.html#autotoc_md778", null ],
        [ "<span class=\"tt\">EventHandler&lt;DownloadCompletedEventArgs&gt; DownloadCompleted</span>", "md_docs_2_file_downloader.html#autotoc_md779", null ]
      ] ],
      [ "Constructors", "md_docs_2_file_downloader.html#autotoc_md781", [
        [ "<span class=\"tt\">FileDownloader(const String&amp; sUrl, const String&amp; sDestinationPath)</span>", "md_docs_2_file_downloader.html#autotoc_md782", null ]
      ] ],
      [ "Member Functions", "md_docs_2_file_downloader.html#autotoc_md784", [
        [ "<span class=\"tt\">bool Start()</span>", "md_docs_2_file_downloader.html#autotoc_md785", null ],
        [ "<span class=\"tt\">void Pause()</span>", "md_docs_2_file_downloader.html#autotoc_md786", null ],
        [ "<span class=\"tt\">bool Resume()</span>", "md_docs_2_file_downloader.html#autotoc_md787", null ],
        [ "<span class=\"tt\">DownloadProgress GetProgress() const</span>", "md_docs_2_file_downloader.html#autotoc_md788", null ],
        [ "<span class=\"tt\">DownloadStatus GetStatus() const</span>", "md_docs_2_file_downloader.html#autotoc_md789", null ],
        [ "<span class=\"tt\">void AddHeaders(const Dictionary&lt;String, String&gt;&amp; headers)</span>", "md_docs_2_file_downloader.html#autotoc_md790", null ],
        [ "<span class=\"tt\">void SetUserAgent(const String&amp; sUserAgent)</span>", "md_docs_2_file_downloader.html#autotoc_md791", null ]
      ] ],
      [ "Example", "md_docs_2_file_downloader.html#autotoc_md793", null ]
    ] ],
    [ "FileStream", "md_docs_2_file_stream.html", [
      [ "Syntax", "md_docs_2_file_stream.html#autotoc_md796", null ],
      [ "Constructors", "md_docs_2_file_stream.html#autotoc_md798", [
        [ "<span class=\"tt\">FileStream(const String&amp; sPath, int iMode)</span>", "md_docs_2_file_stream.html#autotoc_md799", null ]
      ] ],
      [ "Member Functions", "md_docs_2_file_stream.html#autotoc_md801", [
        [ "<span class=\"tt\">bool CanRead() const override</span>", "md_docs_2_file_stream.html#autotoc_md802", null ],
        [ "<span class=\"tt\">bool CanSeek() const override</span>", "md_docs_2_file_stream.html#autotoc_md803", null ],
        [ "<span class=\"tt\">bool CanWrite() const override</span>", "md_docs_2_file_stream.html#autotoc_md804", null ],
        [ "<span class=\"tt\">long GetLength() const override</span>", "md_docs_2_file_stream.html#autotoc_md805", null ],
        [ "<span class=\"tt\">long GetPosition() const override</span> / <span class=\"tt\">void SetPosition(long llValue) override</span>", "md_docs_2_file_stream.html#autotoc_md806", null ],
        [ "<span class=\"tt\">int Read(char* pBuffer, int iOffset, int nCount) override</span>", "md_docs_2_file_stream.html#autotoc_md807", null ],
        [ "<span class=\"tt\">void Write(const char* pBuffer, int iOffset, int nCount) override</span>", "md_docs_2_file_stream.html#autotoc_md808", null ],
        [ "<span class=\"tt\">long Seek(long llOffset, int iOrigin) override</span>", "md_docs_2_file_stream.html#autotoc_md809", null ],
        [ "<span class=\"tt\">void Flush() override</span>", "md_docs_2_file_stream.html#autotoc_md810", null ],
        [ "<span class=\"tt\">void Dispose() override</span>", "md_docs_2_file_stream.html#autotoc_md811", null ]
      ] ],
      [ "Example", "md_docs_2_file_stream.html#autotoc_md813", null ]
    ] ],
    [ "Generic Collections (<span class=\"tt\">DotNetDupe::System::Collections::Generic</span>)", "md_docs_2_generic_collections.html", [
      [ "Available Classes", "md_docs_2_generic_collections.html#autotoc_md815", null ],
      [ "Usage Examples", "md_docs_2_generic_collections.html#autotoc_md817", [
        [ "1. <span class=\"tt\">Queue&lt;T&gt;</span> &amp; <span class=\"tt\">Stack&lt;T&gt;</span>", "md_docs_2_generic_collections.html#autotoc_md818", null ],
        [ "2. <span class=\"tt\">HashSet&lt;T&gt;</span> &amp; <span class=\"tt\">SortedSet&lt;T&gt;</span>", "md_docs_2_generic_collections.html#autotoc_md819", null ],
        [ "3. <span class=\"tt\">LinkedList&lt;T&gt;</span> &amp; <span class=\"tt\">PriorityQueue&lt;TElement, TPriority&gt;</span>", "md_docs_2_generic_collections.html#autotoc_md820", null ]
      ] ]
    ] ],
    [ "class <span class=\"tt\">GenericUriParser</span>", "md_docs_2_generic_uri_parser.html", null ],
    [ "Guid", "md_docs_2_guid.html", [
      [ "Syntax", "md_docs_2_guid.html#autotoc_md826", null ],
      [ "Static Fields &amp; Methods", "md_docs_2_guid.html#autotoc_md828", [
        [ "<span class=\"tt\">static const Guid Empty</span>", "md_docs_2_guid.html#autotoc_md829", null ],
        [ "<span class=\"tt\">static Guid NewGuid()</span>", "md_docs_2_guid.html#autotoc_md830", null ]
      ] ],
      [ "Constructors", "md_docs_2_guid.html#autotoc_md832", [
        [ "<span class=\"tt\">Guid()</span>", "md_docs_2_guid.html#autotoc_md833", null ],
        [ "<span class=\"tt\">Guid(const String&amp; g)</span>", "md_docs_2_guid.html#autotoc_md834", null ],
        [ "<span class=\"tt\">Guid(const Array&lt;uint8_t&gt;&amp; b)</span>", "md_docs_2_guid.html#autotoc_md835", null ]
      ] ],
      [ "Member Functions &amp; Operators", "md_docs_2_guid.html#autotoc_md837", [
        [ "<span class=\"tt\">String ToString() const</span>", "md_docs_2_guid.html#autotoc_md838", null ],
        [ "<span class=\"tt\">Array&lt;uint8_t&gt; ToByteArray() const</span>", "md_docs_2_guid.html#autotoc_md839", null ],
        [ "<span class=\"tt\">bool operator==(const Guid&amp; other) const</span>", "md_docs_2_guid.html#autotoc_md840", null ],
        [ "<span class=\"tt\">bool operator!=(const Guid&amp; other) const</span>", "md_docs_2_guid.html#autotoc_md841", null ]
      ] ],
      [ "Example", "md_docs_2_guid.html#autotoc_md843", null ]
    ] ],
    [ "HashSet&lt;T&gt;", "md_docs_2_hash_set.html", [
      [ "Syntax", "md_docs_2_hash_set.html#autotoc_md846", null ],
      [ "Constructors &amp; Assignment", "md_docs_2_hash_set.html#autotoc_md848", [
        [ "<span class=\"tt\">HashSet()</span>", "md_docs_2_hash_set.html#autotoc_md849", null ],
        [ "<span class=\"tt\">HashSet(const HashSet&amp; other)</span> / <span class=\"tt\">HashSet(HashSet&amp;&amp; other) noexcept</span>", "md_docs_2_hash_set.html#autotoc_md850", null ],
        [ "<span class=\"tt\">HashSet&amp; operator=(const HashSet&amp; other)</span> / <span class=\"tt\">operator=(HashSet&amp;&amp; other) noexcept</span>", "md_docs_2_hash_set.html#autotoc_md851", null ]
      ] ],
      [ "Properties", "md_docs_2_hash_set.html#autotoc_md853", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_hash_set.html#autotoc_md854", null ]
      ] ],
      [ "Member Functions", "md_docs_2_hash_set.html#autotoc_md856", [
        [ "<span class=\"tt\">bool Add(const T&amp; item)</span>", "md_docs_2_hash_set.html#autotoc_md857", null ],
        [ "<span class=\"tt\">bool Remove(const T&amp; item)</span>", "md_docs_2_hash_set.html#autotoc_md858", null ],
        [ "<span class=\"tt\">bool Contains(const T&amp; item) const</span>", "md_docs_2_hash_set.html#autotoc_md859", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_hash_set.html#autotoc_md860", null ],
        [ "<span class=\"tt\">void UnionWith(const HashSet&lt;T&gt;&amp; other)</span>", "md_docs_2_hash_set.html#autotoc_md861", null ],
        [ "<span class=\"tt\">void IntersectWith(const HashSet&lt;T&gt;&amp; other)</span>", "md_docs_2_hash_set.html#autotoc_md862", null ],
        [ "<span class=\"tt\">void ExceptWith(const HashSet&lt;T&gt;&amp; other)</span>", "md_docs_2_hash_set.html#autotoc_md863", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_hash_set.html#autotoc_md864", null ]
      ] ],
      [ "Example", "md_docs_2_hash_set.html#autotoc_md866", null ]
    ] ],
    [ "HttpClient, HttpRequestMessage, HttpResponseMessage &amp; HttpContent", "md_docs_2_http_client.html", [
      [ "<span class=\"tt\">HttpMethod</span> Class", "md_docs_2_http_client.html#autotoc_md869", null ],
      [ "<span class=\"tt\">HttpContent</span> (Abstract Base) &amp; Subclasses", "md_docs_2_http_client.html#autotoc_md871", [
        [ "Concrete Implementations", "md_docs_2_http_client.html#autotoc_md872", null ],
        [ "Member Functions", "md_docs_2_http_client.html#autotoc_md873", null ]
      ] ],
      [ "<span class=\"tt\">HttpRequestMessage</span> Class", "md_docs_2_http_client.html#autotoc_md875", [
        [ "Constructors", "md_docs_2_http_client.html#autotoc_md876", null ],
        [ "Properties", "md_docs_2_http_client.html#autotoc_md877", null ]
      ] ],
      [ "<span class=\"tt\">HttpResponseMessage</span> Class", "md_docs_2_http_client.html#autotoc_md879", [
        [ "Properties", "md_docs_2_http_client.html#autotoc_md880", null ]
      ] ],
      [ "<span class=\"tt\">HttpClient</span> Class", "md_docs_2_http_client.html#autotoc_md882", [
        [ "Syntax", "md_docs_2_http_client.html#autotoc_md883", null ],
        [ "Member Functions", "md_docs_2_http_client.html#autotoc_md884", null ]
      ] ],
      [ "Example", "md_docs_2_http_client.html#autotoc_md886", null ]
    ] ],
    [ "HttpContext, HttpRequest &amp; HttpResponse", "md_docs_2_http_context.html", [
      [ "Syntax", "md_docs_2_http_context.html#autotoc_md889", null ],
      [ "HttpRequest Members", "md_docs_2_http_context.html#autotoc_md891", null ],
      [ "HttpResponse Members", "md_docs_2_http_context.html#autotoc_md893", null ],
      [ "HttpContext Members", "md_docs_2_http_context.html#autotoc_md895", null ],
      [ "Example", "md_docs_2_http_context.html#autotoc_md897", null ]
    ] ],
    [ "interface <span class=\"tt\">IDisposable</span>", "md_docs_2_i_disposable.html", null ],
    [ "Core Interfaces", "md_docs_2_interfaces.html", [
      [ "<span class=\"tt\">IDisposable</span>", "md_docs_2_interfaces.html#autotoc_md904", [
        [ "Syntax", "md_docs_2_interfaces.html#autotoc_md905", null ],
        [ "Usage", "md_docs_2_interfaces.html#autotoc_md906", null ]
      ] ],
      [ "<span class=\"tt\">IComparable</span>", "md_docs_2_interfaces.html#autotoc_md908", [
        [ "Syntax", "md_docs_2_interfaces.html#autotoc_md909", null ]
      ] ],
      [ "<span class=\"tt\">IClonable</span>", "md_docs_2_interfaces.html#autotoc_md911", [
        [ "Syntax", "md_docs_2_interfaces.html#autotoc_md912", null ]
      ] ],
      [ "<span class=\"tt\">IFormatProvider&lt;T&gt;</span>", "md_docs_2_interfaces.html#autotoc_md914", [
        [ "Syntax", "md_docs_2_interfaces.html#autotoc_md915", null ]
      ] ],
      [ "<span class=\"tt\">IServiceProvider</span>", "md_docs_2_interfaces.html#autotoc_md917", [
        [ "Syntax", "md_docs_2_interfaces.html#autotoc_md918", null ]
      ] ]
    ] ],
    [ "Interlocked&lt;T&gt;", "md_docs_2_interlocked.html", [
      [ "Syntax", "md_docs_2_interlocked.html#autotoc_md921", null ],
      [ "Constructors", "md_docs_2_interlocked.html#autotoc_md923", [
        [ "<span class=\"tt\">Interlocked(T initialValue = 0)</span>", "md_docs_2_interlocked.html#autotoc_md924", null ]
      ] ],
      [ "Atomic Operations", "md_docs_2_interlocked.html#autotoc_md926", [
        [ "<span class=\"tt\">T Increment()</span>", "md_docs_2_interlocked.html#autotoc_md927", null ],
        [ "<span class=\"tt\">T Decrement()</span>", "md_docs_2_interlocked.html#autotoc_md928", null ],
        [ "<span class=\"tt\">T Add(T value)</span>", "md_docs_2_interlocked.html#autotoc_md929", null ],
        [ "<span class=\"tt\">T Exchange(T value)</span>", "md_docs_2_interlocked.html#autotoc_md930", null ],
        [ "<span class=\"tt\">T CompareExchange(T value, T comparand)</span>", "md_docs_2_interlocked.html#autotoc_md931", null ]
      ] ],
      [ "Operator Overloads", "md_docs_2_interlocked.html#autotoc_md933", null ],
      [ "Example", "md_docs_2_interlocked.html#autotoc_md935", null ]
    ] ],
    [ "JsonSerializer &amp; JsonElement", "md_docs_2_json_serializer.html", [
      [ "<span class=\"tt\">JsonSerializer</span> Class", "md_docs_2_json_serializer.html#autotoc_md938", [
        [ "Syntax", "md_docs_2_json_serializer.html#autotoc_md939", null ]
      ] ],
      [ "Static Methods", "md_docs_2_json_serializer.html#autotoc_md941", [
        [ "<span class=\"tt\">template &lt;typename T&gt; static String Serialize(const T&amp; value)</span>", "md_docs_2_json_serializer.html#autotoc_md942", null ],
        [ "<span class=\"tt\">template &lt;typename T&gt; static T Deserialize(const String&amp; sJson)</span>", "md_docs_2_json_serializer.html#autotoc_md943", null ]
      ] ],
      [ "Custom Struct Serialization (<span class=\"tt\">JSON_SERIALIZABLE</span> Macro)", "md_docs_2_json_serializer.html#autotoc_md945", null ],
      [ "Example", "md_docs_2_json_serializer.html#autotoc_md947", null ]
    ] ],
    [ "JWTToken, HMACSHA256 &amp; X509Certificate2", "md_docs_2_j_w_t_token.html", [
      [ "<span class=\"tt\">HMACSHA256</span> Class", "md_docs_2_j_w_t_token.html#autotoc_md950", [
        [ "Methods", "md_docs_2_j_w_t_token.html#autotoc_md951", null ]
      ] ],
      [ "<span class=\"tt\">X509Certificate2</span> Class", "md_docs_2_j_w_t_token.html#autotoc_md953", [
        [ "Constructors", "md_docs_2_j_w_t_token.html#autotoc_md954", null ],
        [ "Member Functions", "md_docs_2_j_w_t_token.html#autotoc_md955", null ]
      ] ],
      [ "<span class=\"tt\">JWTToken</span> Class", "md_docs_2_j_w_t_token.html#autotoc_md957", [
        [ "Methods", "md_docs_2_j_w_t_token.html#autotoc_md958", null ]
      ] ],
      [ "Example", "md_docs_2_j_w_t_token.html#autotoc_md960", null ]
    ] ],
    [ "LinkedList&lt;T&gt; &amp; LinkedListNode&lt;T&gt;", "md_docs_2_linked_list.html", [
      [ "Syntax", "md_docs_2_linked_list.html#autotoc_md963", null ],
      [ "Constructors", "md_docs_2_linked_list.html#autotoc_md965", [
        [ "<span class=\"tt\">LinkedList()</span>", "md_docs_2_linked_list.html#autotoc_md966", null ]
      ] ],
      [ "Properties", "md_docs_2_linked_list.html#autotoc_md968", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_linked_list.html#autotoc_md969", null ],
        [ "<span class=\"tt\">LinkedListNode&lt;T&gt;* GetFirst() const</span>", "md_docs_2_linked_list.html#autotoc_md970", null ],
        [ "<span class=\"tt\">LinkedListNode&lt;T&gt;* GetLast() const</span>", "md_docs_2_linked_list.html#autotoc_md971", null ]
      ] ],
      [ "Member Functions", "md_docs_2_linked_list.html#autotoc_md973", [
        [ "<span class=\"tt\">LinkedListNode&lt;T&gt;* AddFirst(const T&amp; value)</span>", "md_docs_2_linked_list.html#autotoc_md974", null ],
        [ "<span class=\"tt\">LinkedListNode&lt;T&gt;* AddLast(const T&amp; value)</span>", "md_docs_2_linked_list.html#autotoc_md975", null ],
        [ "<span class=\"tt\">void RemoveFirst()</span>", "md_docs_2_linked_list.html#autotoc_md976", null ],
        [ "<span class=\"tt\">void RemoveLast()</span>", "md_docs_2_linked_list.html#autotoc_md977", null ],
        [ "<span class=\"tt\">bool Remove(const T&amp; value)</span>", "md_docs_2_linked_list.html#autotoc_md978", null ],
        [ "<span class=\"tt\">bool Contains(const T&amp; value) const</span>", "md_docs_2_linked_list.html#autotoc_md979", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_linked_list.html#autotoc_md980", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_linked_list.html#autotoc_md981", null ]
      ] ],
      [ "Example", "md_docs_2_linked_list.html#autotoc_md983", null ]
    ] ],
    [ "List&lt;T&gt;", "md_docs_2_list.html", [
      [ "Syntax", "md_docs_2_list.html#autotoc_md986", null ],
      [ "Constructors &amp; Assignment", "md_docs_2_list.html#autotoc_md988", [
        [ "<span class=\"tt\">List()</span>", "md_docs_2_list.html#autotoc_md989", null ],
        [ "<span class=\"tt\">explicit List(int iCapacity)</span>", "md_docs_2_list.html#autotoc_md990", null ],
        [ "<span class=\"tt\">List(const std::initializer_list&lt;T&gt;&amp; vCollection)</span>", "md_docs_2_list.html#autotoc_md991", null ],
        [ "<span class=\"tt\">List(const List&amp; lstOther)</span> / <span class=\"tt\">List(List&amp;&amp; lstOther) noexcept</span>", "md_docs_2_list.html#autotoc_md992", null ],
        [ "<span class=\"tt\">List&amp; operator=(const List&amp; lstOther)</span> / <span class=\"tt\">operator=(List&amp;&amp; lstOther) noexcept</span>", "md_docs_2_list.html#autotoc_md993", null ]
      ] ],
      [ "Properties", "md_docs_2_list.html#autotoc_md995", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_list.html#autotoc_md996", null ],
        [ "<span class=\"tt\">int GetCapacity() const</span>", "md_docs_2_list.html#autotoc_md997", null ],
        [ "<span class=\"tt\">void SetCapacity(int iValue)</span>", "md_docs_2_list.html#autotoc_md998", null ],
        [ "<span class=\"tt\">T&amp; operator[](int iIndex)</span> / <span class=\"tt\">const T&amp; operator[](int iIndex) const</span>", "md_docs_2_list.html#autotoc_md999", null ]
      ] ],
      [ "Adding &amp; Modifying Elements", "md_docs_2_list.html#autotoc_md1001", [
        [ "<span class=\"tt\">void Add(const T&amp; item)</span>", "md_docs_2_list.html#autotoc_md1002", null ],
        [ "<span class=\"tt\">void AddRange(const Array&lt;T&gt;&amp; arrCollection)</span>", "md_docs_2_list.html#autotoc_md1003", null ],
        [ "<span class=\"tt\">void Insert(int iIndex, const T&amp; item)</span>", "md_docs_2_list.html#autotoc_md1004", null ],
        [ "<span class=\"tt\">bool Remove(const T&amp; item)</span>", "md_docs_2_list.html#autotoc_md1005", null ],
        [ "<span class=\"tt\">void RemoveAt(int iIndex)</span>", "md_docs_2_list.html#autotoc_md1006", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_list.html#autotoc_md1007", null ],
        [ "<span class=\"tt\">void SwapElements(T&amp; a, T&amp; b)</span>", "md_docs_2_list.html#autotoc_md1008", null ]
      ] ],
      [ "Searching &amp; Sorting", "md_docs_2_list.html#autotoc_md1010", [
        [ "<span class=\"tt\">bool Contains(const T&amp; item) const</span>", "md_docs_2_list.html#autotoc_md1011", null ],
        [ "<span class=\"tt\">int IndexOf(const T&amp; item) const</span>", "md_docs_2_list.html#autotoc_md1012", null ],
        [ "<span class=\"tt\">int BinarySearch(const T&amp; item) const</span>", "md_docs_2_list.html#autotoc_md1013", null ],
        [ "<span class=\"tt\">void Sort()</span>", "md_docs_2_list.html#autotoc_md1014", null ]
      ] ],
      [ "Functional &amp; Predicate Queries", "md_docs_2_list.html#autotoc_md1016", [
        [ "<span class=\"tt\">template &lt;typename Predicate&gt; bool Exists(Predicate fnMatch) const</span>", "md_docs_2_list.html#autotoc_md1017", null ],
        [ "<span class=\"tt\">template &lt;typename Predicate&gt; T Find(Predicate fnMatch) const</span>", "md_docs_2_list.html#autotoc_md1018", null ],
        [ "<span class=\"tt\">template &lt;typename Predicate&gt; List&lt;T&gt; FindAll(Predicate fnMatch) const</span>", "md_docs_2_list.html#autotoc_md1019", null ],
        [ "<span class=\"tt\">template &lt;typename Predicate&gt; bool TrueForAll(Predicate fnMatch) const</span>", "md_docs_2_list.html#autotoc_md1020", null ]
      ] ],
      [ "Transformations &amp; Iteration", "md_docs_2_list.html#autotoc_md1022", [
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_list.html#autotoc_md1023", null ],
        [ "<span class=\"tt\">begin()</span> / <span class=\"tt\">end()</span>", "md_docs_2_list.html#autotoc_md1024", null ]
      ] ],
      [ "Example", "md_docs_2_list.html#autotoc_md1026", null ]
    ] ],
    [ "Lock&lt;T&gt;", "md_docs_2_lock.html", [
      [ "Template Specializations", "md_docs_2_lock.html#autotoc_md1028", null ],
      [ "Constructors", "md_docs_2_lock.html#autotoc_md1029", [
        [ "<span class=\"tt\">Lock(T&amp; syncObject, int millisecondsTimeout = -1, int releaseCount = -1)</span>", "md_docs_2_lock.html#autotoc_md1030", null ]
      ] ],
      [ "Destructor", "md_docs_2_lock.html#autotoc_md1031", [
        [ "<span class=\"tt\">~Lock()</span>", "md_docs_2_lock.html#autotoc_md1032", null ]
      ] ],
      [ "Type Aliases", "md_docs_2_lock.html#autotoc_md1033", null ],
      [ "Example Usage", "md_docs_2_lock.html#autotoc_md1034", [
        [ "Using MutexLock", "md_docs_2_lock.html#autotoc_md1035", null ],
        [ "Using SemaphoreLock with Timeout", "md_docs_2_lock.html#autotoc_md1036", null ]
      ] ],
      [ "Code Example", "md_docs_2_lock.html#autotoc_md1037", null ]
    ] ],
    [ "LoggerTextWriter", "md_docs_2_logger_text_writer.html", [
      [ "Syntax", "md_docs_2_logger_text_writer.html#autotoc_md1040", null ],
      [ "Constructors", "md_docs_2_logger_text_writer.html#autotoc_md1042", [
        [ "<span class=\"tt\">LoggerTextWriter(const String&amp; sCategoryName = \"Console\", LogLevel logLevel = LogLevel::Information)</span>", "md_docs_2_logger_text_writer.html#autotoc_md1043", null ],
        [ "<span class=\"tt\">LoggerTextWriter(const SmartPointer&lt;ILogger&gt;&amp; pLogger, LogLevel logLevel = LogLevel::Information)</span>", "md_docs_2_logger_text_writer.html#autotoc_md1044", null ]
      ] ],
      [ "Properties / Methods", "md_docs_2_logger_text_writer.html#autotoc_md1046", [
        [ "<span class=\"tt\">void Write(const String&amp; sValue) override</span>", "md_docs_2_logger_text_writer.html#autotoc_md1047", null ],
        [ "<span class=\"tt\">void WriteLine(const String&amp; sValue) override</span>", "md_docs_2_logger_text_writer.html#autotoc_md1048", null ],
        [ "<span class=\"tt\">void Flush() override</span>", "md_docs_2_logger_text_writer.html#autotoc_md1049", null ]
      ] ],
      [ "Example: Redirecting Console to FileLogger", "md_docs_2_logger_text_writer.html#autotoc_md1051", null ]
    ] ],
    [ "Logging, LogManager &amp; LoggerTextWriter", "md_docs_2_logging.html", [
      [ "<span class=\"tt\">LogLevel</span> Enum", "md_docs_2_logging.html#autotoc_md1054", null ],
      [ "<span class=\"tt\">ILogger</span> Interface", "md_docs_2_logging.html#autotoc_md1056", [
        [ "Syntax", "md_docs_2_logging.html#autotoc_md1057", null ],
        [ "Methods", "md_docs_2_logging.html#autotoc_md1058", null ]
      ] ],
      [ "<span class=\"tt\">LogManager</span> Class", "md_docs_2_logging.html#autotoc_md1060", [
        [ "Static Methods", "md_docs_2_logging.html#autotoc_md1061", null ]
      ] ],
      [ "<span class=\"tt\">LoggerTextWriter</span>", "md_docs_2_logging.html#autotoc_md1063", [
        [ "Constructors", "md_docs_2_logging.html#autotoc_md1064", null ]
      ] ],
      [ "Example", "md_docs_2_logging.html#autotoc_md1066", null ]
    ] ],
    [ "LogManager", "md_docs_2_log_manager.html", [
      [ "Namespace", "md_docs_2_log_manager.html#autotoc_md1068", null ],
      [ "Syntax &amp; Class Definition", "md_docs_2_log_manager.html#autotoc_md1069", null ],
      [ "Methods", "md_docs_2_log_manager.html#autotoc_md1070", null ],
      [ "Code Example", "md_docs_2_log_manager.html#autotoc_md1071", null ]
    ] ],
    [ "ManualResetEvent", "md_docs_2_manual_reset_event.html", [
      [ "Methods", "md_docs_2_manual_reset_event.html#autotoc_md1073", [
        [ "<span class=\"tt\">ManualResetEvent(bool initialState)</span>", "md_docs_2_manual_reset_event.html#autotoc_md1074", null ],
        [ "<span class=\"tt\">bool WaitOne(int millisecondsTimeout)</span>", "md_docs_2_manual_reset_event.html#autotoc_md1075", null ]
      ] ],
      [ "Remarks", "md_docs_2_manual_reset_event.html#autotoc_md1076", null ],
      [ "Code Example", "md_docs_2_manual_reset_event.html#autotoc_md1077", null ]
    ] ],
    [ "Mutex", "md_docs_2_mutex.html", [
      [ "Syntax", "md_docs_2_mutex.html#autotoc_md1080", null ],
      [ "Constructors", "md_docs_2_mutex.html#autotoc_md1082", [
        [ "<span class=\"tt\">Mutex()</span>", "md_docs_2_mutex.html#autotoc_md1083", null ],
        [ "<span class=\"tt\">Mutex(bool bInitiallyOwned)</span>", "md_docs_2_mutex.html#autotoc_md1084", null ],
        [ "<span class=\"tt\">Mutex(const String&amp; sName, bool bInitiallyOwned = false, bool openAlways = true)</span>", "md_docs_2_mutex.html#autotoc_md1085", null ],
        [ "<span class=\"tt\">Mutex(bool bInitiallyOwned, const String&amp; sName, bool openAlways = true)</span>", "md_docs_2_mutex.html#autotoc_md1086", null ],
        [ "<span class=\"tt\">Mutex(bool bInitiallyOwned, const String&amp; sName, bool openAlways, bool&amp; bCreatedNew)</span>", "md_docs_2_mutex.html#autotoc_md1087", null ]
      ] ],
      [ "Member Functions", "md_docs_2_mutex.html#autotoc_md1089", [
        [ "<span class=\"tt\">bool WaitOne() override</span>", "md_docs_2_mutex.html#autotoc_md1090", null ],
        [ "<span class=\"tt\">bool WaitOne(int millisecondsTimeout) override</span>", "md_docs_2_mutex.html#autotoc_md1091", null ],
        [ "<span class=\"tt\">int Release(int releaseCount = 1) override</span>", "md_docs_2_mutex.html#autotoc_md1092", null ]
      ] ],
      [ "Static Methods", "md_docs_2_mutex.html#autotoc_md1094", [
        [ "<span class=\"tt\">static SmartPointer&lt;Mutex&gt; OpenExisting(const String&amp; sName)</span>", "md_docs_2_mutex.html#autotoc_md1095", null ],
        [ "<span class=\"tt\">static bool TryOpenExisting(const String&amp; sName, SmartPointer&lt;Mutex&gt;&amp; pResult)</span>", "md_docs_2_mutex.html#autotoc_md1096", null ]
      ] ],
      [ "Example", "md_docs_2_mutex.html#autotoc_md1098", null ]
    ] ],
    [ "Object", "md_docs_2_object.html", [
      [ "Syntax", "md_docs_2_object.html#autotoc_md1101", null ],
      [ "Member Functions", "md_docs_2_object.html#autotoc_md1103", [
        [ "<span class=\"tt\">virtual ~Object() = default</span>", "md_docs_2_object.html#autotoc_md1104", null ],
        [ "<span class=\"tt\">bool operator==(const Object&amp; obj) const</span>", "md_docs_2_object.html#autotoc_md1106", null ],
        [ "<span class=\"tt\">virtual bool Equals(const Object&amp; obj) const</span>", "md_docs_2_object.html#autotoc_md1108", null ],
        [ "<span class=\"tt\">static bool Equals(const Object&amp; obj1, const Object&amp; obj2)</span>", "md_docs_2_object.html#autotoc_md1110", null ],
        [ "<span class=\"tt\">virtual int GetHashCode() const</span>", "md_docs_2_object.html#autotoc_md1112", null ],
        [ "<span class=\"tt\">char* ToStringA() const</span>", "md_docs_2_object.html#autotoc_md1114", null ],
        [ "<span class=\"tt\">wchar_t* ToStringW() const</span>", "md_docs_2_object.html#autotoc_md1116", null ]
      ] ],
      [ "Low-Level Memory Helpers", "md_docs_2_object.html#autotoc_md1118", [
        [ "<span class=\"tt\">void* AllocateCollectionBuffer(size_t size)</span>", "md_docs_2_object.html#autotoc_md1119", null ],
        [ "<span class=\"tt\">void FreeCollectionBuffer(void* p)</span>", "md_docs_2_object.html#autotoc_md1121", null ]
      ] ],
      [ "Example", "md_docs_2_object.html#autotoc_md1123", null ]
    ] ],
    [ "OperatingSystem", "md_docs_2_operating_system.html", [
      [ "Syntax", "md_docs_2_operating_system.html#autotoc_md1126", null ],
      [ "Constructors", "md_docs_2_operating_system.html#autotoc_md1128", [
        [ "<span class=\"tt\">OperatingSystem(PlatformID platform, const Version&amp; version)</span>", "md_docs_2_operating_system.html#autotoc_md1129", null ]
      ] ],
      [ "Static Methods", "md_docs_2_operating_system.html#autotoc_md1131", [
        [ "<span class=\"tt\">static bool IsWindows()</span>", "md_docs_2_operating_system.html#autotoc_md1132", null ],
        [ "<span class=\"tt\">static bool IsLinux()</span>", "md_docs_2_operating_system.html#autotoc_md1133", null ]
      ] ],
      [ "Properties &amp; Methods", "md_docs_2_operating_system.html#autotoc_md1135", [
        [ "<span class=\"tt\">PlatformID GetPlatform() const</span>", "md_docs_2_operating_system.html#autotoc_md1136", null ],
        [ "<span class=\"tt\">const Version&amp; GetVersion() const</span>", "md_docs_2_operating_system.html#autotoc_md1137", null ],
        [ "<span class=\"tt\">String GetVersionString() const</span>", "md_docs_2_operating_system.html#autotoc_md1138", null ],
        [ "<span class=\"tt\">String GetServicePack() const</span>", "md_docs_2_operating_system.html#autotoc_md1139", null ]
      ] ],
      [ "Example", "md_docs_2_operating_system.html#autotoc_md1141", null ]
    ] ],
    [ "Path", "md_docs_2_path.html", [
      [ "Syntax", "md_docs_2_path.html#autotoc_md1144", null ],
      [ "Static Methods &amp; Path Operators", "md_docs_2_path.html#autotoc_md1146", [
        [ "<span class=\"tt\">static String Combine(const std::initializer_list&lt;String&gt; sPaths)</span>", "md_docs_2_path.html#autotoc_md1147", null ],
        [ "<span class=\"tt\">static String Join(const std::initializer_list&lt;String&gt; sPaths)</span>", "md_docs_2_path.html#autotoc_md1148", null ],
        [ "<span class=\"tt\">static bool TryJoin(const std::initializer_list&lt;String&gt; sPaths, String&amp; sResult)</span>", "md_docs_2_path.html#autotoc_md1149", null ],
        [ "<span class=\"tt\">static String ChangeExtension(const String&amp; sFilePath, const String&amp; sExtension)</span>", "md_docs_2_path.html#autotoc_md1150", null ],
        [ "<span class=\"tt\">static String GetDirectoryName(const String&amp; sFilePath)</span>", "md_docs_2_path.html#autotoc_md1151", null ],
        [ "<span class=\"tt\">static String GetFileName(const String&amp; sFilePath)</span>", "md_docs_2_path.html#autotoc_md1152", null ],
        [ "<span class=\"tt\">static String GetFileNameWithoutExtension(const String&amp; sFilePath)</span>", "md_docs_2_path.html#autotoc_md1153", null ],
        [ "<span class=\"tt\">static String GetExtension(const String&amp; sFilePath)</span>", "md_docs_2_path.html#autotoc_md1154", null ],
        [ "<span class=\"tt\">static bool HasExtension(const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1155", null ],
        [ "<span class=\"tt\">static String GetFullPath(const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1156", null ],
        [ "<span class=\"tt\">static String GetRelativePath(const String&amp; sRelativeTo, const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1157", null ],
        [ "<span class=\"tt\">static String GetPathRoot(const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1158", null ],
        [ "<span class=\"tt\">static bool IsPathRooted(const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1159", null ],
        [ "<span class=\"tt\">static bool IsPathFullyQualified(const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1160", null ],
        [ "<span class=\"tt\">static String GetTempPath()</span>", "md_docs_2_path.html#autotoc_md1161", null ],
        [ "<span class=\"tt\">static String GetTempFileName()</span>", "md_docs_2_path.html#autotoc_md1162", null ],
        [ "<span class=\"tt\">static String GetRandomFileName()</span>", "md_docs_2_path.html#autotoc_md1163", null ],
        [ "<span class=\"tt\">static bool EndsInDirectorySeparator(const String&amp; sFilePath)</span>", "md_docs_2_path.html#autotoc_md1164", null ],
        [ "<span class=\"tt\">static String TrimEndingDirectorySeparator(const String&amp; sPath)</span>", "md_docs_2_path.html#autotoc_md1165", null ],
        [ "<span class=\"tt\">static bool Exists(const String&amp; sFilePath)</span>", "md_docs_2_path.html#autotoc_md1166", null ]
      ] ],
      [ "Static Separator Constants", "md_docs_2_path.html#autotoc_md1168", null ],
      [ "Example", "md_docs_2_path.html#autotoc_md1170", null ]
    ] ],
    [ "PriorityQueue&lt;TElement, TPriority&gt;", "md_docs_2_priority_queue.html", [
      [ "Syntax", "md_docs_2_priority_queue.html#autotoc_md1173", null ],
      [ "Constructors", "md_docs_2_priority_queue.html#autotoc_md1175", [
        [ "<span class=\"tt\">PriorityQueue()</span>", "md_docs_2_priority_queue.html#autotoc_md1176", null ]
      ] ],
      [ "Properties", "md_docs_2_priority_queue.html#autotoc_md1178", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_priority_queue.html#autotoc_md1179", null ]
      ] ],
      [ "Member Functions", "md_docs_2_priority_queue.html#autotoc_md1181", [
        [ "<span class=\"tt\">void Enqueue(const TElement&amp; element, const TPriority&amp; priority)</span>", "md_docs_2_priority_queue.html#autotoc_md1182", null ],
        [ "<span class=\"tt\">TElement Dequeue()</span>", "md_docs_2_priority_queue.html#autotoc_md1183", null ],
        [ "<span class=\"tt\">TElement Peek() const</span>", "md_docs_2_priority_queue.html#autotoc_md1184", null ],
        [ "<span class=\"tt\">bool TryDequeue(TElement&amp; element, TPriority&amp; priority)</span>", "md_docs_2_priority_queue.html#autotoc_md1185", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_priority_queue.html#autotoc_md1186", null ]
      ] ],
      [ "Example", "md_docs_2_priority_queue.html#autotoc_md1188", null ]
    ] ],
    [ "Process &amp; ProcessStartInfo", "md_docs_2_process.html", [
      [ "<span class=\"tt\">ProcessStartInfo</span>", "md_docs_2_process.html#autotoc_md1191", [
        [ "Syntax", "md_docs_2_process.html#autotoc_md1192", null ],
        [ "Constructors", "md_docs_2_process.html#autotoc_md1193", null ],
        [ "Fields", "md_docs_2_process.html#autotoc_md1194", null ]
      ] ],
      [ "<span class=\"tt\">Process</span>", "md_docs_2_process.html#autotoc_md1196", [
        [ "Syntax", "md_docs_2_process.html#autotoc_md1197", null ]
      ] ],
      [ "Member Functions", "md_docs_2_process.html#autotoc_md1199", [
        [ "<span class=\"tt\">ProcessStartInfo GetStartInfo() const</span> / <span class=\"tt\">void SetStartInfo(const ProcessStartInfo&amp; objStartInfo)</span>", "md_docs_2_process.html#autotoc_md1200", null ],
        [ "<span class=\"tt\">bool Start()</span>", "md_docs_2_process.html#autotoc_md1201", null ],
        [ "<span class=\"tt\">void WaitForExit()</span>", "md_docs_2_process.html#autotoc_md1202", null ],
        [ "<span class=\"tt\">bool WaitForExit(int iMilliseconds)</span>", "md_docs_2_process.html#autotoc_md1203", null ],
        [ "<span class=\"tt\">int GetExitCode() const</span>", "md_docs_2_process.html#autotoc_md1204", null ],
        [ "<span class=\"tt\">bool GetHasExited() const</span>", "md_docs_2_process.html#autotoc_md1205", null ],
        [ "<span class=\"tt\">int GetId() const</span>", "md_docs_2_process.html#autotoc_md1206", null ],
        [ "<span class=\"tt\">String GetProcessName() const</span>", "md_docs_2_process.html#autotoc_md1207", null ],
        [ "<span class=\"tt\">void Kill()</span>", "md_docs_2_process.html#autotoc_md1208", null ]
      ] ],
      [ "Static Methods", "md_docs_2_process.html#autotoc_md1210", [
        [ "<span class=\"tt\">static SmartPointer&lt;Process&gt; Start(const String&amp; sFileName)</span>", "md_docs_2_process.html#autotoc_md1211", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;Process&gt; Start(const String&amp; sFileName, const String&amp; sArguments)</span>", "md_docs_2_process.html#autotoc_md1212", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;Process&gt; Start(const ProcessStartInfo&amp; objStartInfo)</span>", "md_docs_2_process.html#autotoc_md1213", null ],
        [ "<span class=\"tt\">static int GetCurrentProcessId()</span>", "md_docs_2_process.html#autotoc_md1214", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;Process&gt; GetCurrentProcess()</span>", "md_docs_2_process.html#autotoc_md1215", null ],
        [ "<span class=\"tt\">static Array&lt;SmartPointer&lt;Process&gt;&gt; GetProcesses()</span>", "md_docs_2_process.html#autotoc_md1216", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;Process&gt; GetProcessById(int iProcessId)</span>", "md_docs_2_process.html#autotoc_md1217", null ],
        [ "<span class=\"tt\">static Array&lt;SmartPointer&lt;Process&gt;&gt; GetProcessesByName(const String&amp; sProcessName)</span>", "md_docs_2_process.html#autotoc_md1218", null ]
      ] ],
      [ "Example", "md_docs_2_process.html#autotoc_md1220", null ]
    ] ],
    [ "Process Management Comparison: STL vs. DotNetDupe", "md_docs_2_process_comparison.html", [
      [ "1. Process Creation and Management", "md_docs_2_process_comparison.html#autotoc_md1223", null ],
      [ "2. Comparison Examples", "md_docs_2_process_comparison.html#autotoc_md1225", [
        [ "Example: Running a command and waiting for it to finish", "md_docs_2_process_comparison.html#autotoc_md1226", [
          [ "Windows (Win32 API)", "md_docs_2_process_comparison.html#autotoc_md1227", null ],
          [ "Linux (POSIX)", "md_docs_2_process_comparison.html#autotoc_md1228", null ],
          [ "DotNetDupe (Cross-Platform)", "md_docs_2_process_comparison.html#autotoc_md1229", null ]
        ] ]
      ] ],
      [ "3. Advanced Process Configuration", "md_docs_2_process_comparison.html#autotoc_md1231", null ],
      [ "Conclusion", "md_docs_2_process_comparison.html#autotoc_md1233", null ]
    ] ],
    [ "ProcessStreamer, ProcessStreamOptions &amp; ProcessEventArgs", "md_docs_2_process_streamer.html", [
      [ "1. Architectural Overview", "md_docs_2_process_streamer.html#autotoc_md1236", null ],
      [ "2. Enums &amp; Configuration", "md_docs_2_process_streamer.html#autotoc_md1238", [
        [ "<span class=\"tt\">ProcessMetricsDetail</span> Enum", "md_docs_2_process_streamer.html#autotoc_md1239", null ],
        [ "<span class=\"tt\">ProcessStreamOptions</span> Struct", "md_docs_2_process_streamer.html#autotoc_md1240", null ]
      ] ],
      [ "3. EventArgs Classes", "md_docs_2_process_streamer.html#autotoc_md1242", [
        [ "<span class=\"tt\">ProcessEventArgs</span>", "md_docs_2_process_streamer.html#autotoc_md1243", null ],
        [ "<span class=\"tt\">ProcessBatchEventArgs</span>", "md_docs_2_process_streamer.html#autotoc_md1244", null ],
        [ "<span class=\"tt\">ProcessStreamErrorEventArgs</span>", "md_docs_2_process_streamer.html#autotoc_md1245", null ]
      ] ],
      [ "4. <span class=\"tt\">ProcessStreamer</span> Class", "md_docs_2_process_streamer.html#autotoc_md1247", [
        [ "Syntax", "md_docs_2_process_streamer.html#autotoc_md1248", null ],
        [ "Multicast Events", "md_docs_2_process_streamer.html#autotoc_md1249", null ],
        [ "Methods", "md_docs_2_process_streamer.html#autotoc_md1250", null ]
      ] ],
      [ "5. Usage Example", "md_docs_2_process_streamer.html#autotoc_md1252", null ]
    ] ],
    [ "Push Notifications (SSE &amp; WebSockets)", "md_docs_2_push_notifications.html", [
      [ "1. Server-Sent Events (SSE)", "md_docs_2_push_notifications.html#autotoc_md1255", [
        [ "SSE Endpoint Implementation", "md_docs_2_push_notifications.html#autotoc_md1256", null ]
      ] ],
      [ "2. WebSockets (<span class=\"tt\">IWebSocketHandler</span>)", "md_docs_2_push_notifications.html#autotoc_md1258", [
        [ "<span class=\"tt\">IWebSocketHandler</span> Interface", "md_docs_2_push_notifications.html#autotoc_md1259", null ],
        [ "Mapping WebSocket Handlers", "md_docs_2_push_notifications.html#autotoc_md1260", null ]
      ] ],
      [ "Example", "md_docs_2_push_notifications.html#autotoc_md1262", null ]
    ] ],
    [ "Queue&lt;T&gt;", "md_docs_2_queue.html", [
      [ "Syntax", "md_docs_2_queue.html#autotoc_md1265", null ],
      [ "Constructors", "md_docs_2_queue.html#autotoc_md1267", [
        [ "<span class=\"tt\">Queue()</span>", "md_docs_2_queue.html#autotoc_md1268", null ]
      ] ],
      [ "Properties", "md_docs_2_queue.html#autotoc_md1270", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_queue.html#autotoc_md1271", null ]
      ] ],
      [ "Member Functions", "md_docs_2_queue.html#autotoc_md1273", [
        [ "<span class=\"tt\">void Enqueue(const T&amp; item)</span>", "md_docs_2_queue.html#autotoc_md1274", null ],
        [ "<span class=\"tt\">T Dequeue()</span>", "md_docs_2_queue.html#autotoc_md1275", null ],
        [ "<span class=\"tt\">T Peek() const</span>", "md_docs_2_queue.html#autotoc_md1276", null ],
        [ "<span class=\"tt\">bool TryDequeue(T&amp; result)</span>", "md_docs_2_queue.html#autotoc_md1277", null ],
        [ "<span class=\"tt\">bool TryPeek(T&amp; result) const</span>", "md_docs_2_queue.html#autotoc_md1278", null ],
        [ "<span class=\"tt\">bool Contains(const T&amp; item) const</span>", "md_docs_2_queue.html#autotoc_md1279", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_queue.html#autotoc_md1280", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_queue.html#autotoc_md1281", null ]
      ] ],
      [ "Example", "md_docs_2_queue.html#autotoc_md1283", null ]
    ] ],
    [ "Random", "md_docs_2_random.html", [
      [ "Syntax", "md_docs_2_random.html#autotoc_md1286", null ],
      [ "Constructors", "md_docs_2_random.html#autotoc_md1288", [
        [ "<span class=\"tt\">Random()</span>", "md_docs_2_random.html#autotoc_md1289", null ],
        [ "<span class=\"tt\">Random(int seed)</span>", "md_docs_2_random.html#autotoc_md1290", null ]
      ] ],
      [ "Member Functions", "md_docs_2_random.html#autotoc_md1292", [
        [ "<span class=\"tt\">virtual int Next()</span>", "md_docs_2_random.html#autotoc_md1293", null ],
        [ "<span class=\"tt\">virtual int Next(int maxValue)</span>", "md_docs_2_random.html#autotoc_md1294", null ],
        [ "<span class=\"tt\">virtual int Next(int minValue, int maxValue)</span>", "md_docs_2_random.html#autotoc_md1295", null ],
        [ "<span class=\"tt\">virtual double NextDouble()</span>", "md_docs_2_random.html#autotoc_md1296", null ],
        [ "<span class=\"tt\">virtual void NextBytes(unsigned char* buffer, int bufferSize)</span>", "md_docs_2_random.html#autotoc_md1297", null ]
      ] ],
      [ "Example", "md_docs_2_random.html#autotoc_md1299", null ]
    ] ],
    [ "class <span class=\"tt\">SystemMetrics</span> &amp; <span class=\"tt\">ActiveUserSession</span>", "md_docs_2_realtime_telemetry.html", [
      [ "Code Example", "md_docs_2_realtime_telemetry.html#autotoc_md1309", null ]
    ] ],
    [ "RestClient&lt;TResource&gt;", "md_docs_2_rest_client.html", [
      [ "Syntax", "md_docs_2_rest_client.html#autotoc_md1312", null ],
      [ "Constructors", "md_docs_2_rest_client.html#autotoc_md1314", [
        [ "<span class=\"tt\">RestClient(const String&amp; sBaseUrl)</span>", "md_docs_2_rest_client.html#autotoc_md1315", null ]
      ] ],
      [ "Authentication &amp; Headers", "md_docs_2_rest_client.html#autotoc_md1317", [
        [ "<span class=\"tt\">void SetBearerToken(const String&amp; sToken)</span>", "md_docs_2_rest_client.html#autotoc_md1318", null ],
        [ "<span class=\"tt\">void SetBasicAuthentication(const String&amp; sUsername, const String&amp; sPassword)</span>", "md_docs_2_rest_client.html#autotoc_md1319", null ],
        [ "<span class=\"tt\">void ClearAuthentication()</span>", "md_docs_2_rest_client.html#autotoc_md1320", null ],
        [ "<span class=\"tt\">void AddDefaultRequestHeader(const String&amp; sName, const String&amp; sValue)</span>", "md_docs_2_rest_client.html#autotoc_md1321", null ],
        [ "<span class=\"tt\">void RemoveDefaultRequestHeader(const String&amp; sName)</span>", "md_docs_2_rest_client.html#autotoc_md1322", null ]
      ] ],
      [ "CRUD Operations", "md_docs_2_rest_client.html#autotoc_md1324", [
        [ "<span class=\"tt\">List&lt;TResource&gt; GetAll()</span>", "md_docs_2_rest_client.html#autotoc_md1325", null ],
        [ "<span class=\"tt\">TResource Get(const String&amp; sId)</span>", "md_docs_2_rest_client.html#autotoc_md1326", null ],
        [ "<span class=\"tt\">String Post(const TResource&amp; resource)</span>", "md_docs_2_rest_client.html#autotoc_md1327", null ],
        [ "<span class=\"tt\">template &lt;typename TResult = TResource&gt; TResult PostAndReturn(const TResource&amp; resource)</span>", "md_docs_2_rest_client.html#autotoc_md1328", null ],
        [ "<span class=\"tt\">String Put(const String&amp; sId, const TResource&amp; resource)</span>", "md_docs_2_rest_client.html#autotoc_md1329", null ],
        [ "<span class=\"tt\">void Delete(const String&amp; sId)</span>", "md_docs_2_rest_client.html#autotoc_md1330", null ]
      ] ],
      [ "Example", "md_docs_2_rest_client.html#autotoc_md1332", null ]
    ] ],
    [ "Semaphore &amp; SemaphoreSlim", "md_docs_2_semaphore.html", [
      [ "<span class=\"tt\">Semaphore</span>", "md_docs_2_semaphore.html#autotoc_md1335", [
        [ "Syntax", "md_docs_2_semaphore.html#autotoc_md1336", null ],
        [ "Constructors", "md_docs_2_semaphore.html#autotoc_md1337", null ],
        [ "Member Functions", "md_docs_2_semaphore.html#autotoc_md1338", null ],
        [ "Static Methods", "md_docs_2_semaphore.html#autotoc_md1339", null ]
      ] ],
      [ "<span class=\"tt\">SemaphoreSlim</span>", "md_docs_2_semaphore.html#autotoc_md1341", [
        [ "Syntax", "md_docs_2_semaphore.html#autotoc_md1342", null ],
        [ "Constructors", "md_docs_2_semaphore.html#autotoc_md1343", null ],
        [ "Member Functions", "md_docs_2_semaphore.html#autotoc_md1344", null ]
      ] ],
      [ "Example", "md_docs_2_semaphore.html#autotoc_md1346", null ]
    ] ],
    [ "SemaphoreSlim", "md_docs_2_semaphore_slim.html", [
      [ "Methods", "md_docs_2_semaphore_slim.html#autotoc_md1348", [
        [ "<span class=\"tt\">SemaphoreSlim(int initialCount)</span>", "md_docs_2_semaphore_slim.html#autotoc_md1349", null ],
        [ "<span class=\"tt\">SemaphoreSlim(int initialCount, int maximumCount)</span>", "md_docs_2_semaphore_slim.html#autotoc_md1350", null ],
        [ "<span class=\"tt\">bool WaitOne()</span>", "md_docs_2_semaphore_slim.html#autotoc_md1351", null ],
        [ "<span class=\"tt\">bool WaitOne(int millisecondsTimeout)</span>", "md_docs_2_semaphore_slim.html#autotoc_md1352", null ],
        [ "<span class=\"tt\">int Release(int releaseCount = 1)</span>", "md_docs_2_semaphore_slim.html#autotoc_md1353", null ],
        [ "<span class=\"tt\">int GetCurrentCount()</span>", "md_docs_2_semaphore_slim.html#autotoc_md1354", null ]
      ] ],
      [ "Code Example", "md_docs_2_semaphore_slim.html#autotoc_md1355", null ]
    ] ],
    [ "SmartPointer&lt;T&gt;", "md_docs_2_smart_pointer.html", [
      [ "Syntax", "md_docs_2_smart_pointer.html#autotoc_md1358", null ],
      [ "Static Factory Methods (Recommended)", "md_docs_2_smart_pointer.html#autotoc_md1360", [
        [ "<span class=\"tt\">static SmartPointer&lt;T&gt; NewUnique()</span> / <span class=\"tt\">template &lt;typename Arg1, typename... Args&gt; static SmartPointer&lt;T&gt; NewUnique(Arg1&amp;&amp; arg1, Args&amp;&amp;... args)</span>", "md_docs_2_smart_pointer.html#autotoc_md1361", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;T&gt; NewShared()</span> / <span class=\"tt\">template &lt;typename Arg1, typename... Args&gt; static SmartPointer&lt;T&gt; NewShared(Arg1&amp;&amp; arg1, Args&amp;&amp;... args)</span>", "md_docs_2_smart_pointer.html#autotoc_md1362", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;T&gt; New()</span> / <span class=\"tt\">template &lt;typename Arg1, typename... Args&gt; static SmartPointer&lt;T&gt; New(Arg1&amp;&amp; arg1, Args&amp;&amp;... args)</span>", "md_docs_2_smart_pointer.html#autotoc_md1363", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;T&gt; MakeUnique()</span> / <span class=\"tt\">template &lt;typename Arg1, typename... Args&gt; static SmartPointer&lt;T&gt; MakeUnique(Arg1&amp;&amp; arg1, Args&amp;&amp;... args)</span>", "md_docs_2_smart_pointer.html#autotoc_md1364", null ],
        [ "<span class=\"tt\">static SmartPointer&lt;T&gt; MakeShared()</span> / <span class=\"tt\">template &lt;typename Arg1, typename... Args&gt; static SmartPointer&lt;T&gt; MakeShared(Arg1&amp;&amp; arg1, Args&amp;&amp;... args)</span>", "md_docs_2_smart_pointer.html#autotoc_md1365", null ]
      ] ],
      [ "Constructors &amp; Assignment", "md_docs_2_smart_pointer.html#autotoc_md1367", [
        [ "<span class=\"tt\">SmartPointer()</span>", "md_docs_2_smart_pointer.html#autotoc_md1368", null ],
        [ "<span class=\"tt\">explicit SmartPointer(bool bIsShared)</span>", "md_docs_2_smart_pointer.html#autotoc_md1369", null ],
        [ "<span class=\"tt\">explicit SmartPointer(T* pPtr)</span>", "md_docs_2_smart_pointer.html#autotoc_md1370", null ],
        [ "<span class=\"tt\">SmartPointer(T* pPtr, bool bIsShared)</span>", "md_docs_2_smart_pointer.html#autotoc_md1371", null ],
        [ "<span class=\"tt\">SmartPointer(std::nullptr_t)</span>", "md_docs_2_smart_pointer.html#autotoc_md1372", null ],
        [ "<span class=\"tt\">SmartPointer(const SmartPointer&lt;T&gt;&amp; objOther)</span> / <span class=\"tt\">template &lt;typename U&gt; SmartPointer(const SmartPointer&lt;U&gt;&amp; objOther)</span>", "md_docs_2_smart_pointer.html#autotoc_md1373", null ],
        [ "<span class=\"tt\">SmartPointer&amp; operator=(const SmartPointer&lt;T&gt;&amp; objOther)</span> / <span class=\"tt\">template &lt;typename U&gt; SmartPointer&amp; operator=(const SmartPointer&lt;U&gt;&amp; objOther)</span>", "md_docs_2_smart_pointer.html#autotoc_md1374", null ],
        [ "<span class=\"tt\">SmartPointer(SmartPointer&lt;T&gt;&amp;&amp; objOther) noexcept</span> / <span class=\"tt\">template &lt;typename U&gt; SmartPointer(SmartPointer&lt;U&gt;&amp;&amp; objOther) noexcept</span>", "md_docs_2_smart_pointer.html#autotoc_md1375", null ],
        [ "<span class=\"tt\">SmartPointer&amp; operator=(SmartPointer&lt;T&gt;&amp;&amp; objOther) noexcept</span> / <span class=\"tt\">template &lt;typename U&gt; SmartPointer&amp; operator=(SmartPointer&lt;U&gt;&amp;&amp; objOther) noexcept</span>", "md_docs_2_smart_pointer.html#autotoc_md1376", null ]
      ] ],
      [ "Member Functions", "md_docs_2_smart_pointer.html#autotoc_md1378", [
        [ "<span class=\"tt\">T* Get() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1379", null ],
        [ "<span class=\"tt\">bool IsNull() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1380", null ],
        [ "<span class=\"tt\">int GetRefCount() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1381", null ],
        [ "<span class=\"tt\">void Reset(T* pPtr = nullptr)</span>", "md_docs_2_smart_pointer.html#autotoc_md1382", null ],
        [ "<span class=\"tt\">void Reset(T* pPtr, bool bIsShared)</span>", "md_docs_2_smart_pointer.html#autotoc_md1383", null ],
        [ "<span class=\"tt\">void Attach(T* pPtr, bool bIsShared = false)</span>", "md_docs_2_smart_pointer.html#autotoc_md1384", null ],
        [ "<span class=\"tt\">T* Detach()</span>", "md_docs_2_smart_pointer.html#autotoc_md1385", null ],
        [ "<span class=\"tt\">template &lt;typename U&gt; SmartPointer&lt;U&gt; DynamicCast() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1386", null ]
      ] ],
      [ "Operators", "md_docs_2_smart_pointer.html#autotoc_md1388", [
        [ "<span class=\"tt\">T* operator-&gt;() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1389", null ],
        [ "<span class=\"tt\">T&amp; operator*() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1390", null ],
        [ "<span class=\"tt\">explicit operator bool() const</span>", "md_docs_2_smart_pointer.html#autotoc_md1391", null ],
        [ "<span class=\"tt\">template &lt;typename U&gt; bool operator==(const SmartPointer&lt;U&gt;&amp; other) const</span>", "md_docs_2_smart_pointer.html#autotoc_md1392", null ],
        [ "<span class=\"tt\">template &lt;typename U&gt; bool operator!=(const SmartPointer&lt;U&gt;&amp; other) const</span>", "md_docs_2_smart_pointer.html#autotoc_md1393", null ]
      ] ],
      [ "Example", "md_docs_2_smart_pointer.html#autotoc_md1395", null ]
    ] ],
    [ "Sockets, NetworkStream, TcpClient, TcpListener &amp; UdpClient", "md_docs_2_sockets.html", [
      [ "Enums", "md_docs_2_sockets.html#autotoc_md1398", null ],
      [ "<span class=\"tt\">Socket</span>", "md_docs_2_sockets.html#autotoc_md1400", [
        [ "Constructors", "md_docs_2_sockets.html#autotoc_md1401", null ],
        [ "Member Functions", "md_docs_2_sockets.html#autotoc_md1402", null ]
      ] ],
      [ "<span class=\"tt\">NetworkStream</span>", "md_docs_2_sockets.html#autotoc_md1404", [
        [ "Constructors", "md_docs_2_sockets.html#autotoc_md1405", null ],
        [ "Member Functions", "md_docs_2_sockets.html#autotoc_md1406", null ]
      ] ],
      [ "<span class=\"tt\">TcpClient</span>", "md_docs_2_sockets.html#autotoc_md1408", [
        [ "Constructors", "md_docs_2_sockets.html#autotoc_md1409", null ],
        [ "Member Functions", "md_docs_2_sockets.html#autotoc_md1410", null ]
      ] ],
      [ "<span class=\"tt\">TcpListener</span>", "md_docs_2_sockets.html#autotoc_md1412", [
        [ "Constructors", "md_docs_2_sockets.html#autotoc_md1413", null ],
        [ "Member Functions", "md_docs_2_sockets.html#autotoc_md1414", null ]
      ] ],
      [ "<span class=\"tt\">UdpClient</span>", "md_docs_2_sockets.html#autotoc_md1416", [
        [ "Constructors", "md_docs_2_sockets.html#autotoc_md1417", null ],
        [ "Member Functions", "md_docs_2_sockets.html#autotoc_md1418", null ]
      ] ],
      [ "Example", "md_docs_2_sockets.html#autotoc_md1420", null ]
    ] ],
    [ "SortedDictionary&lt;TKey, TValue&gt;", "md_docs_2_sorted_dictionary.html", [
      [ "Syntax", "md_docs_2_sorted_dictionary.html#autotoc_md1423", null ],
      [ "Constructors", "md_docs_2_sorted_dictionary.html#autotoc_md1425", [
        [ "<span class=\"tt\">SortedDictionary()</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1426", null ]
      ] ],
      [ "Properties", "md_docs_2_sorted_dictionary.html#autotoc_md1428", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1429", null ],
        [ "<span class=\"tt\">TValue&amp; operator[](const TKey&amp; key)</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1430", null ],
        [ "<span class=\"tt\">const TValue&amp; operator[](const TKey&amp; key) const</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1431", null ]
      ] ],
      [ "Member Functions", "md_docs_2_sorted_dictionary.html#autotoc_md1433", [
        [ "<span class=\"tt\">void Add(const TKey&amp; key, const TValue&amp; value)</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1434", null ],
        [ "<span class=\"tt\">bool Remove(const TKey&amp; key)</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1435", null ],
        [ "<span class=\"tt\">bool ContainsKey(const TKey&amp; key) const</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1436", null ],
        [ "<span class=\"tt\">bool TryGetValue(const TKey&amp; key, TValue&amp; value) const</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1437", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1438", null ],
        [ "<span class=\"tt\">Array&lt;TKey&gt; GetKeys() const</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1439", null ],
        [ "<span class=\"tt\">Array&lt;TValue&gt; GetValues() const</span>", "md_docs_2_sorted_dictionary.html#autotoc_md1440", null ]
      ] ],
      [ "Example", "md_docs_2_sorted_dictionary.html#autotoc_md1442", null ]
    ] ],
    [ "SortedSet&lt;T&gt;", "md_docs_2_sorted_set.html", [
      [ "Syntax", "md_docs_2_sorted_set.html#autotoc_md1445", null ],
      [ "Constructors", "md_docs_2_sorted_set.html#autotoc_md1447", [
        [ "<span class=\"tt\">SortedSet()</span>", "md_docs_2_sorted_set.html#autotoc_md1448", null ]
      ] ],
      [ "Properties", "md_docs_2_sorted_set.html#autotoc_md1450", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_sorted_set.html#autotoc_md1451", null ]
      ] ],
      [ "Member Functions", "md_docs_2_sorted_set.html#autotoc_md1453", [
        [ "<span class=\"tt\">bool Add(const T&amp; item)</span>", "md_docs_2_sorted_set.html#autotoc_md1454", null ],
        [ "<span class=\"tt\">bool Remove(const T&amp; item)</span>", "md_docs_2_sorted_set.html#autotoc_md1455", null ],
        [ "<span class=\"tt\">bool Contains(const T&amp; item) const</span>", "md_docs_2_sorted_set.html#autotoc_md1456", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_sorted_set.html#autotoc_md1457", null ],
        [ "<span class=\"tt\">void UnionWith(const SortedSet&lt;T&gt;&amp; other)</span>", "md_docs_2_sorted_set.html#autotoc_md1458", null ],
        [ "<span class=\"tt\">void IntersectWith(const SortedSet&lt;T&gt;&amp; other)</span>", "md_docs_2_sorted_set.html#autotoc_md1459", null ],
        [ "<span class=\"tt\">void ExceptWith(const SortedSet&lt;T&gt;&amp; other)</span>", "md_docs_2_sorted_set.html#autotoc_md1460", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_sorted_set.html#autotoc_md1461", null ]
      ] ],
      [ "Example", "md_docs_2_sorted_set.html#autotoc_md1463", null ]
    ] ],
    [ "SslStream", "md_docs_2_ssl_stream.html", [
      [ "Syntax", "md_docs_2_ssl_stream.html#autotoc_md1466", null ],
      [ "Constructors", "md_docs_2_ssl_stream.html#autotoc_md1468", [
        [ "<span class=\"tt\">SslStream(const SmartPointer&lt;IO::Stream&gt;&amp; innerStream)</span>", "md_docs_2_ssl_stream.html#autotoc_md1469", null ],
        [ "<span class=\"tt\">SslStream(const SmartPointer&lt;IO::Stream&gt;&amp; innerStream, bool leaveInnerStreamOpen)</span>", "md_docs_2_ssl_stream.html#autotoc_md1470", null ]
      ] ],
      [ "Authentication Methods", "md_docs_2_ssl_stream.html#autotoc_md1472", [
        [ "<span class=\"tt\">void AuthenticateAsClient(const String&amp; targetHost)</span>", "md_docs_2_ssl_stream.html#autotoc_md1473", null ],
        [ "<span class=\"tt\">void AuthenticateAsServer(const SmartPointer&lt;X509Certificate2&gt;&amp; certificate)</span>", "md_docs_2_ssl_stream.html#autotoc_md1474", null ]
      ] ],
      [ "Stream Operations", "md_docs_2_ssl_stream.html#autotoc_md1476", null ],
      [ "Example", "md_docs_2_ssl_stream.html#autotoc_md1478", null ]
    ] ],
    [ "Stack&lt;T&gt;", "md_docs_2_stack.html", [
      [ "Syntax", "md_docs_2_stack.html#autotoc_md1481", null ],
      [ "Constructors", "md_docs_2_stack.html#autotoc_md1483", [
        [ "<span class=\"tt\">Stack()</span>", "md_docs_2_stack.html#autotoc_md1484", null ]
      ] ],
      [ "Properties", "md_docs_2_stack.html#autotoc_md1486", [
        [ "<span class=\"tt\">int GetCount() const</span>", "md_docs_2_stack.html#autotoc_md1487", null ]
      ] ],
      [ "Member Functions", "md_docs_2_stack.html#autotoc_md1489", [
        [ "<span class=\"tt\">void Push(const T&amp; item)</span>", "md_docs_2_stack.html#autotoc_md1490", null ],
        [ "<span class=\"tt\">T Pop()</span>", "md_docs_2_stack.html#autotoc_md1491", null ],
        [ "<span class=\"tt\">T Peek() const</span>", "md_docs_2_stack.html#autotoc_md1492", null ],
        [ "<span class=\"tt\">bool TryPop(T&amp; result)</span>", "md_docs_2_stack.html#autotoc_md1493", null ],
        [ "<span class=\"tt\">bool TryPeek(T&amp; result) const</span>", "md_docs_2_stack.html#autotoc_md1494", null ],
        [ "<span class=\"tt\">bool Contains(const T&amp; item) const</span>", "md_docs_2_stack.html#autotoc_md1495", null ],
        [ "<span class=\"tt\">void Clear()</span>", "md_docs_2_stack.html#autotoc_md1496", null ],
        [ "<span class=\"tt\">Array&lt;T&gt; ToArray() const</span>", "md_docs_2_stack.html#autotoc_md1497", null ]
      ] ],
      [ "Example", "md_docs_2_stack.html#autotoc_md1499", null ]
    ] ],
    [ "Stopwatch", "md_docs_2_stopwatch.html", [
      [ "Syntax", "md_docs_2_stopwatch.html#autotoc_md1502", null ],
      [ "Static Fields &amp; Constants", "md_docs_2_stopwatch.html#autotoc_md1504", null ],
      [ "Constructors", "md_docs_2_stopwatch.html#autotoc_md1506", [
        [ "<span class=\"tt\">Stopwatch()</span>", "md_docs_2_stopwatch.html#autotoc_md1507", null ],
        [ "<span class=\"tt\">Stopwatch(const TimeProviderPtr&amp; timeProvider)</span>", "md_docs_2_stopwatch.html#autotoc_md1508", null ]
      ] ],
      [ "Member Functions", "md_docs_2_stopwatch.html#autotoc_md1510", [
        [ "<span class=\"tt\">void Start()</span>", "md_docs_2_stopwatch.html#autotoc_md1511", null ],
        [ "<span class=\"tt\">void Stop()</span>", "md_docs_2_stopwatch.html#autotoc_md1512", null ],
        [ "<span class=\"tt\">void Reset()</span>", "md_docs_2_stopwatch.html#autotoc_md1513", null ],
        [ "<span class=\"tt\">void Restart()</span>", "md_docs_2_stopwatch.html#autotoc_md1514", null ],
        [ "<span class=\"tt\">bool IsRunning() const</span>", "md_docs_2_stopwatch.html#autotoc_md1515", null ],
        [ "<span class=\"tt\">TimeSpan Elapsed() const</span>", "md_docs_2_stopwatch.html#autotoc_md1516", null ],
        [ "<span class=\"tt\">long long ElapsedMilliseconds() const</span>", "md_docs_2_stopwatch.html#autotoc_md1517", null ],
        [ "<span class=\"tt\">long long ElapsedTicks() const</span>", "md_docs_2_stopwatch.html#autotoc_md1518", null ]
      ] ],
      [ "Static Methods", "md_docs_2_stopwatch.html#autotoc_md1520", [
        [ "<span class=\"tt\">static Stopwatch StartNew()</span>", "md_docs_2_stopwatch.html#autotoc_md1521", null ],
        [ "<span class=\"tt\">static Stopwatch StartNew(const TimeProviderPtr&amp; timeProvider)</span>", "md_docs_2_stopwatch.html#autotoc_md1522", null ],
        [ "<span class=\"tt\">static long long GetTimestamp()</span>", "md_docs_2_stopwatch.html#autotoc_md1523", null ]
      ] ],
      [ "Example", "md_docs_2_stopwatch.html#autotoc_md1525", null ]
    ] ],
    [ "Stream &amp; MemoryStream", "md_docs_2_stream.html", [
      [ "<span class=\"tt\">Stream</span> (Abstract Base Class)", "md_docs_2_stream.html#autotoc_md1528", [
        [ "Syntax", "md_docs_2_stream.html#autotoc_md1529", null ],
        [ "Pure Virtual Members", "md_docs_2_stream.html#autotoc_md1530", null ]
      ] ],
      [ "<span class=\"tt\">MemoryStream</span>", "md_docs_2_stream.html#autotoc_md1532", [
        [ "Syntax", "md_docs_2_stream.html#autotoc_md1533", null ],
        [ "Constructors", "md_docs_2_stream.html#autotoc_md1534", null ],
        [ "Additional Methods", "md_docs_2_stream.html#autotoc_md1535", null ]
      ] ],
      [ "Example", "md_docs_2_stream.html#autotoc_md1537", null ]
    ] ],
    [ "String", "md_docs_2_string.html", [
      [ "Syntax", "md_docs_2_string.html#autotoc_md1540", null ],
      [ "Constructors &amp; Assignment", "md_docs_2_string.html#autotoc_md1542", [
        [ "<span class=\"tt\">String()</span>", "md_docs_2_string.html#autotoc_md1543", null ],
        [ "<span class=\"tt\">String(const char* pStr)</span>", "md_docs_2_string.html#autotoc_md1544", null ],
        [ "<span class=\"tt\">String(const wchar_t* pWStr)</span>", "md_docs_2_string.html#autotoc_md1545", null ],
        [ "<span class=\"tt\">String(std::nullptr_t)</span> / <span class=\"tt\">String(int nNull)</span>", "md_docs_2_string.html#autotoc_md1546", null ],
        [ "<span class=\"tt\">String(const String&amp; sStr)</span> / <span class=\"tt\">String(String&amp;&amp; sStr) noexcept</span>", "md_docs_2_string.html#autotoc_md1547", null ],
        [ "<span class=\"tt\">String&amp; operator=(const String&amp; sStr)</span> / <span class=\"tt\">operator=(const char* pStr)</span> / <span class=\"tt\">operator=(const wchar_t* pStr)</span> / <span class=\"tt\">operator=(String&amp;&amp; sStr) noexcept</span>", "md_docs_2_string.html#autotoc_md1548", null ]
      ] ],
      [ "Properties &amp; Basic Operations", "md_docs_2_string.html#autotoc_md1550", [
        [ "<span class=\"tt\">int GetLength() const</span>", "md_docs_2_string.html#autotoc_md1551", null ],
        [ "<span class=\"tt\">bool IsEmpty() const</span>", "md_docs_2_string.html#autotoc_md1552", null ],
        [ "<span class=\"tt\">const char* GetRawString() const</span>", "md_docs_2_string.html#autotoc_md1553", null ],
        [ "<span class=\"tt\">char operator[](int iIndex) const</span>", "md_docs_2_string.html#autotoc_md1554", null ],
        [ "<span class=\"tt\">int GetHashCode() const</span>", "md_docs_2_string.html#autotoc_md1555", null ],
        [ "<span class=\"tt\">String Clone() const</span>", "md_docs_2_string.html#autotoc_md1556", null ]
      ] ],
      [ "Comparison &amp; Ordering Operators", "md_docs_2_string.html#autotoc_md1558", [
        [ "<span class=\"tt\">static int Compare(const String&amp; sStr1, int iIndex1, const String&amp; sStr2, int iIndex2, int iLength, bool bIgnoreCase)</span>", "md_docs_2_string.html#autotoc_md1559", null ],
        [ "<span class=\"tt\">int CompareTo(const String&amp; sStr) const</span>", "md_docs_2_string.html#autotoc_md1560", null ],
        [ "<span class=\"tt\">static bool Equals(const String&amp; sStr1, const String&amp; sStr2)</span> / <span class=\"tt\">bool Equals(const String&amp; sStr) const</span>", "md_docs_2_string.html#autotoc_md1561", null ],
        [ "<span class=\"tt\">bool operator==(const String&amp; sStr1, const String&amp; sStr2)</span> / <span class=\"tt\">operator==(const String&amp;, const char*)</span> / <span class=\"tt\">operator==(const char*, const String&amp;)</span>", "md_docs_2_string.html#autotoc_md1562", null ],
        [ "<span class=\"tt\">bool operator!=(const String&amp; sStr1, const String&amp; sStr2)</span> / <span class=\"tt\">operator!=(const String&amp;, const char*)</span> / <span class=\"tt\">operator!=(const char*, const String&amp;)</span>", "md_docs_2_string.html#autotoc_md1563", null ],
        [ "<span class=\"tt\">bool operator&lt;(const String&amp; sStr) const</span>", "md_docs_2_string.html#autotoc_md1564", null ]
      ] ],
      [ "Searching &amp; Indexing", "md_docs_2_string.html#autotoc_md1566", [
        [ "<span class=\"tt\">bool Contains(char ch) const</span> / <span class=\"tt\">bool Contains(const String&amp; sStr) const</span>", "md_docs_2_string.html#autotoc_md1567", null ],
        [ "<span class=\"tt\">int IndexOf(const String&amp; sSubstring) const</span> / <span class=\"tt\">IndexOf(const String&amp; sSubstring, bool bIgnoreCase) const</span> / <span class=\"tt\">IndexOf(const String&amp; sSubstring, int iStartIndex, bool bIgnoreCase) const</span>", "md_docs_2_string.html#autotoc_md1568", null ],
        [ "<span class=\"tt\">int IndexOfAny(int iStartIndex, std::initializer_list&lt;char&gt; chChars)</span>", "md_docs_2_string.html#autotoc_md1569", null ],
        [ "<span class=\"tt\">int LastIndexOf(const String&amp; sStr, bool bIgnoreCase)</span>", "md_docs_2_string.html#autotoc_md1570", null ],
        [ "<span class=\"tt\">int LastIndexOfAny(int iStartIndex, std::initializer_list&lt;char&gt; chChars, bool bIgnoreCase)</span>", "md_docs_2_string.html#autotoc_md1571", null ],
        [ "<span class=\"tt\">bool StartsWith(const String&amp; sPrefix) const</span> / <span class=\"tt\">StartsWith(const String&amp; sPrefix, bool bIgnoreCase) const</span>", "md_docs_2_string.html#autotoc_md1572", null ],
        [ "<span class=\"tt\">bool EndsWith(char ch, bool bIgnoreCase) const</span> / <span class=\"tt\">EndsWith(const String&amp; sSuffix, bool bIgnoreCase) const</span>", "md_docs_2_string.html#autotoc_md1573", null ]
      ] ],
      [ "Formatting, Splitting &amp; Joining", "md_docs_2_string.html#autotoc_md1575", [
        [ "<span class=\"tt\">template &lt;typename... Args&gt; static String Format(const char* pFormat, const Args&amp;... args)</span> / <span class=\"tt\">Format(const String&amp; sFormat, ...)</span>", "md_docs_2_string.html#autotoc_md1576", null ],
        [ "<span class=\"tt\">static String InternalFormat(const char* pFormat, const String* pArgs, int iArgCount)</span>", "md_docs_2_string.html#autotoc_md1577", null ],
        [ "<span class=\"tt\">Array&lt;String&gt; Split(char chSeparator) const</span>", "md_docs_2_string.html#autotoc_md1578", null ],
        [ "<span class=\"tt\">Array&lt;String&gt; Split(const Array&lt;String&gt;&amp; arrSeparators, StringSplitOptions eOptions = StringSplitOptions::None) const</span>", "md_docs_2_string.html#autotoc_md1579", null ],
        [ "<span class=\"tt\">Array&lt;String&gt; Split(const String* pSeparator, int iCount, StringSplitOptions eOptions) const</span>", "md_docs_2_string.html#autotoc_md1580", null ],
        [ "<span class=\"tt\">static String Join(char chSeparator, std::initializer_list&lt;String&gt; sStrings)</span>", "md_docs_2_string.html#autotoc_md1581", null ],
        [ "<span class=\"tt\">static String Join(char chSeparator, std::initializer_list&lt;String&gt; sStrings, int iStartIndex, int iCount)</span>", "md_docs_2_string.html#autotoc_md1582", null ],
        [ "<span class=\"tt\">static String Join(const String&amp; sSeparator, std::initializer_list&lt;String&gt; sStrings)</span>", "md_docs_2_string.html#autotoc_md1583", null ],
        [ "<span class=\"tt\">static String Join(const String&amp; sSeparator, std::initializer_list&lt;String&gt; sStrings, int iStartIndex, int iCount)</span>", "md_docs_2_string.html#autotoc_md1584", null ]
      ] ],
      [ "String Manipulation &amp; Mutation", "md_docs_2_string.html#autotoc_md1586", [
        [ "<span class=\"tt\">String&amp; Append(char ch)</span> / <span class=\"tt\">String&amp; Append(const String&amp; sStr)</span>", "md_docs_2_string.html#autotoc_md1587", null ],
        [ "<span class=\"tt\">String Concat(const std::initializer_list&lt;String&gt; sStrs) const</span>", "md_docs_2_string.html#autotoc_md1588", null ],
        [ "<span class=\"tt\">String&amp; Insert(int iIndex, const String&amp; sStr)</span>", "md_docs_2_string.html#autotoc_md1589", null ],
        [ "<span class=\"tt\">String Remove(int iStartIndex) const</span> / <span class=\"tt\">String Remove(int iStartIndex, int iCount) const</span>", "md_docs_2_string.html#autotoc_md1590", null ],
        [ "<span class=\"tt\">String Replace(char chOriginalChar, char chReplaceChar) const</span>", "md_docs_2_string.html#autotoc_md1591", null ],
        [ "<span class=\"tt\">String Replace(const String&amp; sOriginalStr, const String&amp; sReplaceStr) const</span>", "md_docs_2_string.html#autotoc_md1592", null ],
        [ "<span class=\"tt\">String Substring(int iStartIndex) const</span> / <span class=\"tt\">String Substring(int iStartIndex, int iLength) const</span>", "md_docs_2_string.html#autotoc_md1593", null ],
        [ "<span class=\"tt\">String ToLower() const</span> / <span class=\"tt\">String ToUpper() const</span>", "md_docs_2_string.html#autotoc_md1594", null ],
        [ "<span class=\"tt\">String Trim() const</span> / <span class=\"tt\">String TrimStart() const</span> / <span class=\"tt\">String TrimEnd() const</span>", "md_docs_2_string.html#autotoc_md1595", null ],
        [ "<span class=\"tt\">String PadLeft(int iTotalWidth)</span> / <span class=\"tt\">String PadLeft(int iTotalWidth, char ch)</span>", "md_docs_2_string.html#autotoc_md1596", null ],
        [ "<span class=\"tt\">String PadRight(int iTotalWidth)</span> / <span class=\"tt\">String PadRight(int iTotalWidth, char ch)</span>", "md_docs_2_string.html#autotoc_md1597", null ],
        [ "<span class=\"tt\">void CopyTo(int iSourceIndex, char* pDestination, int iDestinationIndex, int iDestArraySize, int iCount) const</span>", "md_docs_2_string.html#autotoc_md1598", null ]
      ] ],
      [ "Static Conversions", "md_docs_2_string.html#autotoc_md1600", null ],
      [ "Example", "md_docs_2_string.html#autotoc_md1602", null ]
    ] ],
    [ "StringBuilder", "md_docs_2_string_builder.html", [
      [ "Syntax", "md_docs_2_string_builder.html#autotoc_md1605", null ],
      [ "Constructors", "md_docs_2_string_builder.html#autotoc_md1607", [
        [ "<span class=\"tt\">StringBuilder()</span>", "md_docs_2_string_builder.html#autotoc_md1608", null ],
        [ "<span class=\"tt\">StringBuilder(int capacity)</span>", "md_docs_2_string_builder.html#autotoc_md1609", null ],
        [ "<span class=\"tt\">StringBuilder(const String&amp; value)</span>", "md_docs_2_string_builder.html#autotoc_md1610", null ]
      ] ],
      [ "Properties", "md_docs_2_string_builder.html#autotoc_md1612", null ],
      [ "Member Functions", "md_docs_2_string_builder.html#autotoc_md1614", [
        [ "<span class=\"tt\">StringBuilder&amp; Append(const String&amp; / const char* / char / int / long long / double / bool value)</span>", "md_docs_2_string_builder.html#autotoc_md1615", null ],
        [ "<span class=\"tt\">StringBuilder&amp; AppendLine()</span>", "md_docs_2_string_builder.html#autotoc_md1616", null ],
        [ "<span class=\"tt\">StringBuilder&amp; AppendLine(const String&amp; value)</span>", "md_docs_2_string_builder.html#autotoc_md1617", null ],
        [ "<span class=\"tt\">StringBuilder&amp; Clear()</span>", "md_docs_2_string_builder.html#autotoc_md1618", null ],
        [ "<span class=\"tt\">String ToString() const</span>", "md_docs_2_string_builder.html#autotoc_md1619", null ]
      ] ],
      [ "Example", "md_docs_2_string_builder.html#autotoc_md1621", null ]
    ] ],
    [ "StringConvert", "md_docs_2_string_convert.html", [
      [ "Syntax", "md_docs_2_string_convert.html#autotoc_md1624", null ],
      [ "Static Methods", "md_docs_2_string_convert.html#autotoc_md1626", [
        [ "<span class=\"tt\">static std::string WCharToUtf8(const wchar_t* pWStr)</span>", "md_docs_2_string_convert.html#autotoc_md1627", null ],
        [ "<span class=\"tt\">static std::wstring Utf8ToWChar(const char* pUtf8Str)</span>", "md_docs_2_string_convert.html#autotoc_md1628", null ]
      ] ],
      [ "Example", "md_docs_2_string_convert.html#autotoc_md1630", null ]
    ] ],
    [ "class <span class=\"tt\">StringReader</span>", "md_docs_2_string_reader.html", null ],
    [ "class <span class=\"tt\">StringWriter</span>", "md_docs_2_string_writer.html", null ],
    [ "SystemMetrics", "md_docs_2_system_metrics.html", [
      [ "Enums &amp; Data Structures", "md_docs_2_system_metrics.html#autotoc_md1644", [
        [ "<span class=\"tt\">MemoryInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1645", null ],
        [ "<span class=\"tt\">DiskInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1646", null ],
        [ "<span class=\"tt\">NetworkUsageInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1647", null ],
        [ "<span class=\"tt\">NetworkConnectionInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1648", null ],
        [ "<span class=\"tt\">ProcessNetworkConnectionInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1649", null ],
        [ "<span class=\"tt\">ProcessInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1650", null ],
        [ "<span class=\"tt\">ServiceInfo</span> Struct", "md_docs_2_system_metrics.html#autotoc_md1651", null ]
      ] ],
      [ "Static Telemetry APIs", "md_docs_2_system_metrics.html#autotoc_md1653", [
        [ "System-Wide Metrics", "md_docs_2_system_metrics.html#autotoc_md1654", null ],
        [ "Process-Specific Metrics", "md_docs_2_system_metrics.html#autotoc_md1655", null ],
        [ "Services &amp; Streaming", "md_docs_2_system_metrics.html#autotoc_md1656", null ]
      ] ],
      [ "Progressive Process Streaming (<span class=\"tt\">ProcessStreamer</span>)", "md_docs_2_system_metrics.html#autotoc_md1658", null ],
      [ "Example", "md_docs_2_system_metrics.html#autotoc_md1660", null ]
    ] ],
    [ "Task &amp; TaskStatus", "md_docs_2_task.html", [
      [ "<span class=\"tt\">TaskStatus</span> Enum", "md_docs_2_task.html#autotoc_md1663", null ],
      [ "<span class=\"tt\">Task</span> Class", "md_docs_2_task.html#autotoc_md1665", [
        [ "Syntax", "md_docs_2_task.html#autotoc_md1666", null ]
      ] ],
      [ "Constructors", "md_docs_2_task.html#autotoc_md1668", [
        [ "<span class=\"tt\">Task(Action&lt;&gt; objAction)</span>", "md_docs_2_task.html#autotoc_md1669", null ]
      ] ],
      [ "Member Functions", "md_docs_2_task.html#autotoc_md1671", [
        [ "<span class=\"tt\">void Start()</span>", "md_docs_2_task.html#autotoc_md1672", null ],
        [ "<span class=\"tt\">void Wait()</span>", "md_docs_2_task.html#autotoc_md1673", null ],
        [ "<span class=\"tt\">bool Wait(int iMillisecondsTimeout)</span>", "md_docs_2_task.html#autotoc_md1674", null ],
        [ "<span class=\"tt\">TaskStatus GetStatus() const</span>", "md_docs_2_task.html#autotoc_md1675", null ],
        [ "<span class=\"tt\">bool GetIsCompleted() const</span>", "md_docs_2_task.html#autotoc_md1676", null ],
        [ "<span class=\"tt\">bool GetIsFaulted() const</span>", "md_docs_2_task.html#autotoc_md1677", null ],
        [ "<span class=\"tt\">bool GetIsCanceled() const</span>", "md_docs_2_task.html#autotoc_md1678", null ]
      ] ],
      [ "Static Methods", "md_docs_2_task.html#autotoc_md1680", [
        [ "<span class=\"tt\">static SmartPointer&lt;Task&gt; Run(Action&lt;&gt; objAction)</span>", "md_docs_2_task.html#autotoc_md1681", null ]
      ] ],
      [ "Example", "md_docs_2_task.html#autotoc_md1683", null ]
    ] ],
    [ "TerminalSession, RdpSessionInfo &amp; RdpSessionState", "md_docs_2_terminal_session.html", [
      [ "<span class=\"tt\">RdpSessionState</span> Enum", "md_docs_2_terminal_session.html#autotoc_md1686", null ],
      [ "<span class=\"tt\">RdpSessionInfo</span> Struct", "md_docs_2_terminal_session.html#autotoc_md1688", null ],
      [ "<span class=\"tt\">TerminalSession</span> Class", "md_docs_2_terminal_session.html#autotoc_md1690", [
        [ "Syntax", "md_docs_2_terminal_session.html#autotoc_md1691", null ]
      ] ],
      [ "Static Methods", "md_docs_2_terminal_session.html#autotoc_md1693", [
        [ "<span class=\"tt\">static List&lt;RdpSessionInfo&gt; GetSessions()</span>", "md_docs_2_terminal_session.html#autotoc_md1694", null ],
        [ "<span class=\"tt\">static List&lt;RdpSessionInfo&gt; GetActiveSessions()</span>", "md_docs_2_terminal_session.html#autotoc_md1695", null ],
        [ "<span class=\"tt\">static List&lt;RdpSessionInfo&gt; GetDisconnectedSessions()</span>", "md_docs_2_terminal_session.html#autotoc_md1696", null ]
      ] ],
      [ "Example", "md_docs_2_terminal_session.html#autotoc_md1698", null ]
    ] ],
    [ "TextEncoding &amp; Encoding", "md_docs_2_text_encoding.html", [
      [ "<span class=\"tt\">Encoding</span> (Abstract Base Class)", "md_docs_2_text_encoding.html#autotoc_md1701", [
        [ "Syntax", "md_docs_2_text_encoding.html#autotoc_md1702", null ],
        [ "Pure Virtual Members", "md_docs_2_text_encoding.html#autotoc_md1703", null ]
      ] ],
      [ "<span class=\"tt\">UTF8Encoding</span> &amp; <span class=\"tt\">TextEncoding</span>", "md_docs_2_text_encoding.html#autotoc_md1705", [
        [ "Static Factory", "md_docs_2_text_encoding.html#autotoc_md1706", null ]
      ] ],
      [ "Example", "md_docs_2_text_encoding.html#autotoc_md1708", null ]
    ] ],
    [ "TextReader &amp; StringReader", "md_docs_2_text_reader.html", [
      [ "<span class=\"tt\">TextReader</span> (Abstract Base Class)", "md_docs_2_text_reader.html#autotoc_md1711", [
        [ "Syntax", "md_docs_2_text_reader.html#autotoc_md1712", null ],
        [ "Pure Virtual Members", "md_docs_2_text_reader.html#autotoc_md1713", null ]
      ] ],
      [ "<span class=\"tt\">StringReader</span>", "md_docs_2_text_reader.html#autotoc_md1715", [
        [ "Syntax", "md_docs_2_text_reader.html#autotoc_md1716", null ],
        [ "Constructors", "md_docs_2_text_reader.html#autotoc_md1717", null ]
      ] ],
      [ "Example", "md_docs_2_text_reader.html#autotoc_md1719", null ]
    ] ],
    [ "TextWriter &amp; StringWriter", "md_docs_2_text_writer.html", [
      [ "<span class=\"tt\">TextWriter</span> (Abstract Base Class)", "md_docs_2_text_writer.html#autotoc_md1722", [
        [ "Syntax", "md_docs_2_text_writer.html#autotoc_md1723", null ],
        [ "Pure Virtual Members", "md_docs_2_text_writer.html#autotoc_md1724", null ],
        [ "Protected Static Methods", "md_docs_2_text_writer.html#autotoc_md1725", null ]
      ] ],
      [ "<span class=\"tt\">StringWriter</span>", "md_docs_2_text_writer.html#autotoc_md1727", [
        [ "Syntax", "md_docs_2_text_writer.html#autotoc_md1728", null ],
        [ "Constructors", "md_docs_2_text_writer.html#autotoc_md1729", null ],
        [ "Additional Methods", "md_docs_2_text_writer.html#autotoc_md1730", null ]
      ] ],
      [ "Example", "md_docs_2_text_writer.html#autotoc_md1732", null ]
    ] ],
    [ "Thread", "md_docs_2_thread.html", [
      [ "Syntax", "md_docs_2_thread.html#autotoc_md1735", null ],
      [ "Constructors", "md_docs_2_thread.html#autotoc_md1737", [
        [ "<span class=\"tt\">Thread(ThreadStart start)</span>", "md_docs_2_thread.html#autotoc_md1738", null ],
        [ "<span class=\"tt\">Thread(ParameterizedThreadStart start)</span>", "md_docs_2_thread.html#autotoc_md1739", null ]
      ] ],
      [ "Member Functions", "md_docs_2_thread.html#autotoc_md1741", [
        [ "<span class=\"tt\">void Start()</span>", "md_docs_2_thread.html#autotoc_md1742", null ],
        [ "<span class=\"tt\">void Start(Object* parameter)</span>", "md_docs_2_thread.html#autotoc_md1743", null ],
        [ "<span class=\"tt\">void Join()</span>", "md_docs_2_thread.html#autotoc_md1744", null ],
        [ "<span class=\"tt\">bool Join(int millisecondsTimeout)</span>", "md_docs_2_thread.html#autotoc_md1745", null ],
        [ "<span class=\"tt\">bool IsAlive() const</span>", "md_docs_2_thread.html#autotoc_md1746", null ],
        [ "<span class=\"tt\">String GetName() const</span> / <span class=\"tt\">void SetName(const String&amp; name)</span>", "md_docs_2_thread.html#autotoc_md1747", null ]
      ] ],
      [ "Static Methods", "md_docs_2_thread.html#autotoc_md1749", [
        [ "<span class=\"tt\">static void Sleep(int millisecondsTimeout)</span>", "md_docs_2_thread.html#autotoc_md1750", null ],
        [ "<span class=\"tt\">static Thread* GetCurrentThread()</span>", "md_docs_2_thread.html#autotoc_md1751", null ],
        [ "<span class=\"tt\">static int GetCurrentThreadId()</span>", "md_docs_2_thread.html#autotoc_md1752", null ]
      ] ],
      [ "Example", "md_docs_2_thread.html#autotoc_md1754", null ]
    ] ],
    [ "Threading Comparison: STL vs. DotNetDupe", "md_docs_2_threading_comparison.html", [
      [ "1. Thread Management", "md_docs_2_threading_comparison.html#autotoc_md1757", [
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1758", null ]
      ] ],
      [ "2. Mutex &amp; Exclusive Locking", "md_docs_2_threading_comparison.html#autotoc_md1760", null ],
      [ "7. RAII Scoped Locking", "md_docs_2_threading_comparison.html#autotoc_md1762", [
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1763", null ],
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1764", null ]
      ] ],
      [ "3. Critical Section (User Mode)", "md_docs_2_threading_comparison.html#autotoc_md1766", [
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1767", null ]
      ] ],
      [ "4. Signaling Events (Manual &amp; Auto Reset)", "md_docs_2_threading_comparison.html#autotoc_md1769", [
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1770", null ]
      ] ],
      [ "5. Semaphores", "md_docs_2_threading_comparison.html#autotoc_md1772", [
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1773", null ]
      ] ],
      [ "6. Atomic Operations (Interlocked)", "md_docs_2_threading_comparison.html#autotoc_md1775", [
        [ "Example", "md_docs_2_threading_comparison.html#autotoc_md1776", null ]
      ] ]
    ] ],
    [ "ThreadPool", "md_docs_2_thread_pool.html", [
      [ "Syntax", "md_docs_2_thread_pool.html#autotoc_md1779", null ],
      [ "Static Methods", "md_docs_2_thread_pool.html#autotoc_md1781", [
        [ "<span class=\"tt\">static bool QueueUserWorkItem(WaitCallback callback)</span>", "md_docs_2_thread_pool.html#autotoc_md1782", null ],
        [ "<span class=\"tt\">static bool QueueUserWorkItem(WaitCallback callback, Object* pState)</span>", "md_docs_2_thread_pool.html#autotoc_md1783", null ],
        [ "<span class=\"tt\">static bool SetMinThreads(int iMinThreads)</span>", "md_docs_2_thread_pool.html#autotoc_md1784", null ]
      ] ],
      [ "Example", "md_docs_2_thread_pool.html#autotoc_md1786", null ]
    ] ],
    [ "TimeProvider", "md_docs_2_time_provider.html", [
      [ "Syntax", "md_docs_2_time_provider.html#autotoc_md1789", null ],
      [ "Static Methods", "md_docs_2_time_provider.html#autotoc_md1791", [
        [ "<span class=\"tt\">static TimeProviderPtr GetSystem()</span>", "md_docs_2_time_provider.html#autotoc_md1792", null ]
      ] ],
      [ "Member Functions", "md_docs_2_time_provider.html#autotoc_md1794", [
        [ "<span class=\"tt\">virtual DateTimeOffset GetUtcNow() const = 0</span>", "md_docs_2_time_provider.html#autotoc_md1795", null ],
        [ "<span class=\"tt\">virtual DateTimeOffset GetLocalNow() const = 0</span>", "md_docs_2_time_provider.html#autotoc_md1796", null ],
        [ "<span class=\"tt\">virtual int64_t GetTimestamp() const = 0</span>", "md_docs_2_time_provider.html#autotoc_md1797", null ],
        [ "<span class=\"tt\">virtual int64_t GetTimestampFrequency() const = 0</span>", "md_docs_2_time_provider.html#autotoc_md1798", null ],
        [ "<span class=\"tt\">TimeSpan GetElapsedTime(int64_t startingTimestamp) const</span>", "md_docs_2_time_provider.html#autotoc_md1799", null ],
        [ "<span class=\"tt\">TimeSpan GetElapsedTime(int64_t startingTimestamp, int64_t endingTimestamp) const</span>", "md_docs_2_time_provider.html#autotoc_md1800", null ]
      ] ],
      [ "Example", "md_docs_2_time_provider.html#autotoc_md1802", null ]
    ] ],
    [ "TimeSpan", "md_docs_2_time_span.html", [
      [ "Syntax", "md_docs_2_time_span.html#autotoc_md1805", null ],
      [ "Constants", "md_docs_2_time_span.html#autotoc_md1807", null ],
      [ "Constructors &amp; Factory Methods", "md_docs_2_time_span.html#autotoc_md1809", [
        [ "<span class=\"tt\">TimeSpan()</span>", "md_docs_2_time_span.html#autotoc_md1810", null ],
        [ "<span class=\"tt\">TimeSpan(int64_t ticks)</span> / <span class=\"tt\">static TimeSpan FromTicks(int64_t ticks)</span>", "md_docs_2_time_span.html#autotoc_md1811", null ],
        [ "<span class=\"tt\">static TimeSpan FromDays(double value)</span>", "md_docs_2_time_span.html#autotoc_md1812", null ],
        [ "<span class=\"tt\">static TimeSpan FromHours(double value)</span> / <span class=\"tt\">FromMinutes()</span> / <span class=\"tt\">FromSeconds()</span> / <span class=\"tt\">FromMilliseconds()</span>", "md_docs_2_time_span.html#autotoc_md1813", null ]
      ] ],
      [ "Properties &amp; Calculations", "md_docs_2_time_span.html#autotoc_md1815", [
        [ "<span class=\"tt\">int64_t GetTicks() const</span>", "md_docs_2_time_span.html#autotoc_md1816", null ],
        [ "<span class=\"tt\">double GetTotalMilliseconds() const</span> / <span class=\"tt\">GetTotalSeconds()</span> / <span class=\"tt\">GetTotalMinutes()</span> / <span class=\"tt\">GetTotalHours()</span> / <span class=\"tt\">GetTotalDays()</span>", "md_docs_2_time_span.html#autotoc_md1817", null ],
        [ "<span class=\"tt\">TimeSpan operator+(const TimeSpan&amp; other) const</span> / <span class=\"tt\">operator-()</span>", "md_docs_2_time_span.html#autotoc_md1818", null ],
        [ "<span class=\"tt\">bool operator==(const TimeSpan&amp; other) const</span> / <span class=\"tt\">!=</span> / <span class=\"tt\">&lt;</span> / <span class=\"tt\">&lt;=</span> / <span class=\"tt\">&gt;</span> / <span class=\"tt\">&gt;=</span>", "md_docs_2_time_span.html#autotoc_md1819", null ]
      ] ],
      [ "Example", "md_docs_2_time_span.html#autotoc_md1821", null ]
    ] ],
    [ "TimeZone", "md_docs_2_time_zone.html", [
      [ "Syntax", "md_docs_2_time_zone.html#autotoc_md1824", null ],
      [ "Static Methods", "md_docs_2_time_zone.html#autotoc_md1826", [
        [ "<span class=\"tt\">static TimeZone* GetCurrentTimeZone()</span>", "md_docs_2_time_zone.html#autotoc_md1827", null ],
        [ "<span class=\"tt\">static bool IsDaylightSavingTime(const DateTimeOffset&amp; time, const DaylightTime&amp; daylightTimes)</span>", "md_docs_2_time_zone.html#autotoc_md1828", null ]
      ] ],
      [ "Member Functions", "md_docs_2_time_zone.html#autotoc_md1830", [
        [ "<span class=\"tt\">virtual String GetStandardName() const = 0</span>", "md_docs_2_time_zone.html#autotoc_md1831", null ],
        [ "<span class=\"tt\">virtual String GetDaylightName() const = 0</span>", "md_docs_2_time_zone.html#autotoc_md1832", null ],
        [ "<span class=\"tt\">virtual DaylightTime GetDaylightChanges(int year) = 0</span>", "md_docs_2_time_zone.html#autotoc_md1833", null ],
        [ "<span class=\"tt\">virtual TimeSpan GetUtcOffset(const DateTimeOffset&amp; time) = 0</span>", "md_docs_2_time_zone.html#autotoc_md1834", null ],
        [ "<span class=\"tt\">virtual bool IsDaylightSavingTime(const DateTimeOffset&amp; time)</span>", "md_docs_2_time_zone.html#autotoc_md1835", null ],
        [ "<span class=\"tt\">virtual DateTimeOffset ToLocalTime(const DateTimeOffset&amp; time)</span>", "md_docs_2_time_zone.html#autotoc_md1836", null ],
        [ "<span class=\"tt\">virtual DateTimeOffset ToUniversalTime(const DateTimeOffset&amp; time)</span>", "md_docs_2_time_zone.html#autotoc_md1837", null ]
      ] ],
      [ "Example", "md_docs_2_time_zone.html#autotoc_md1839", null ]
    ] ],
    [ "TimeZoneInfo", "md_docs_2_time_zone_info.html", [
      [ "Syntax", "md_docs_2_time_zone_info.html#autotoc_md1842", null ],
      [ "Static Properties / Methods", "md_docs_2_time_zone_info.html#autotoc_md1844", [
        [ "<span class=\"tt\">static TimeZoneInfo Local()</span>", "md_docs_2_time_zone_info.html#autotoc_md1845", null ],
        [ "<span class=\"tt\">static TimeZoneInfo Utc()</span>", "md_docs_2_time_zone_info.html#autotoc_md1846", null ],
        [ "<span class=\"tt\">static TimeZoneInfo FindSystemTimeZoneById(const String&amp; id)</span>", "md_docs_2_time_zone_info.html#autotoc_md1847", null ],
        [ "<span class=\"tt\">static DateTimeOffset ConvertTime(const DateTimeOffset&amp; dateTimeOffset, const TimeZoneInfo&amp; destinationTimeZone)</span>", "md_docs_2_time_zone_info.html#autotoc_md1848", null ],
        [ "<span class=\"tt\">static DateTimeOffset ConvertTime(const DateTimeOffset&amp; dateTimeOffset, const TimeZoneInfo&amp; sourceTimeZone, const TimeZoneInfo&amp; destinationTimeZone)</span>", "md_docs_2_time_zone_info.html#autotoc_md1849", null ]
      ] ],
      [ "Properties &amp; Calculations", "md_docs_2_time_zone_info.html#autotoc_md1851", [
        [ "<span class=\"tt\">String GetId() const</span> / <span class=\"tt\">GetDisplayName()</span> / <span class=\"tt\">GetStandardName()</span> / <span class=\"tt\">GetDaylightName()</span>", "md_docs_2_time_zone_info.html#autotoc_md1852", null ],
        [ "<span class=\"tt\">TimeSpan GetBaseUtcOffset() const</span>", "md_docs_2_time_zone_info.html#autotoc_md1853", null ],
        [ "<span class=\"tt\">bool SupportsDaylightSavingTime() const</span>", "md_docs_2_time_zone_info.html#autotoc_md1854", null ],
        [ "<span class=\"tt\">TimeSpan GetUtcOffset(const DateTimeOffset&amp; dateTimeOffset) const</span>", "md_docs_2_time_zone_info.html#autotoc_md1855", null ],
        [ "<span class=\"tt\">bool IsDaylightSavingTime(const DateTimeOffset&amp; dateTimeOffset) const</span>", "md_docs_2_time_zone_info.html#autotoc_md1856", null ],
        [ "<span class=\"tt\">bool Equals(const TimeZoneInfo&amp; other) const</span>", "md_docs_2_time_zone_info.html#autotoc_md1857", null ]
      ] ],
      [ "Example", "md_docs_2_time_zone_info.html#autotoc_md1859", null ]
    ] ],
    [ "Uri", "md_docs_2_uri.html", [
      [ "Syntax", "md_docs_2_uri.html#autotoc_md1862", null ],
      [ "Constructors", "md_docs_2_uri.html#autotoc_md1864", [
        [ "<span class=\"tt\">Uri(const String&amp; uriString)</span>", "md_docs_2_uri.html#autotoc_md1865", null ]
      ] ],
      [ "Static Methods", "md_docs_2_uri.html#autotoc_md1867", [
        [ "<span class=\"tt\">static String EscapeDataString(const String&amp; stringToEscape)</span>", "md_docs_2_uri.html#autotoc_md1868", null ],
        [ "<span class=\"tt\">static String UnescapeDataString(const String&amp; stringToUnescape)</span>", "md_docs_2_uri.html#autotoc_md1869", null ]
      ] ],
      [ "Properties &amp; Methods", "md_docs_2_uri.html#autotoc_md1871", [
        [ "<span class=\"tt\">String GetScheme() const</span>", "md_docs_2_uri.html#autotoc_md1872", null ],
        [ "<span class=\"tt\">String GetHost() const</span>", "md_docs_2_uri.html#autotoc_md1873", null ],
        [ "<span class=\"tt\">int GetPort() const</span>", "md_docs_2_uri.html#autotoc_md1874", null ],
        [ "<span class=\"tt\">String GetAuthority() const</span>", "md_docs_2_uri.html#autotoc_md1875", null ],
        [ "<span class=\"tt\">String GetAbsolutePath() const</span>", "md_docs_2_uri.html#autotoc_md1876", null ],
        [ "<span class=\"tt\">String GetAbsoluteUri() const</span>", "md_docs_2_uri.html#autotoc_md1877", null ],
        [ "<span class=\"tt\">String GetQuery() const</span>", "md_docs_2_uri.html#autotoc_md1878", null ],
        [ "<span class=\"tt\">String GetFragment() const</span>", "md_docs_2_uri.html#autotoc_md1879", null ],
        [ "<span class=\"tt\">String GetUserInfo() const</span>", "md_docs_2_uri.html#autotoc_md1880", null ],
        [ "<span class=\"tt\">String GetOriginalString() const</span>", "md_docs_2_uri.html#autotoc_md1881", null ],
        [ "<span class=\"tt\">String ToString() const</span>", "md_docs_2_uri.html#autotoc_md1882", null ],
        [ "<span class=\"tt\">bool IsDefaultPort() const</span>", "md_docs_2_uri.html#autotoc_md1883", null ],
        [ "<span class=\"tt\">bool IsFile() const</span>", "md_docs_2_uri.html#autotoc_md1884", null ],
        [ "<span class=\"tt\">bool IsLoopback() const</span>", "md_docs_2_uri.html#autotoc_md1885", null ]
      ] ],
      [ "Example", "md_docs_2_uri.html#autotoc_md1887", null ]
    ] ],
    [ "UriBuilder", "md_docs_2_uri_builder.html", [
      [ "Syntax", "md_docs_2_uri_builder.html#autotoc_md1890", null ],
      [ "Constructors", "md_docs_2_uri_builder.html#autotoc_md1892", [
        [ "<span class=\"tt\">UriBuilder()</span>", "md_docs_2_uri_builder.html#autotoc_md1893", null ],
        [ "<span class=\"tt\">UriBuilder(const String&amp; uri)</span>", "md_docs_2_uri_builder.html#autotoc_md1894", null ],
        [ "<span class=\"tt\">UriBuilder(const Uri&amp; uri)</span>", "md_docs_2_uri_builder.html#autotoc_md1895", null ],
        [ "<span class=\"tt\">UriBuilder(const String&amp; scheme, const String&amp; host)</span>", "md_docs_2_uri_builder.html#autotoc_md1896", null ],
        [ "<span class=\"tt\">UriBuilder(const String&amp; scheme, const String&amp; host, int port)</span>", "md_docs_2_uri_builder.html#autotoc_md1897", null ]
      ] ],
      [ "Properties &amp; Mutators", "md_docs_2_uri_builder.html#autotoc_md1899", [
        [ "<span class=\"tt\">Uri GetUri()</span>", "md_docs_2_uri_builder.html#autotoc_md1900", null ],
        [ "<span class=\"tt\">String ToString()</span>", "md_docs_2_uri_builder.html#autotoc_md1901", null ]
      ] ],
      [ "Example", "md_docs_2_uri_builder.html#autotoc_md1903", null ]
    ] ],
    [ "enum <span class=\"tt\">UriComponents</span>", "md_docs_2_uri_components.html", null ],
    [ "enum <span class=\"tt\">UriFormat</span>", "md_docs_2_uri_format.html", null ],
    [ "class <span class=\"tt\">UriParser</span>", "md_docs_2_uri_parser.html", null ],
    [ "UserPrincipal, UserInfo &amp; UserClass", "md_docs_2_user_principal.html", [
      [ "<span class=\"tt\">UserClass</span> Enum", "md_docs_2_user_principal.html#autotoc_md1917", null ],
      [ "<span class=\"tt\">UserInfo</span> Struct", "md_docs_2_user_principal.html#autotoc_md1919", null ],
      [ "<span class=\"tt\">UserPrincipal</span> Class", "md_docs_2_user_principal.html#autotoc_md1921", [
        [ "Syntax", "md_docs_2_user_principal.html#autotoc_md1922", null ]
      ] ],
      [ "Static Methods", "md_docs_2_user_principal.html#autotoc_md1924", [
        [ "<span class=\"tt\">static UserInfo GetCurrent()</span>", "md_docs_2_user_principal.html#autotoc_md1925", null ],
        [ "<span class=\"tt\">static UserInfo GetUser(const String&amp; sUsername)</span>", "md_docs_2_user_principal.html#autotoc_md1926", null ],
        [ "<span class=\"tt\">static List&lt;UserInfo&gt; EnumerateUsers()</span>", "md_docs_2_user_principal.html#autotoc_md1927", null ]
      ] ],
      [ "Example", "md_docs_2_user_principal.html#autotoc_md1929", null ]
    ] ],
    [ "Version", "md_docs_2_version.html", [
      [ "Syntax", "md_docs_2_version.html#autotoc_md1932", null ],
      [ "Constructors", "md_docs_2_version.html#autotoc_md1934", [
        [ "<span class=\"tt\">Version()</span>", "md_docs_2_version.html#autotoc_md1935", null ],
        [ "<span class=\"tt\">Version(int iMajor, int iMinor)</span>", "md_docs_2_version.html#autotoc_md1936", null ],
        [ "<span class=\"tt\">Version(int iMajor, int iMinor, int iBuild)</span>", "md_docs_2_version.html#autotoc_md1937", null ],
        [ "<span class=\"tt\">Version(int iMajor, int iMinor, int iBuild, int iRevision)</span>", "md_docs_2_version.html#autotoc_md1938", null ]
      ] ],
      [ "Properties &amp; Methods", "md_docs_2_version.html#autotoc_md1940", [
        [ "<span class=\"tt\">String ToString() const</span>", "md_docs_2_version.html#autotoc_md1941", null ],
        [ "<span class=\"tt\">static Version Parse(const String&amp; sInput)</span>", "md_docs_2_version.html#autotoc_md1942", null ],
        [ "<span class=\"tt\">static bool TryParse(const String&amp; sInput, Version&amp; vResult)</span>", "md_docs_2_version.html#autotoc_md1943", null ],
        [ "<span class=\"tt\">bool operator==(const Version&amp; vOther) const</span>", "md_docs_2_version.html#autotoc_md1944", null ],
        [ "<span class=\"tt\">bool operator!=(const Version&amp; vOther) const</span>", "md_docs_2_version.html#autotoc_md1945", null ]
      ] ],
      [ "Example", "md_docs_2_version.html#autotoc_md1947", null ]
    ] ],
    [ "WaitHandle, EventWaitHandle, AutoResetEvent &amp; ManualResetEvent", "md_docs_2_wait_handle.html", [
      [ "<span class=\"tt\">WaitHandle</span> (Abstract Base Class)", "md_docs_2_wait_handle.html#autotoc_md1950", [
        [ "Syntax", "md_docs_2_wait_handle.html#autotoc_md1951", null ],
        [ "Pure Virtual Members", "md_docs_2_wait_handle.html#autotoc_md1952", null ]
      ] ],
      [ "<span class=\"tt\">EventWaitHandle</span>", "md_docs_2_wait_handle.html#autotoc_md1954", [
        [ "Syntax", "md_docs_2_wait_handle.html#autotoc_md1955", null ],
        [ "Constructors", "md_docs_2_wait_handle.html#autotoc_md1956", null ],
        [ "Methods", "md_docs_2_wait_handle.html#autotoc_md1957", null ]
      ] ],
      [ "<span class=\"tt\">AutoResetEvent</span>", "md_docs_2_wait_handle.html#autotoc_md1959", [
        [ "Constructors", "md_docs_2_wait_handle.html#autotoc_md1960", null ],
        [ "Static Methods", "md_docs_2_wait_handle.html#autotoc_md1961", null ]
      ] ],
      [ "<span class=\"tt\">ManualResetEvent</span>", "md_docs_2_wait_handle.html#autotoc_md1963", [
        [ "Constructors", "md_docs_2_wait_handle.html#autotoc_md1964", null ],
        [ "Static Methods", "md_docs_2_wait_handle.html#autotoc_md1965", null ]
      ] ],
      [ "Example", "md_docs_2_wait_handle.html#autotoc_md1967", null ]
    ] ],
    [ "WebApplication, WebAppServer &amp; ControllerBase", "md_docs_2_web_application.html", [
      [ "<span class=\"tt\">WebApplicationBuilder</span> &amp; <span class=\"tt\">WebApplication</span>", "md_docs_2_web_application.html#autotoc_md1970", [
        [ "Creation &amp; Bootstrapping", "md_docs_2_web_application.html#autotoc_md1971", null ],
        [ "Minimal Endpoint Routing", "md_docs_2_web_application.html#autotoc_md1972", null ]
      ] ],
      [ "<span class=\"tt\">ControllerBase</span>", "md_docs_2_web_application.html#autotoc_md1974", [
        [ "Helper Action Results", "md_docs_2_web_application.html#autotoc_md1975", null ],
        [ "Authentication &amp; Context", "md_docs_2_web_application.html#autotoc_md1976", null ]
      ] ],
      [ "<span class=\"tt\">WebAppServer</span> (Static Web Content Hosting)", "md_docs_2_web_application.html#autotoc_md1978", [
        [ "Methods", "md_docs_2_web_application.html#autotoc_md1979", null ]
      ] ],
      [ "Example", "md_docs_2_web_application.html#autotoc_md1981", null ]
    ] ],
    [ "WebAppServer", "md_docs_2_web_app_server.html", [
      [ "Namespace", "md_docs_2_web_app_server.html#autotoc_md1983", null ],
      [ "Header", "md_docs_2_web_app_server.html#autotoc_md1984", null ],
      [ "Key Methods", "md_docs_2_web_app_server.html#autotoc_md1985", null ],
      [ "Supported MIME Types", "md_docs_2_web_app_server.html#autotoc_md1987", null ],
      [ "Complete Usage Example", "md_docs_2_web_app_server.html#autotoc_md1989", null ]
    ] ],
    [ "WebSocket &amp; WebSocketException", "md_docs_2_web_socket.html", [
      [ "Enums", "md_docs_2_web_socket.html#autotoc_md1992", [
        [ "<span class=\"tt\">WebSocketState</span>", "md_docs_2_web_socket.html#autotoc_md1993", null ],
        [ "<span class=\"tt\">WebSocketError</span>", "md_docs_2_web_socket.html#autotoc_md1994", null ]
      ] ],
      [ "<span class=\"tt\">WebSocket</span>", "md_docs_2_web_socket.html#autotoc_md1996", [
        [ "Constructors &amp; Destructor", "md_docs_2_web_socket.html#autotoc_md1997", null ],
        [ "Member Functions", "md_docs_2_web_socket.html#autotoc_md1998", null ]
      ] ],
      [ "<span class=\"tt\">WebSocketException</span>", "md_docs_2_web_socket.html#autotoc_md2000", [
        [ "Constructors", "md_docs_2_web_socket.html#autotoc_md2001", null ],
        [ "Member Functions", "md_docs_2_web_socket.html#autotoc_md2002", null ]
      ] ],
      [ "Usage Example", "md_docs_2_web_socket.html#autotoc_md2004", null ]
    ] ],
    [ "System.Security.Cryptography.X509Certificates Namespace", "md_docs_2_x509_certificate2.html", [
      [ "Code Example", "md_docs_2_x509_certificate2.html#autotoc_md2014", null ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", "namespacemembers_dup" ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_abandoned_mutex_exception_8h.html",
"_i_logger_8h_source.html",
"_task_8h_source.html",
"class_dot_net_dupe_1_1_extensions_1_1_logging_1_1_aggregate_logger.html#a2334834a13ffbf195dd81d0f21e399f0",
"class_dot_net_dupe_1_1_system_1_1_aggregate_exception.html#a4c11084968ba9f69ec0df329790e1d69",
"class_dot_net_dupe_1_1_system_1_1_collections_1_1_concurrent_1_1_concurrent_bag.html",
"class_dot_net_dupe_1_1_system_1_1_collections_1_1_generic_1_1_list.html#a91500ae8dff47215ba3fb875f3bc3f1a",
"class_dot_net_dupe_1_1_system_1_1_console.html#af57f2f58d2e2b0fd06da15a5d8d01029",
"class_dot_net_dupe_1_1_system_1_1_data_1_1_common_1_1_db_data_reader.html#a088ac22ef00b471f2b25a4369b4dc36d",
"class_dot_net_dupe_1_1_system_1_1_date_time.html#ad60377177526fddc1b690495887ad2bb",
"class_dot_net_dupe_1_1_system_1_1_diagnostics_1_1_process_start_info.html#ab1eb2b24e1e834614a6e736daa5da0e6",
"class_dot_net_dupe_1_1_system_1_1_event_handler.html#a36413955c13d5c56150a2d35b41c773f",
"class_dot_net_dupe_1_1_system_1_1_i_o_1_1_directory.html#a3dbafa019ac342673de43f5e8e489699",
"class_dot_net_dupe_1_1_system_1_1_i_o_1_1_stream.html#a115f97e9d4c7a1e7d682b10cd593f4a7",
"class_dot_net_dupe_1_1_system_1_1_net_1_1_http_1_1_download_progress_changed_event_args.html#ad27167e5e3a6825c7e95cba5231ae80e",
"class_dot_net_dupe_1_1_system_1_1_net_1_1_security_1_1_ssl_stream.html#a9c14cc758fb7eb0309864f7de4c9db58",
"class_dot_net_dupe_1_1_system_1_1_object_disposed_exception.html#a820614c9fdab382fa5662091151c38e1",
"class_dot_net_dupe_1_1_system_1_1_string.html#a2adda320a0a377fee342ea6dcc5eeb07",
"class_dot_net_dupe_1_1_system_1_1_text_1_1_json_1_1_json_element.html#acb30806723b53709f2777e395eb69444",
"class_dot_net_dupe_1_1_system_1_1_threading_1_1_mutex.html#aced69541157a0fa5c21bb85051edb0e3",
"class_dot_net_dupe_1_1_system_1_1_time_zone_info.html#ace29d3b3c8f1b092ee27dab532222e34",
"class_dot_net_dupe_1_1_web_app_core_1_1_controllers_1_1_controller_base.html#a997df4a939051e85607f5b33e3805675",
"functions_func_n.html",
"md_docs_2_concurrent_dictionary.html#autotoc_md303",
"md_docs_2_environment.html#autotoc_md617",
"md_docs_2_json_serializer.html",
"md_docs_2_queue.html#autotoc_md1276",
"md_docs_2_string.html#autotoc_md1596",
"md_docs_2_version.html#autotoc_md1947",
"namespace_dot_net_dupe_1_1_system_1_1_diagnostics.html#a9df595a071c41aa77f05b905fab00771a278d01e5af56273bae1bb99a98b370cd",
"namespace_dot_net_dupe_1_1_system_1_1_threading.html#a9f37dba995238d99589556b039006362",
"struct_dot_net_dupe_1_1_system_1_1_diagnostics_1_1_network_usage_info.html#adcbf64cc814fde315a24e9489d40a244",
"struct_dot_net_dupe_1_1_system_1_1_text_1_1_json_1_1_json_converter_3_01_collections_1_1_generic_1_1_list_3_01_u_01_4_01_4.html"
];

const SYNCONMSG = 'click to disable panel synchronization';
const SYNCOFFMSG = 'click to enable panel synchronization';
const LISTOFALLMEMBERS = 'List of all members';