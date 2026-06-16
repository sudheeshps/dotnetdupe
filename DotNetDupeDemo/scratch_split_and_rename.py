import re
import os
import glob

# 1. DI Code blocks to inject
di_headers = """#include "System/IServiceProvider.h"
#include "Extensions/DependencyInjection/ServiceCollection.h"
#include "Extensions/DependencyInjection/ServiceProvider.h"
"""

di_code = """
namespace DependencyInjectionDemo {
    class IDatabaseConnection : public virtual DotNetDupe::System::Object, public DotNetDupe::System::IO::IDisposable {
    public:
        virtual ~IDatabaseConnection() = default;
        virtual void Query(const char* query) = 0;
    };

    class SqlDatabaseConnection : public IDatabaseConnection {
    public:
        SqlDatabaseConnection() {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Constructed.");
        }
        ~SqlDatabaseConnection() override {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Destructed.");
        }
        void Query(const char* query) override {
            DotNetDupe::System::Console::Write("  [SqlDatabaseConnection] Executing query: ");
            DotNetDupe::System::Console::WriteLine(query);
        }
        void Dispose() override {
            DotNetDupe::System::Console::WriteLine("  [SqlDatabaseConnection] Disposed.");
        }
    };

    class IRepository : public virtual DotNetDupe::System::Object {
    public:
        virtual ~IRepository() = default;
        virtual void SaveData(const char* data) = 0;
    };

    class SqlRepository : public IRepository {
    public:
        SqlRepository(DotNetDupe::System::SmartPointer<IDatabaseConnection> spConn) : m_spConn(spConn) {
            DotNetDupe::System::Console::WriteLine("  [SqlRepository] Constructed.");
        }
        ~SqlRepository() override {
            DotNetDupe::System::Console::WriteLine("  [SqlRepository] Destructed.");
        }
        void SaveData(const char* data) override {
            DotNetDupe::System::Console::Write("  [SqlRepository] Saving data: ");
            DotNetDupe::System::Console::WriteLine(data);
            m_spConn->Query("INSERT INTO Data VALUES (...)");
        }
    private:
        DotNetDupe::System::SmartPointer<IDatabaseConnection> m_spConn;
    };
}

void DemonstrateDependencyInjection() {
    using namespace DotNetDupe::System;
    using namespace DotNetDupe::Extensions::DependencyInjection;
    using namespace DependencyInjectionDemo;

    Console::WriteLine("\\n=== Dependency Injection Demonstration ===");
    try {
        ServiceCollection services;

        // Register IDatabaseConnection as Scoped
        services.AddScoped<IDatabaseConnection, SqlDatabaseConnection>();

        // Register IRepository as Transient, using a lambda factory to inject IDatabaseConnection
        services.AddTransient<IRepository>([](const SmartPointer<IServiceProvider>& sp) {
            auto spConn = sp->GetRequiredService<IDatabaseConnection>();
            return SmartPointer<Object>(SmartPointer<SqlRepository>::NewShared(spConn));
        });

        Console::WriteLine("Building ServiceProvider...");
        auto spProvider = services.BuildServiceProvider();

        Console::WriteLine("\\n--- Scope 1 Boundary ---");
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spScopedProvider = spScope->GetServiceProvider();

            Console::WriteLine("Resolving repository in Scope 1...");
            auto spRepo1 = spScopedProvider->GetRequiredService<IRepository>();
            spRepo1->SaveData("User1");

            Console::WriteLine("Resolving repository in Scope 1 again...");
            auto spRepo2 = spScopedProvider->GetRequiredService<IRepository>();
            spRepo2->SaveData("User2");
        } // Scope 1 disposed, SqlDatabaseConnection should be disposed

        Console::WriteLine("\\n--- Scope 2 Boundary ---");
        {
            auto spScopeFactory = spProvider->GetRequiredService<IServiceScopeFactory>();
            auto spScope = spScopeFactory->CreateScope();
            auto spScopedProvider = spScope->GetServiceProvider();

            Console::WriteLine("Resolving repository in Scope 2...");
            auto spRepo = spScopedProvider->GetRequiredService<IRepository>();
            spRepo->SaveData("User3");
        } // Scope 2 disposed, SqlDatabaseConnection should be disposed

    } catch (const BasicException<char>& ex) {
        Console::Write("Error during Dependency Injection demonstration: ");
        Console::WriteLine(ex.What());
    }
    Console::WriteLine("==========================================");
}
"""

def split_and_rename():
    # Read the restored DotNetDupeDemo.cpp
    with open('DotNetDupeDemo.cpp', 'r', encoding='utf-8') as f:
        content = f.read()

    # 2. Add DI headers at the top
    # We will insert them right after #include "System/IdentityModel/Tokens/Jwt/JWTToken.h"
    jwt_include = '#include "System/IdentityModel/Tokens/Jwt/JWTToken.h"'
    content = content.replace(jwt_include, jwt_include + "\n" + di_headers)

    # 3. Add DI code before main()
    # We will find "int main() {" and insert the DI code before it
    main_pattern = r'int\s+main\s*\(\)\s*\{'
    content = re.sub(main_pattern, di_code + "\nint main() {", content)

    # 4. Add calling code in main()
    # We will find "DemonstrateJwt();" in main() and insert "DemonstrateDependencyInjection();" after it
    jwt_call = "DemonstrateJwt();"
    content = content.replace(jwt_call, jwt_call + "\n    DemonstrateDependencyInjection();")

    # 5. Find the positions of all void Demonstrate...() { declarations
    matches = list(re.finditer(r'void\s+(Demonstrate[A-Za-z0-9_]+)\s*\(\)\s*\{', content))
    
    first_match_start = matches[0].start()
    header_block = content[:first_match_start]
    
    demo_files = []
    
    for i in range(len(matches)):
        func_name = matches[i].group(1)
        start_pos = matches[i].start()
        
        # Determine the start of this block (including any preceding helper code)
        if i == 0:
            block_start = first_match_start
        else:
            prev_content = content[matches[i-1].start():start_pos]
            last_brace_idx = prev_content.rfind('}\n')
            if last_brace_idx != -1:
                block_start = matches[i-1].start() + last_brace_idx + 2
            else:
                block_start = matches[i-1].end()
        
        # Determine the end of this block
        if i < len(matches) - 1:
            next_content = content[start_pos:matches[i+1].start()]
            last_brace_idx = next_content.rfind('}\n')
            if last_brace_idx != -1:
                end_pos = start_pos + last_brace_idx + 2
            else:
                end_pos = matches[i+1].start()
        else:
            main_match = re.search(main_pattern, content)
            main_start = main_match.start()
            next_content = content[start_pos:main_start]
            last_brace_idx = next_content.rfind('}\n')
            if last_brace_idx != -1:
                end_pos = start_pos + last_brace_idx + 2
            else:
                end_pos = main_start
        
        demo_content = content[block_start:end_pos].strip()
        
        # Create the file name using the NameDemo.cpp format
        # e.g. DemonstrateConsole -> ConsoleDemo.cpp
        demo_name = func_name.replace("Demonstrate", "")
        file_name = f"{demo_name}Demo.cpp"
        
        # Assemble file content
        file_content = header_block.strip() + "\n\n#include \"Demos.h\"\n\n" + demo_content + "\n"
        
        with open(file_name, 'w', encoding='utf-8') as df:
            df.write(file_content)
        
        demo_files.append(file_name)
        print(f"Created {file_name}")

    # Generate Demos.h
    demos_h = "#pragma once\n\n"
    for m in matches:
        func_name = m.group(1)
        demos_h += f"void {func_name}();\n"
    
    with open('Demos.h', 'w', encoding='utf-8') as hf:
        hf.write(demos_h)
    print("Created Demos.h")

    # Update DotNetDupeDemo.cpp to contain only headers, includes and main()
    main_match = re.search(main_pattern, content)
    main_content = content[main_match.start():]
    
    new_main_content = header_block.strip() + "\n\n#include \"Demos.h\"\n\n" + main_content
    with open('DotNetDupeDemo.cpp', 'w', encoding='utf-8') as mf:
        mf.write(new_main_content)
    print("Updated DotNetDupeDemo.cpp to contain main() only")

    # Output MSBuild compile lines
    print("\nAdd these files to DotNetDupeDemo.vcxproj under <ItemGroup>:")
    for f in sorted(demo_files):
        print(f'    <ClCompile Include="{f}" />')
    print('    <ClInclude Include="Demos.h" />')

if __name__ == '__main__':
    split_and_rename()
