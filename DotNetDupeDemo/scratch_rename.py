import re
import os
import glob

def clean_and_rename():
    # 1. Clean up old Demonstrate*.cpp files
    old_files = glob.glob('Demonstrate*.cpp')
    for f in old_files:
        try:
            os.remove(f)
            print(f"Removed old file {f}")
        except Exception as e:
            print(f"Error removing {f}: {e}")

    # Read original DotNetDupeDemo.cpp backing content (we can find it in the current main file or recreate)
    # Wait, the current DotNetDupeDemo.cpp was updated to only contain main().
    # But wait, we didn't save the full backing content in a variable, but we have the individual Demonstrate*.cpp files we just created!
    # Yes! We can read from the recently created Demonstrate*.cpp files before we deleted them!
    # Wait, did we already delete them? The loop above deletes them.
    # Let's delete them AFTER we read them, or we can just read them first!
    # Yes, let's read the content from each old file, write to the new file name, then delete the old file!
    
    # Let's get the list of old files before deleting:
    old_files = glob.glob('Demonstrate*.cpp')
    new_files = []
    
    for f in old_files:
        # e.g., f = "DemonstrateConsole.cpp"
        # Extract the name without Demonstrate prefix
        match = re.match(r'Demonstrate([A-Za-z0-9_]+)\.cpp', f)
        if not match:
            continue
        
        name = match.group(1)
        new_name = f"{name}Demo.cpp"
        
        # Read content from old file
        with open(f, 'r', encoding='utf-8') as sf:
            content = sf.read()
            
        # Write to new file
        with open(new_name, 'w', encoding='utf-8') as df:
            df.write(content)
            
        print(f"Created {new_name} from {f}")
        new_files.append(new_name)
        
        # Delete old file
        try:
            os.remove(f)
            print(f"Removed {f}")
        except Exception as e:
            print(f"Error removing {f}: {e}")

    # Update Demos.h with the correct function names (which can remain DemonstrateName() or we can keep them as is)
    # The function names are still void DemonstrateName(), which is fine, but let's make sure Demos.h lists them correctly.
    # The file names are now NameDemo.cpp, and they contain void DemonstrateName() which is called from main().
    # This plan works perfectly because:
    # 1. The filenames are clean (e.g. BitConverterDemo.cpp)
    # 2. main() in DotNetDupeDemo.cpp calls DemonstrateBitConverter() declared in Demos.h.
    
    # Print the XML for DotNetDupeDemo.vcxproj
    print("\nAdd these files to DotNetDupeDemo.vcxproj under <ItemGroup>:")
    for f in sorted(new_files):
        print(f'    <ClCompile Include="{f}" />')

if __name__ == '__main__':
    clean_and_rename()
