#include "System/Console.h"
#include "System/SmartPointer.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/BasicException.h"
#include "Demos.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Data::SqlClient;

void DemonstrateDatabase() {
    Console::WriteLine("\n=== Database ADO.NET Demonstration ===");
    try {
        // 1. Establish connection to simulated database
        Console::WriteLine("Connecting to Database 'DemoShopDb'...");
        SqlConnection conn("Data Source=DemoShopDb;");
        conn.Open();
        Console::WriteLine("Connection state: OPEN");

        // 2. Create a simulated table
        Console::WriteLine("\n[SQL] Creating Table 'Products'...");
        auto cmdCreate = conn.CreateCommand();
        cmdCreate->SetCommandText("CREATE TABLE Products (Id INT, Name VARCHAR, Price INT)");
        cmdCreate->ExecuteNonQuery();
        Console::WriteLine("Table 'Products' created successfully.");

        // 3. Insert records using parameterized commands (prevent SQL injection)
        Console::WriteLine("\n[SQL] Inserting records using parameters...");
        auto cmdInsert = conn.CreateCommand();
        cmdInsert->SetCommandText("INSERT INTO Products (Id, Name, Price) VALUES (@id, @name, @price)");

        // Insert Record 1
        cmdInsert->GetParameters()->AddWithValue("@id", 101);
        cmdInsert->GetParameters()->AddWithValue("@name", "Ergonomic Keyboard");
        cmdInsert->GetParameters()->AddWithValue("@price", 120);
        cmdInsert->ExecuteNonQuery();

        // Insert Record 2
        cmdInsert->GetParameters()->Clear();
        cmdInsert->GetParameters()->AddWithValue("@id", 102);
        cmdInsert->GetParameters()->AddWithValue("@name", "Wireless Mouse");
        cmdInsert->GetParameters()->AddWithValue("@price", 45);
        cmdInsert->ExecuteNonQuery();

        // Insert Record 3
        cmdInsert->GetParameters()->Clear();
        cmdInsert->GetParameters()->AddWithValue("@id", 103);
        cmdInsert->GetParameters()->AddWithValue("@name", "UltraWide Monitor");
        cmdInsert->GetParameters()->AddWithValue("@price", 350);
        cmdInsert->ExecuteNonQuery();

        Console::WriteLine("3 records inserted successfully.");

        // 4. Query all records using SqlDataReader
        Console::WriteLine("\n[SQL] Selecting all records:");
        auto cmdSelectAll = conn.CreateCommand();
        cmdSelectAll->SetCommandText("SELECT Id, Name, Price FROM Products");
        
        auto reader = cmdSelectAll->ExecuteReader();
        while (reader->Read()) {
            int id = reader->GetInt32(0);
            String name = reader->GetString(1);
            int price = reader->GetInt32(2);

            Console::Write(" - [ID: ");
            Console::Write(id);
            Console::Write("] Name: ");
            Console::Write(name);
            Console::Write(", Price: $");
            Console::WriteLine(price);
        }

        // 5. Update a record
        Console::WriteLine("\n[SQL] Updating price of 'Wireless Mouse' (ID: 102) to $39...");
        auto cmdUpdate = conn.CreateCommand();
        cmdUpdate->SetCommandText("UPDATE Products SET Price = @price WHERE Id = @id");
        cmdUpdate->GetParameters()->AddWithValue("@id", 102);
        cmdUpdate->GetParameters()->AddWithValue("@price", 39);
        int rowsUpdated = cmdUpdate->ExecuteNonQuery();
        Console::Write("Rows affected: ");
        Console::WriteLine(rowsUpdated);

        // Verify the update using ExecuteScalar
        Console::WriteLine("\n[SQL] Verifying update using ExecuteScalar...");
        auto cmdScalar = conn.CreateCommand();
        cmdScalar->SetCommandText("SELECT Price FROM Products WHERE Id = @id");
        cmdScalar->GetParameters()->AddWithValue("@id", 102);
        String updatedPrice = cmdScalar->ExecuteScalar();
        Console::Write("New price of ID 102 is: $");
        Console::WriteLine(updatedPrice);

        // 6. Close the connection
        conn.Close();
        Console::WriteLine("\nConnection state: CLOSED");

    } catch (const BasicException<char>& ex) {
        Console::Write("[Database Exception] Error: ");
        Console::WriteLine(ex.What());
    } catch (const std::exception& ex) {
        Console::Write("[Standard Exception] Error: ");
        Console::WriteLine(ex.what());
    }
}
