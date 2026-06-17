#include "pch.h"
#include "gtest/gtest.h"
#include "System/Data/SqlClient/SqlConnection.h"
#include "System/Data/SqlClient/SqlCommand.h"
#include "System/Data/SqlClient/SqlDataReader.h"
#include "System/Data/Internal/DatabaseEngine.h"
#include "System/InvalidOperationException.h"

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Data::SqlClient;

namespace DatabaseTests {

    TEST(DatabaseTests, GivenConnectionOpen_WhenSqlExecuted_CreatesInsertsAndQueriesCorrectly) {
        try {
            // Clear internal DB instance to ensure clean test environment
            DotNetDupe::System::Data::Internal::DatabaseEngine::Instance().ClearDatabase("TestShopDb");

            // Given
            SqlConnection conn("Data Source=TestShopDb;");
            conn.Open();
            ASSERT_TRUE(conn.IsOpen());
            ASSERT_EQ(conn.GetDatabaseName(), "TestShopDb");

            // 1. CREATE TABLE
            auto cmdCreate = conn.CreateCommand();
            cmdCreate->SetCommandText("CREATE TABLE Products (Id INT, Name VARCHAR, Price INT)");
            cmdCreate->ExecuteNonQuery();

            // 2. INSERT Items
            auto cmdInsert = conn.CreateCommand();
            cmdInsert->SetCommandText("INSERT INTO Products (Id, Name, Price) VALUES (@id, @name, @price)");

            cmdInsert->GetParameters()->AddWithValue("@id", 1);
            cmdInsert->GetParameters()->AddWithValue("@name", "Coffee Maker");
            cmdInsert->GetParameters()->AddWithValue("@price", 85);
            int rows1 = cmdInsert->ExecuteNonQuery();
            ASSERT_EQ(rows1, 1);

            cmdInsert->GetParameters()->Clear();
            cmdInsert->GetParameters()->AddWithValue("@id", 2);
            cmdInsert->GetParameters()->AddWithValue("@name", "Toaster");
            cmdInsert->GetParameters()->AddWithValue("@price", 45);
            int rows2 = cmdInsert->ExecuteNonQuery();
            ASSERT_EQ(rows2, 1);

            cmdInsert->GetParameters()->Clear();
            cmdInsert->GetParameters()->AddWithValue("@id", 3);
            cmdInsert->GetParameters()->AddWithValue("@name", "Blender");
            cmdInsert->GetParameters()->AddWithValue("@price", 60);
            int rows3 = cmdInsert->ExecuteNonQuery();
            ASSERT_EQ(rows3, 1);

            // 3. SELECT ALL
            auto cmdSelectAll = conn.CreateCommand();
            cmdSelectAll->SetCommandText("SELECT * FROM Products");
            auto readerAll = cmdSelectAll->ExecuteReader();

            int count = 0;
            std::vector<std::string> names;
            std::vector<int> prices;

            while (readerAll->Read()) {
                count++;
                names.push_back(readerAll->GetString(1).GetRawString());
                prices.push_back(readerAll->GetInt32(2));
            }

            ASSERT_EQ(count, 3);
            ASSERT_EQ(names[0], "Coffee Maker");
            ASSERT_EQ(prices[0], 85);
            ASSERT_EQ(names[1], "Toaster");
            ASSERT_EQ(prices[1], 45);
            ASSERT_EQ(names[2], "Blender");
            ASSERT_EQ(prices[2], 60);

            // 4. SELECT WHERE
            auto cmdSelectWhere = conn.CreateCommand();
            cmdSelectWhere->SetCommandText("SELECT Name, Price FROM Products WHERE Id = @id");
            cmdSelectWhere->GetParameters()->AddWithValue("@id", 2);
            auto readerWhere = cmdSelectWhere->ExecuteReader();

            ASSERT_TRUE(readerWhere->Read());
            ASSERT_EQ(readerWhere->GetString(0), "Toaster");
            ASSERT_EQ(readerWhere->GetInt32(1), 45);
            ASSERT_FALSE(readerWhere->Read()); // Only one match

            // 5. UPDATE
            auto cmdUpdate = conn.CreateCommand();
            cmdUpdate->SetCommandText("UPDATE Products SET Price = @price WHERE Id = @id");
            cmdUpdate->GetParameters()->AddWithValue("@id", 3);
            cmdUpdate->GetParameters()->AddWithValue("@price", 55);
            int rowsUpdated = cmdUpdate->ExecuteNonQuery();
            ASSERT_EQ(rowsUpdated, 1);

            // Verify UPDATE
            auto cmdVerifyUpdate = conn.CreateCommand();
            cmdVerifyUpdate->SetCommandText("SELECT Price FROM Products WHERE Id = @id");
            cmdVerifyUpdate->GetParameters()->AddWithValue("@id", 3);
            auto readerUpdate = cmdVerifyUpdate->ExecuteReader();
            ASSERT_TRUE(readerUpdate->Read());
            ASSERT_EQ(readerUpdate->GetInt32(0), 55);

            // 6. DELETE
            auto cmdDelete = conn.CreateCommand();
            cmdDelete->SetCommandText("DELETE FROM Products WHERE Id = @id");
            cmdDelete->GetParameters()->AddWithValue("@id", 1);
            int rowsDeleted = cmdDelete->ExecuteNonQuery();
            ASSERT_EQ(rowsDeleted, 1);

            // Verify DELETE (total count should be 2 now)
            auto cmdVerifyDelete = conn.CreateCommand();
            cmdVerifyDelete->SetCommandText("SELECT * FROM Products");
            auto readerDelete = cmdVerifyDelete->ExecuteReader();
            int finalCount = 0;
            while (readerDelete->Read()) {
                finalCount++;
            }
            ASSERT_EQ(finalCount, 2);

            conn.Close();
            ASSERT_FALSE(conn.IsOpen());

        } catch (const BasicException<char>& ex) {
            FAIL() << "BasicException thrown: " << ex.What();
        } catch (const std::exception& ex) {
            FAIL() << "std::exception thrown: " << ex.what();
        } catch (...) {
            FAIL() << "Unknown exception thrown";
        }
    }

    TEST(DatabaseTests, GivenInMemoryEngineConnection_WhenOpenCalled_Succeeds) {
        // Given
        SqlConnection conn("Data Source=TestShopDb2;Engine=InMemory;");
        
        // When
        conn.Open();
        
        // Then
        ASSERT_TRUE(conn.IsOpen());
        
        // We can verify that we can execute database commands on this connection
        auto cmd = conn.CreateCommand();
        cmd->SetCommandText("CREATE TABLE Items (Id INT)");
        ASSERT_NO_THROW(cmd->ExecuteNonQuery());
        
        conn.Close();
    }

    TEST(DatabaseTests, GivenSQLiteEngineConnectionWithoutBuildFlag_WhenOpenCalled_ThrowsInvalidOperationException) {
        // Given
        SqlConnection conn("Data Source=TestShopDb3;Engine=SQLite;");
        
        // When & Then
#if defined(DOTNETDUPE_USE_SQLITE)
        (void)conn;
#else
        ASSERT_THROW(conn.Open(), InvalidOperationException);
#endif
    }

}

