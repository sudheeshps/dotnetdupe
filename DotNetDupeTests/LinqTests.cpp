#include "pch.h"
#include "gtest/gtest.h"
#include "System/Linq.h"
#include "System/String.h"
#include <cstdint>

using namespace DotNetDupe::System;
using namespace DotNetDupe::System::Collections::Generic;
using namespace DotNetDupe::System::Linq;

namespace DotNetDupeTests {

    struct FileItemDto {
        String Name;
        uint64_t SizeBytes;
        String Extension;
        bool IsDirectory;

        FileItemDto() : SizeBytes(0), IsDirectory(false) {}
        FileItemDto(const String& name, uint64_t size, const String& ext, bool isDir)
            : Name(name), SizeBytes(size), Extension(ext), IsDirectory(isDir) {}

        bool operator==(const FileItemDto& other) const {
            return Name == other.Name && SizeBytes == other.SizeBytes;
        }

        bool operator<(const FileItemDto& other) const {
            if (SizeBytes != other.SizeBytes) return SizeBytes < other.SizeBytes;
            return Name < other.Name;
        }
    };

    struct StudentScore {
        String Name;
        int Grade;
        int Score;

        StudentScore() : Grade(0), Score(0) {}
        StudentScore(const String& name, int grade, int score)
            : Name(name), Grade(grade), Score(score) {}
    };

    // 1. Where Filtering
    TEST(LinqTests, GivenListOfNumbers_WhenWhereEven_ThenReturnsOnlyEvens) {
        List<int> numbers = { 1, 2, 3, 4, 5, 6 };
        auto result = numbers
            .Where([](int n) { return n % 2 == 0; })
            .ToList();

        EXPECT_EQ(result.GetCount(), 3);
        EXPECT_EQ(result[0], 2);
        EXPECT_EQ(result[1], 4);
        EXPECT_EQ(result[2], 6);
    }

    // 2. Select Projection
    TEST(LinqTests, GivenListOfStrings_WhenProjectedWithSelect_ThenTransformsTypes) {
        List<String> words = { "cat", "elephant", "dog" };
        auto lengths = words
            .Select([](const String& s) { return s.GetLength(); })
            .ToList();

        EXPECT_EQ(lengths.GetCount(), 3);
        EXPECT_EQ(lengths[0], 3);
        EXPECT_EQ(lengths[1], 8);
        EXPECT_EQ(lengths[2], 3);
    }

    // 3. SelectMany Flattening
    TEST(LinqTests, GivenListOfLists_WhenSelectMany_ThenFlattensSequence) {
        List<List<int>> matrix;
        List<int> row1 = { 1, 2 };
        List<int> row2 = { 3, 4, 5 };
        matrix.Add(row1);
        matrix.Add(row2);

        auto flat = matrix
            .SelectMany([](const List<int>& r) { return r; })
            .ToList();

        EXPECT_EQ(flat.GetCount(), 5);
        EXPECT_EQ(flat[0], 1);
        EXPECT_EQ(flat[4], 5);
    }

    // 4. OrderBy Ascending
    TEST(LinqTests, GivenUnsortedList_WhenOrderByAscending_ThenSortsDeterministically) {
        List<int> numbers = { 5, 1, 4, 2, 8 };
        auto sorted = numbers
            .OrderBy([](int n) { return n; })
            .ToList();

        EXPECT_EQ(sorted.GetCount(), 5);
        EXPECT_EQ(sorted[0], 1);
        EXPECT_EQ(sorted[1], 2);
        EXPECT_EQ(sorted[2], 4);
        EXPECT_EQ(sorted[3], 5);
        EXPECT_EQ(sorted[4], 8);
    }

    // 5. OrderBy Descending
    TEST(LinqTests, GivenUnsortedList_WhenOrderByDescending_ThenSortsReversed) {
        List<int> numbers = { 5, 1, 4, 2, 8 };
        auto sorted = numbers
            .OrderByDescending([](int n) { return n; })
            .ToList();

        EXPECT_EQ(sorted.GetCount(), 5);
        EXPECT_EQ(sorted[0], 8);
        EXPECT_EQ(sorted[1], 5);
        EXPECT_EQ(sorted[2], 4);
        EXPECT_EQ(sorted[3], 2);
        EXPECT_EQ(sorted[4], 1);
    }

    // 6. Multi-level Sorting: OrderBy + ThenBy
    TEST(LinqTests, GivenStudents_WhenOrderByGradeAndThenByScore_ThenSortsBothLevels) {
        List<StudentScore> students;
        students.Add(StudentScore("Charlie", 10, 85));
        students.Add(StudentScore("Alice", 9, 95));
        students.Add(StudentScore("Bob", 10, 90));
        students.Add(StudentScore("David", 9, 80));

        auto sorted = students
            .OrderBy([](const StudentScore& s) { return s.Grade; })
            .ThenBy([](const StudentScore& s) { return s.Score; })
            .ToList();

        EXPECT_EQ(sorted.GetCount(), 4);
        EXPECT_TRUE(sorted[0].Name == "David"); // Grade 9, Score 80
        EXPECT_TRUE(sorted[1].Name == "Alice"); // Grade 9, Score 95
        EXPECT_TRUE(sorted[2].Name == "Charlie"); // Grade 10, Score 85
        EXPECT_TRUE(sorted[3].Name == "Bob");     // Grade 10, Score 90
    }

    // 7. Multi-level Sorting: OrderBy + ThenByDescending
    TEST(LinqTests, GivenStudents_WhenOrderByGradeAndThenByScoreDesc_ThenSortsSecondaryDescending) {
        List<StudentScore> students;
        students.Add(StudentScore("Charlie", 10, 85));
        students.Add(StudentScore("Alice", 9, 95));
        students.Add(StudentScore("Bob", 10, 90));
        students.Add(StudentScore("David", 9, 80));

        auto sorted = students
            .OrderBy([](const StudentScore& s) { return s.Grade; })
            .ThenByDescending([](const StudentScore& s) { return s.Score; })
            .ToList();

        EXPECT_EQ(sorted.GetCount(), 4);
        EXPECT_TRUE(sorted[0].Name == "Alice");   // Grade 9, Score 95
        EXPECT_TRUE(sorted[1].Name == "David");   // Grade 9, Score 80
        EXPECT_TRUE(sorted[2].Name == "Bob");     // Grade 10, Score 90
        EXPECT_TRUE(sorted[3].Name == "Charlie"); // Grade 10, Score 85
    }

    // 8. Take and Skip Partitioning
    TEST(LinqTests, GivenSequence_WhenTakeAndSkip_ThenPartitionsAccurately) {
        List<int> numbers = { 10, 20, 30, 40, 50, 60 };

        auto skipped = numbers.Skip(2).ToList();
        EXPECT_EQ(skipped.GetCount(), 4);
        EXPECT_EQ(skipped[0], 30);
        EXPECT_EQ(skipped[3], 60);

        auto taken = numbers.Take(3).ToList();
        EXPECT_EQ(taken.GetCount(), 3);
        EXPECT_EQ(taken[0], 10);
        EXPECT_EQ(taken[2], 30);

        auto middle = numbers.Skip(2).Take(2).ToList();
        EXPECT_EQ(middle.GetCount(), 2);
        EXPECT_EQ(middle[0], 30);
        EXPECT_EQ(middle[1], 40);
    }

    // 9. TakeWhile and SkipWhile
    TEST(LinqTests, GivenSequence_WhenTakeWhileAndSkipWhile_ThenPartitionsConditionally) {
        List<int> numbers = { 1, 2, 3, 10, 4, 5 };

        auto taken = numbers.TakeWhile([](int n) { return n < 4; }).ToList();
        EXPECT_EQ(taken.GetCount(), 3);
        EXPECT_EQ(taken[2], 3);

        auto skipped = numbers.SkipWhile([](int n) { return n < 4; }).ToList();
        EXPECT_EQ(skipped.GetCount(), 3);
        EXPECT_EQ(skipped[0], 10);
        EXPECT_EQ(skipped[1], 4);
        EXPECT_EQ(skipped[2], 5);
    }

    // 10. Distinct Deduplication
    TEST(LinqTests, GivenDuplicateElements_WhenDistinctCalled_ThenRemovesRedundantValues) {
        List<int> numbers = { 1, 2, 2, 3, 1, 4, 3, 5 };
        auto unique = numbers.Distinct().ToList();

        EXPECT_EQ(unique.GetCount(), 5);
        EXPECT_EQ(unique[0], 1);
        EXPECT_EQ(unique[1], 2);
        EXPECT_EQ(unique[2], 3);
        EXPECT_EQ(unique[3], 4);
        EXPECT_EQ(unique[4], 5);
    }

    // 11. GroupBy
    TEST(LinqTests, GivenFilesList_WhenGroupedBySize_ThenCreatesBuckets) {
        List<FileItemDto> files;
        files.Add(FileItemDto("a.txt", 1024, ".txt", false));
        files.Add(FileItemDto("b.png", 2048, ".png", false));
        files.Add(FileItemDto("c.txt", 1024, ".txt", false));
        files.Add(FileItemDto("d.doc", 4096, ".doc", false));

        auto groups = files
            .GroupBy([](const FileItemDto& f) { return f.SizeBytes; })
            .ToList();

        EXPECT_EQ(groups.GetCount(), 3); // 1024, 2048, 4096
        EXPECT_EQ(groups[0].Key(), 1024ULL);
        EXPECT_EQ(groups[0].Count(), 2);
        EXPECT_TRUE(groups[0][0].Name == "a.txt");
        EXPECT_TRUE(groups[0][1].Name == "c.txt");
    }

    // 12. GroupBy with Element Selector
    TEST(LinqTests, GivenFiles_WhenGroupedWithElementSelector_ThenTransformsGroupItems) {
        List<FileItemDto> files;
        files.Add(FileItemDto("a.txt", 1024, ".txt", false));
        files.Add(FileItemDto("b.txt", 1024, ".txt", false));
        files.Add(FileItemDto("c.png", 2048, ".png", false));

        auto groups = files
            .GroupBy(
                [](const FileItemDto& f) { return f.SizeBytes; },
                [](const FileItemDto& f) { return f.Name; }
            )
            .ToList();

        EXPECT_EQ(groups.GetCount(), 2);
        EXPECT_EQ(groups[0].Count(), 2);
        EXPECT_TRUE(groups[0][0] == "a.txt");
        EXPECT_TRUE(groups[0][1] == "b.txt");
    }

    // 13. Concat and Union
    TEST(LinqTests, GivenTwoSequences_WhenConcatAndUnion_ThenProducesExpectedSets) {
        List<int> seq1 = { 1, 2, 3 };
        List<int> seq2 = { 3, 4, 5 };

        auto concatResult = seq1.Concat(seq2).ToList();
        EXPECT_EQ(concatResult.GetCount(), 6);

        auto unionResult = seq1.Union(seq2).ToList();
        EXPECT_EQ(unionResult.GetCount(), 5);
        EXPECT_EQ(unionResult[3], 4);
        EXPECT_EQ(unionResult[4], 5);
    }

    // 14. Intersect and Except
    TEST(LinqTests, GivenTwoSequences_WhenIntersectAndExcept_ThenProducesExpectedDifferences) {
        List<int> seq1 = { 1, 2, 3, 4 };
        List<int> seq2 = { 3, 4, 5, 6 };

        auto common = seq1.Intersect(seq2).ToList();
        EXPECT_EQ(common.GetCount(), 2);
        EXPECT_EQ(common[0], 3);
        EXPECT_EQ(common[1], 4);

        auto diff = seq1.Except(seq2).ToList();
        EXPECT_EQ(diff.GetCount(), 2);
        EXPECT_EQ(diff[0], 1);
        EXPECT_EQ(diff[1], 2);
    }

    // 15. Zip
    TEST(LinqTests, GivenTwoSequences_WhenZipCalled_ThenCombinesPairwise) {
        List<int> ids = { 1, 2, 3 };
        List<String> names = { "One", "Two", "Three" };

        auto zipped = ids
            .Zip(names, [](int id, const String& name) {
                return name + String(":") + String::FromInt(id);
            })
            .ToList();

        EXPECT_EQ(zipped.GetCount(), 3);
        EXPECT_TRUE(zipped[0] == "One:1");
        EXPECT_TRUE(zipped[1] == "Two:2");
        EXPECT_TRUE(zipped[2] == "Three:3");
    }

    // 16. Reverse
    TEST(LinqTests, GivenSequence_WhenReverseCalled_ThenInvertsOrder) {
        List<int> numbers = { 10, 20, 30 };
        auto reversed = numbers.Reverse().ToList();

        EXPECT_EQ(reversed.GetCount(), 3);
        EXPECT_EQ(reversed[0], 30);
        EXPECT_EQ(reversed[1], 20);
        EXPECT_EQ(reversed[2], 10);
    }

    // 17. Any and All Quantifiers
    TEST(LinqTests, GivenSequence_WhenAnyAndAllCalled_ThenEvaluatesCorrectly) {
        List<int> numbers = { 2, 4, 6, 8 };

        EXPECT_TRUE(numbers.Any());
        EXPECT_TRUE(numbers.Any([](int n) { return n == 6; }));
        EXPECT_FALSE(numbers.Any([](int n) { return n == 5; }));

        EXPECT_TRUE(numbers.All([](int n) { return n % 2 == 0; }));
        EXPECT_FALSE(numbers.All([](int n) { return n > 4; }));

        List<int> emptyList;
        EXPECT_FALSE(emptyList.Any());
        EXPECT_TRUE(emptyList.All([](int n) { return n == 0; }));
    }

    // 18. Contains
    TEST(LinqTests, GivenSequence_WhenContainsCalled_ThenFindsItem) {
        List<String> fruits = { "Apple", "Banana", "Cherry" };
        EXPECT_TRUE(fruits.Contains("Banana"));
        EXPECT_FALSE(fruits.Contains("Orange"));
    }

    // 19. First and Last
    TEST(LinqTests, GivenSequence_WhenFirstAndLastCalled_ThenReturnsBoundaries) {
        List<int> numbers = { 10, 20, 30, 40 };

        EXPECT_EQ(numbers.First(), 10);
        EXPECT_EQ(numbers.First([](int n) { return n > 25; }), 30);
        EXPECT_EQ(numbers.Last(), 40);
        EXPECT_EQ(numbers.Last([](int n) { return n < 35; }), 30);
    }

    // 20. First on Empty Throws
    TEST(LinqTests, GivenEmptySequence_WhenFirstCalled_ThenThrowsInvalidOperationException) {
        List<int> emptyList;
        EXPECT_THROW(emptyList.First(), InvalidOperationException);
    }

    // 21. FirstOrDefault
    TEST(LinqTests, GivenEmptySequence_WhenFirstOrDefaultCalled_ThenReturnsDefault) {
        List<int> emptyList;
        EXPECT_EQ(emptyList.FirstOrDefault(), 0);

        List<int> numbers = { 5, 10 };
        EXPECT_EQ(numbers.FirstOrDefault([](int n) { return n > 100; }), 0);
    }

    // 22. Single and SingleOrDefault
    TEST(LinqTests, GivenSingleElement_WhenSingleCalled_ThenReturnsValue) {
        List<int> singleItem = { 42 };
        EXPECT_EQ(singleItem.Single(), 42);
        EXPECT_EQ(singleItem.SingleOrDefault(), 42);

        List<int> multiples = { 10, 20 };
        EXPECT_THROW(multiples.Single(), InvalidOperationException);
        EXPECT_THROW(multiples.SingleOrDefault(), InvalidOperationException);
    }

    // 23. ElementAt and ElementAtOrDefault
    TEST(LinqTests, GivenSequence_WhenElementAtCalled_ThenRetrievesByIndex) {
        List<int> numbers = { 100, 200, 300 };
        EXPECT_EQ(numbers.ElementAt(1), 200);
        EXPECT_THROW(numbers.ElementAt(5), ArgumentOutOfRangeException);
        EXPECT_THROW(numbers.ElementAt(-1), ArgumentOutOfRangeException);

        EXPECT_EQ(numbers.ElementAtOrDefault(1), 200);
        EXPECT_EQ(numbers.ElementAtOrDefault(5, -1), -1);
    }

    // 24. Aggregations: Sum, Min, Max, Average
    TEST(LinqTests, GivenNumericSequence_WhenAggregatesCalculated_ThenComputesCorrectly) {
        List<int> numbers = { 2, 4, 6, 8, 10 };

        EXPECT_EQ(numbers.Sum(), 30);
        EXPECT_EQ(numbers.Min(), 2);
        EXPECT_EQ(numbers.Max(), 10);
        EXPECT_DOUBLE_EQ(numbers.Average(), 6.0);

        EXPECT_EQ(numbers.Where([](int) { return true; }).Sum([](int n) { return n * 2; }), 60);
        EXPECT_EQ(numbers.Where([](int) { return true; }).Min([](int n) { return n * 2; }), 4);
        EXPECT_EQ(numbers.Where([](int) { return true; }).Max([](int n) { return n * 2; }), 20);
        EXPECT_DOUBLE_EQ(numbers.Where([](int) { return true; }).Average([](int n) { return n * 2; }), 12.0);
    }

    // 25. Aggregate Fold
    TEST(LinqTests, GivenSequence_WhenAggregateCalled_ThenFoldsValues) {
        List<int> numbers = { 1, 2, 3, 4 };
        int product = numbers.Where([](int) { return true; }).Aggregate([](int acc, int x) { return acc * x; });
        EXPECT_EQ(product, 24);

        int sumWithSeed = numbers.Where([](int) { return true; }).Aggregate(10, [](int acc, int x) { return acc + x; });
        EXPECT_EQ(sumWithSeed, 20);
    }

    // 26. Generators: Range, Repeat, Empty
    TEST(LinqTests, GivenRangeAndRepeat_WhenGenerated_ThenYieldsExpectedSequences) {
        auto range = Enumerable<int>::Range(5, 4).ToList();
        EXPECT_EQ(range.GetCount(), 4);
        EXPECT_EQ(range[0], 5);
        EXPECT_EQ(range[3], 8);

        auto repeated = Enumerable<String>::Repeat("Test", 3).ToList();
        EXPECT_EQ(repeated.GetCount(), 3);
        EXPECT_TRUE(repeated[0] == "Test");
        EXPECT_TRUE(repeated[2] == "Test");

        auto emptySeq = Enumerable<double>::Empty().ToList();
        EXPECT_EQ(emptySeq.GetCount(), 0);

        EXPECT_THROW(Enumerable<int>::Range(0, -1), ArgumentOutOfRangeException);
        EXPECT_THROW(Enumerable<int>::Repeat(1, -1), ArgumentOutOfRangeException);
    }

    // 27. Materialization: ToArray, ToDictionary, ToHashSet
    TEST(LinqTests, GivenSequence_WhenMaterializedToDifferentTypes_ThenPopulatesCorrectly) {
        List<int> numbers = { 10, 20, 30 };

        Array<int> arr = numbers.ToArray();
        EXPECT_EQ(arr.GetLength(), 3);
        EXPECT_EQ(arr[0], 10);

        auto dict = numbers.ToDictionary(
            [](int n) { return n; },
            [](int n) { return String::FromInt(n); }
        );
        EXPECT_EQ(dict.GetCount(), 3);
        EXPECT_TRUE(dict[20] == "20");

        auto set = numbers.ToHashSet();
        EXPECT_EQ(set.GetCount(), 3);
        EXPECT_TRUE(set.Contains(20));
    }

    // 28. SmartX Real-world Pipeline Simulation
    TEST(LinqTests, GivenSmartXFiles_WhenComplexQueryChained_ThenFiltersAndSortsAccurately) {
        List<FileItemDto> files;
        files.Add(FileItemDto("video1.mp4", 120ULL * 1024 * 1024, ".mp4", false));
        files.Add(FileItemDto("small.txt", 1024ULL, ".txt", false));
        files.Add(FileItemDto("video2.mp4", 500ULL * 1024 * 1024, ".mp4", false));
        files.Add(FileItemDto("photos_folder", 0, "", true));
        files.Add(FileItemDto("movie.mkv", 2000ULL * 1024 * 1024, ".mkv", false));
        files.Add(FileItemDto("video3.mp4", 50ULL * 1024 * 1024, ".mp4", false));

        // SmartX Query: Find non-directory video files >= 100MB, sort by size desc, take top 2 names
        auto topVideos = files
            .Where([](const FileItemDto& f) { return !f.IsDirectory; })
            .Where([](const FileItemDto& f) { return f.Extension == ".mp4" || f.Extension == ".mkv"; })
            .Where([](const FileItemDto& f) { return f.SizeBytes >= 100ULL * 1024 * 1024; })
            .OrderByDescending([](const FileItemDto& f) { return f.SizeBytes; })
            .Select([](const FileItemDto& f) { return f.Name; })
            .Take(2)
            .ToList();

        EXPECT_EQ(topVideos.GetCount(), 2);
        EXPECT_TRUE(topVideos[0] == "movie.mkv");  // 2000 MB
        EXPECT_TRUE(topVideos[1] == "video2.mp4"); // 500 MB
    }

    // 29. Subscript and Range-based for loop
    TEST(EnumerableTest, GivenEnumerable_WhenSubscriptOrRangeForUsed_ThenAccessesElements) {
        List<int> list = { 10, 20, 30 };
        Enumerable<int> seq(list);

        // Verify operator[]
        EXPECT_EQ(seq[0], 10);
        EXPECT_EQ(seq[1], 20);
        EXPECT_EQ(seq[2], 30);

        // Verify begin()/end() range-based for loop
        int sum = 0;
        for (int val : seq) {
            sum += val;
        }
        EXPECT_EQ(sum, 60);
    }

    // 30. GroupBy + SelectMany + IGrouping
    TEST(EnumerableTest, GivenGroupBySequence_WhenSelectManyAndIGroupingUsed_ThenFlattensCorrectly) {
        List<int> numbers = { 1, 2, 2, 3, 3, 3 };

        // Verifies GroupBy, IGrouping as IEnumerable, and SelectMany
        auto flattened = numbers
            .GroupBy([](int n) { return n; })
            .Where([](const auto& g) { return g.Count() > 1; })
            .SelectMany([](const auto& g) { return g; })
            .ToList();

        EXPECT_EQ(flattened.GetCount(), 5); // 2, 2, 3, 3, 3
    }

} // namespace DotNetDupeTests
