#include "System/Console.h"
#include "System/String.h"
#include "System/Linq.h"
#include "System/Collections/Generic/List.h"
#include <cstdint>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::System::Linq;

struct DemoFileItem {
    String Name;
    uint64_t SizeBytes;
    String Extension;
    bool IsDirectory;

    DemoFileItem() : SizeBytes(0), IsDirectory(false) {}
    DemoFileItem(const String& name, uint64_t size, const String& ext, bool isDir)
        : Name(name), SizeBytes(size), Extension(ext), IsDirectory(isDir) {}

    bool operator==(const DemoFileItem& other) const {
        return Name == other.Name && SizeBytes == other.SizeBytes;
    }

    bool operator<(const DemoFileItem& other) const {
        if (SizeBytes != other.SizeBytes) return SizeBytes < other.SizeBytes;
        return Name < other.Name;
    }
};

void DemonstrateLinq() {
    Console::WriteLine("\n=== System.Linq (Language Integrated Query) Demo ===");

    // 1. Prepare sample dataset
    List<DemoFileItem> files;
    files.Add(DemoFileItem("report.pdf", 2500000ULL, ".pdf", false));
    files.Add(DemoFileItem("video_clip.mp4", 125000000ULL, ".mp4", false));
    files.Add(DemoFileItem("photos", 0ULL, "", true));
    files.Add(DemoFileItem("large_dataset.csv", 45000000ULL, ".csv", false));
    files.Add(DemoFileItem("backup_copy.csv", 45000000ULL, ".csv", false));
    files.Add(DemoFileItem("presentation.pptx", 12000000ULL, ".pptx", false));
    files.Add(DemoFileItem("notes.txt", 15000ULL, ".txt", false));

    // 2. Query 1: Filter non-directory files >= 10MB
    Console::WriteLine("\n--- Large Files (>= 10 MB) ---");
    auto largeFiles = AsEnumerable(files)
        .Where([](const DemoFileItem& f) { return !f.IsDirectory && f.SizeBytes >= 10000000ULL; })
        .OrderByDescending([](const DemoFileItem& f) { return f.SizeBytes; })
        .Select([](const DemoFileItem& f) { return f.Name + " (" + String::FromInt((int)(f.SizeBytes / 1000000ULL)) + " MB)"; })
        .ToList();

    for (int i = 0; i < largeFiles.GetCount(); ++i) {
        Console::WriteLine("  - {0}", largeFiles[i]);
    }

    // 3. Query 2: Duplicate file size detection using GroupBy
    Console::WriteLine("\n--- Potential Duplicate Files by Size (GroupBy) ---");
    auto duplicateGroups = AsEnumerable(files)
        .Where([](const DemoFileItem& f) { return !f.IsDirectory; })
        .GroupBy([](const DemoFileItem& f) { return f.SizeBytes; })
        .Where([](const auto& g) { return g.Count() > 1; })
        .ToList();

    for (int i = 0; i < duplicateGroups.GetCount(); ++i) {
        Console::WriteLine("  Size Bucket {0} bytes (Count: {1}):", duplicateGroups[i].Key(), duplicateGroups[i].Count());
        for (int j = 0; j < duplicateGroups[i].Count(); ++j) {
            Console::WriteLine("    * {0}", duplicateGroups[i][j].Name);
        }
    }

    // 4. Query 3: Numeric Aggregations
    List<int> numbers = { 10, 25, 40, 15, 60, 30 };
    Console::WriteLine("\n--- Numeric Aggregations ---");
    int sum = AsEnumerable(numbers).Sum();
    int min = AsEnumerable(numbers).Min();
    int max = AsEnumerable(numbers).Max();
    double avg = AsEnumerable(numbers).Average();
    Console::WriteLine("  Sum: {0}, Min: {1}, Max: {2}, Average: {3}", sum, min, max, avg);

    // 5. Query 4: Generators
    auto range = Enumerable<int>::Range(1, 5).Select([](int n) { return n * 10; }).ToList();
    Console::WriteLine("  Generated Range(1, 5) * 10: [{0}, {1}, {2}, {3}, {4}]",
        range[0], range[1], range[2], range[3], range[4]);
}
