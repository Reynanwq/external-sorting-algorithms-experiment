#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <stdexcept>

struct NumberStream
{
    NumberStream(const std::string& path, bool shouldClear = false)
        : fileName{ path }
        , buffer{ 0 }
    {
        std::ios::openmode mode = std::ios::in | std::ios::out;
        mode |= shouldClear ? std::ios::trunc : std::ios::app;
        file.open(fileName, mode);
        if (!shouldClear)
        {
            file << '\n';
            Rewind();
        }
    }

    int Get()
    {
        if (file.eof())
        {
            throw std::runtime_error{ "eof" };
        }
        int last = buffer;
        file >> buffer;
        return last;
    }

    int Peek()
    {
        if (file.eof())
        {
            throw std::runtime_error{ "eof" };
        }
        return buffer;
    }

    void Put(int value)
    {
        file << value << '\n';
    }

    void Rewind()
    {
        file.clear();
        file.seekp(0);
        file.seekg(0);
        file >> buffer;
    }

    void Clear()
    {
        file.close();
        file.open(fileName, std::ios::in | std::ios::out | std::ios::trunc);
    }

    std::string fileName;
    std::fstream file;
    int buffer;
};

struct SortState
{
    std::vector<NumberStream> temporaryFiles;
    std::vector<std::size_t> logicalTapes;
    std::size_t level = 0;
};

static void MergeOne(const std::vector<NumberStream>& from, NumberStream& to)
{
    std::vector<int> inputBuffer(from.size());
    std::vector<bool> stringIsValid(from.size(), false);

    for (std::size_t i = 0; i < from.size(); ++i)
    {
        NumberStream& file = const_cast<NumberStream&>(from[i]);
        if (!file.file.eof())
        {
            inputBuffer[i] = file.Get();
            stringIsValid[i] = true;
        }
    }

    for (;;)
    {
        int max = -1;
        for (int i = 0; i < from.size(); ++i)
        {
            if (stringIsValid[i])
            {
                if (max == -1)
                {
                    max = i;
                }
                else if (inputBuffer[max] < inputBuffer[i])
                {
                    max = i;
                }
            }
        }

        if (max == -1)
        {
            return;
        }

        to.Put(inputBuffer[max]);

        NumberStream& maxFile = const_cast<NumberStream&>(from[max]);
        if (maxFile.file.eof() || maxFile.Peek() > inputBuffer[max])
        {
            stringIsValid[max] = false;
        }
        else
        {
            inputBuffer[max] = maxFile.Get();
        }
    }
}

static void Cascade(SortState& state)
{
    auto size = state.temporaryFiles.size();

    for (std::size_t i = size - 2; i > 0; --i)
    {
        std::size_t physicalIndex = state.logicalTapes[i];
        auto& currentFile = state.temporaryFiles[physicalIndex];

        while (!currentFile.file.eof())
        {
            std::vector<NumberStream> from;
            std::size_t last = state.logicalTapes[i + 1];
            for (std::size_t j = 0; j <= i; ++j)
            {
                std::size_t phisicalFrom = state.logicalTapes[j];
                from.push_back(state.temporaryFiles[phisicalFrom]);
            }

            MergeOne(from, state.temporaryFiles[last]);
            state.temporaryFiles[last].Rewind();
        }
        currentFile.Clear();
        state.temporaryFiles[state.logicalTapes[i + 1]].Rewind();
    }
    state.temporaryFiles[state.logicalTapes[0]].Clear();
}

void CascadeSort(const std::string& fileName, std::size_t numberOfFiles)
{
    if (numberOfFiles < 3)
    {
        throw std::runtime_error{ "at least 3 temporary files are required" };
    }

    SortState state;

    NumberStream inputFile{ fileName };
    if (!inputFile.file.is_open())
    {
        throw std::runtime_error{ "could not open file" };
    }
    if (inputFile.file.eof())
    {
        return;
    }

    std::string baseFileName = "tmp";
    for (std::size_t i = 0; i < numberOfFiles; ++i)
    {
        state.temporaryFiles.emplace_back(baseFileName + std::to_string(i), true);
    }

    state.logicalTapes.resize(numberOfFiles);
    for (std::size_t i = 0; i < numberOfFiles; ++i)
    {
        state.logicalTapes[i] = i;
    }

    std::cout << "*********** Fase de Ordenação em Cascata ***********\n";
    Cascade(state);

    inputFile.Clear();
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <number_of_temp_files>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::size_t numberOfFiles = std::stoul(argv[2]);

    try
    {
        CascadeSort(inputFile, numberOfFiles);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
