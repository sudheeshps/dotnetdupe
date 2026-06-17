### ADO.NET Database Client (`System::Data`)

Provides a set of classes for connecting to a database, executing SQL commands, and reading results. This mirrors the standard ADO.NET Base Class Library APIs in C#.

DotNetDupe includes a fully self-contained in-memory SQL execution engine, making it possible to execute relational SQL operations (`CREATE TABLE`, `INSERT`, `SELECT`, `UPDATE`, `DELETE`) with parameters without external database dependencies.

---

### class `SqlConnection` (inherits from `DbConnection`)
Represents an open connection to a database.

##### `SqlConnection(const String& sConnectionString)`
Initializes a new instance of the `SqlConnection` class with the specified connection string (e.g. `"Data Source=MyDatabase;"`).

##### `void Open()`
Opens a database connection.

##### `void Close()`
Closes the connection to the database.

##### `SmartPointer<DbCommand> CreateCommand()`
Creates and returns a `SqlCommand` object associated with the `SqlConnection`.

---

### class `SqlCommand` (inherits from `DbCommand`)
Represents a Transact-SQL statement or stored procedure to execute against a database.

##### `void SetCommandText(const String& sCommandText)`
Sets the SQL statement to execute.

##### `SmartPointer<DbParameterCollection> GetParameters() const`
Gets the collection of parameters associated with the SQL command.

##### `SmartPointer<DbDataReader> ExecuteReader()`
Sends the `CommandText` to the connection and builds a `SqlDataReader`.

##### `int ExecuteNonQuery()`
Executes an SQL statement (such as `INSERT`, `UPDATE`, `DELETE`, or `CREATE TABLE`) and returns the number of rows affected.

##### `String ExecuteScalar()`
Executes the query and returns the first column of the first row in the result set returned by the query.

---

### class `SqlDataReader` (inherits from `DbDataReader`)
Provides a way of reading a forward-only stream of rows from a database.

##### `bool Read()`
Advances the reader to the next record in a result set. Returns `true` if there are more rows; otherwise, `false`.

##### `String GetString(int iOrdinal)`
Gets the value of the specified column as a `String`.

##### `int GetInt32(int iOrdinal)`
Gets the value of the specified column as a 32-bit signed integer.

##### `double GetDouble(int iOrdinal)`
Gets the value of the specified column as a double-precision floating-point number.

##### `bool IsDBNull(int iOrdinal)`
Gets a value that indicates whether the column contains non-existent or missing values.

##### `int GetOrdinal(const String& sName)`
Gets the column ordinal, given the name of the column.

##### `String operator[](const String& sName)`
Gets the value of the specified column by name.

##### `String operator[](int iOrdinal)`
Gets the value of the specified column by ordinal index.

---

## Code Example

The following example demonstrates opening a connection, creating a table, executing parameterized inserts, querying with a reader, updating rows, and closing.

```cpp
#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/BasicException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Data::SqlClient;

int main() {
    try {
        // 1. Establish connection to simulated database
        SqlConnection conn("Data Source=ShopDb;");
        conn.Open();
        Console::WriteLine("Database connection opened.");

        // 2. Create a table
        auto cmdCreate = conn.CreateCommand();
        cmdCreate->SetCommandText("CREATE TABLE Products (Id INT, Name VARCHAR, Price INT)");
        cmdCreate->ExecuteNonQuery();

        // 3. Insert records using parameters
        auto cmdInsert = conn.CreateCommand();
        cmdInsert->SetCommandText("INSERT INTO Products (Id, Name, Price) VALUES (@id, @name, @price)");

        cmdInsert->GetParameters()->AddWithValue("@id", 1);
        cmdInsert->GetParameters()->AddWithValue("@name", "Coffee Maker");
        cmdInsert->GetParameters()->AddWithValue("@price", 85);
        cmdInsert->ExecuteNonQuery();

        cmdInsert->GetParameters()->Clear();
        cmdInsert->GetParameters()->AddWithValue("@id", 2);
        cmdInsert->GetParameters()->AddWithValue("@name", "Toaster");
        cmdInsert->GetParameters()->AddWithValue("@price", 45);
        cmdInsert->ExecuteNonQuery();

        // 4. Query records
        auto cmdSelect = conn.CreateCommand();
        cmdSelect->SetCommandText("SELECT Name, Price FROM Products WHERE Price > @minPrice");
        cmdSelect->GetParameters()->AddWithValue("@minPrice", 50);

        auto reader = cmdSelect->ExecuteReader();
        Console::WriteLine("\nProducts costing more than $50:");
        while (reader->Read()) {
            String name = reader->GetString(0);
            int price = reader->GetInt32(1);
            Console::WriteLine(" - " + name + ": $" + Convert::ToString(price));
        }

        // 5. Clean up
        conn.Close();
        Console::WriteLine("\nDatabase connection closed.");

    } catch (const BasicException<char>& ex) {
        Console::WriteLine("Database Error: " + String(ex.What()));
    }
    return 0;
}
```
