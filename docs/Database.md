# SqlConnection, SqlCommand, SqlDataReader &amp; SqlParameter

**Namespace:** `DotNetDupe::System::Data::SqlClient`  
**Header:** `#include "System/Data/SqlClient/SqlConnection.h"`, `#include "System/Data/SqlClient/SqlCommand.h"`, `#include "System/Data/SqlClient/SqlDataReader.h"`, `#include "System/Data/SqlClient/SqlParameter.h"`

Provides ADO.NET-style database connectivity supporting SQL commands, parameterized queries, and forward-only data reading across embedded SQLite and in-memory databases.

---

## `SqlConnection`

### Constructors
- `SqlConnection()`
- `SqlConnection(const String& sConnectionString)`

### Member Functions
- `void Open()`: Opens a database connection.
- `void Close()`: Closes the connection to the database.
- `bool IsOpen() const`: Gets whether the connection is open.
- `String GetConnectionString() const` / `void SetConnectionString(const String& sConnStr)`
- `String GetDatabaseName() const`: Returns database file / catalog name.
- `SmartPointer<DbCommand> CreateCommand()`: Creates a new command associated with this connection.

---

## `SqlCommand`

### Constructors
- `SqlCommand()`
- `SqlCommand(const String& sText, SqlConnection* connection)`

### Member Functions
- `String GetCommandText() const` / `void SetCommandText(const String& sText)`
- `SmartPointer<DbParameterCollection> GetParameters() const`: Parameter collection for SQL injection prevention.
- `int ExecuteNonQuery()`: Executes a Transact-SQL statement (e.g. `INSERT`, `UPDATE`, `DELETE`) and returns the number of rows affected.
- `String ExecuteScalar()`: Executes query and returns the first column of the first row in the result set.
- `SmartPointer<DbDataReader> ExecuteReader()`: Sends the `CommandText` to the `Connection` and builds a `SqlDataReader`.

---

## `SqlDataReader`

### Member Functions
- `bool Read()`: Advances the `SqlDataReader` to the next record.
- `String GetString(int iOrdinal)`: Gets the value of the specified column as a `String`.
- `int GetInt32(int iOrdinal)`: Gets the value of the specified column as a 32-bit signed integer.
- `double GetDouble(int iOrdinal)`: Gets the value of the specified column as a double-precision floating point number.
- `bool IsDBNull(int iOrdinal)`: Gets a value that indicates whether the column contains non-existent or missing values.
- `int GetFieldCount() const`: Gets the number of columns in the current row.
- `String GetName(int iOrdinal)`: Gets the name of the specified column.
- `int GetOrdinal(const String& sName)`: Gets the column ordinal given the name of the column.
- `String operator[](const String& sName)`: Gets the column value by column name.
- `String operator[](int iOrdinal)`: Gets the column value by column index.

---

## `SqlParameter`

### Constructors
- `SqlParameter(const String& sName, const String& sValue)`: Initializes a parameter with name (e.g. `"@name"`) and value.

---

## Example

```cpp
#include "System/Console.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/Data/SqlClient/SqlParameter.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Data::SqlClient;

int main() {
    SqlConnection conn("Data Source=test.db;");
    conn.Open();

    // Create table & Insert
    auto cmd = conn.CreateCommand();
    cmd->SetCommandText("CREATE TABLE Users (Id INTEGER PRIMARY KEY, Username TEXT, Balance REAL);");
    cmd->ExecuteNonQuery();

    auto insertCmd = conn.CreateCommand();
    insertCmd->SetCommandText("INSERT INTO Users (Username, Balance) VALUES (@user, @bal);");
    insertCmd->GetParameters()->Add(new SqlParameter("@user", "Alice"));
    insertCmd->GetParameters()->Add(new SqlParameter("@bal", "1500.75"));
    insertCmd->ExecuteNonQuery();

    // Query Data
    auto selectCmd = conn.CreateCommand();
    selectCmd->SetCommandText("SELECT Id, Username, Balance FROM Users;");
    auto reader = selectCmd->ExecuteReader();

    while (reader->Read()) {
        int id = reader->GetInt32(0);
        String name = reader->GetString(1);
        double balance = reader->GetDouble(2);
        Console::WriteLine("User #{0}: {1} | Balance: ${2:F2}", id, name, balance);
    }

    conn.Close();
    return 0;
}
```
