import re
import os

def split_demos():
    with open('DotNetDupeDemo.cpp', 'r', encoding='utf-8') as f:
        content = f.read()

    # Find the positions of all void Demonstrate...() { declarations
    matches = list(re.finditer(r'void\s+(Demonstrate[A-Za-z0-9_]+)\s*\(\)\s*\{', content))
    
    # Extract the header block (everything before the first Demonstrate function)
    first_match_start = matches[0].start()
    header_block = content[:first_match_start]
    
    # We want to find the using namespace lines and header includes to build the common header.
    # The header block contains both.
    
    # Let's parse each demo block
    demo_files = []
    
    for i in range(len(matches)):
        name = matches[i].group(1)
        start_pos = matches[i].start()
        
        # The block starts after the end of the previous function.
        # So it includes any helper classes defined before the current function.
        if i == 0:
            block_start = first_match_start
        else:
            # Find the end of the previous function.
            # We can approximate this by looking at the content between matches[i-1] and matches[i].
            # Actually, let's just find the last closing brace of the previous function.
            prev_content = content[matches[i-1].start():start_pos]
            # Find the last closing brace that is not followed by other braces at the same level (approximate).
            # A simpler way: since each Demonstrate function ends with a closing brace on a line by itself,
            # we can look for the last '}\n' in prev_content.
            last_brace_idx = prev_content.rfind('}\n')
            if last_brace_idx != -1:
                block_start = matches[i-1].start() + last_brace_idx + 2
            else:
                block_start = matches[i-1].end()
        
        # The block ends before the next function.
        if i < len(matches) - 1:
            next_content = content[start_pos:matches[i+1].start()]
            last_brace_idx = next_content.rfind('}\n')
            if last_brace_idx != -1:
                end_pos = start_pos + last_brace_idx + 2
            else:
                end_pos = matches[i+1].start()
        else:
            # The last function ends before main()
            main_match = re.search(r'int\s+main\s*\(\)\s*\{', content)
            main_start = main_match.start()
            next_content = content[start_pos:main_start]
            last_brace_idx = next_content.rfind('}\n')
            if last_brace_idx != -1:
                end_pos = start_pos + last_brace_idx + 2
            else:
                end_pos = main_start
        
        demo_content = content[block_start:end_pos].strip()
        
        # Create the file content
        file_content = header_block.strip() + "\n\n#include \"Demos.h\"\n\n" + demo_content + "\n"
        
        file_name = f"{name}.cpp"
        with open(file_name, 'w', encoding='utf-8') as df:
            df.write(file_content)
        
        demo_files.append(file_name)
        print(f"Created {file_name}")

    # Create Demos.h
    demos_h = "#pragma once\n\n"
    for m in matches:
        name = m.group(1)
        demos_h += f"void {name}();\n"
    
    with open('Demos.h', 'w', encoding='utf-8') as hf:
        hf.write(demos_h)
    print("Created Demos.h")

    # Create the new DotNetDupeDemo.cpp with only main()
    main_match = re.search(r'int\s+main\s*\(\)\s*\{', content)
    main_content = content[main_match.start():]
    
    # Assemble new main file
    new_main_content = header_block.strip() + "\n\n#include \"Demos.h\"\n\n" + main_content
    with open('DotNetDupeDemo.cpp', 'w', encoding='utf-8') as mf:
        mf.write(new_main_content)
    print("Updated DotNetDupeDemo.cpp with main() only")

    # Output MSBuild compile lines
    print("\nAdd these files to DotNetDupeDemo.vcxproj under <ItemGroup>:")
    for f in demo_files:
        print(f'    <ClCompile Include="{f}" />')
    print('    <ClInclude Include="Demos.h" />')

if __name__ == '__main__':
    split_demos()
