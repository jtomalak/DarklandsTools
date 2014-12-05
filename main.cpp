
#include <Quest.hpp>

#include <fstream>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <vector>
#include <type_traits>
#include <cassert>

int main(int argc, char** argv) {

    if ( argc != 2 ) {
        std::cerr << "ERROR: You must provide the path to the Darklands data file"
                  << " (DARKLAND.LST) as the first argument.\n" << std::endl;
        return 1;
    }

    std::ifstream data_is(argv[1]);
    auto items = load_items(data_is);

    std::string filename("DKSAVE13.SAV");
    std::ifstream is(filename, std::ifstream::binary);

    // Read the whole file into memory since it is quite small
    // (Usual size ~30kb)
    is.seekg(0, is.end);
    auto filelength = is.tellg();
    is.seekg(0, is.beg);

    std::string contents(filelength, ' ');
    char* begin = &*contents.begin(); // BUG? std::string is not guaranteed to be contiguous

    is.read(begin, filelength);
    is.close();

    // Now parse the contents of the file from binary data.
    // Extract ...
    std::istringstream binss(contents);

    const std::vector<std::string> compass{
        "N", "NE", "E", "SE", "S", "SW", "W", "NW"
    };

    std::int16_t year;
    std::int16_t month;
    std::int16_t day;
    std::int16_t hour;
    std::uint8_t character_count_in;
    std::uint16_t character_count;

    binss.seekg(104u, binss.beg);
    binss.read(reinterpret_cast<char*>(&year), sizeof year);
    binss.read(reinterpret_cast<char*>(&month), sizeof month);
    ++month; // month is stored as int16_t starting with Januaury as 0
    binss.read(reinterpret_cast<char*>(&day), sizeof day);
    binss.read(reinterpret_cast<char*>(&hour), sizeof hour);

    binss.seekg(241u, binss.beg);
    binss.read(reinterpret_cast<char*>(&character_count_in), sizeof character_count_in);
    character_count = character_count_in;

    assert(binss.tellg() == 242u);

    auto quest_count_offset = 393u + character_count * 554u;
    binss.seekg(quest_count_offset, binss.beg);
    std::int16_t quest_count;
    binss.read(reinterpret_cast<char*>(&quest_count), sizeof quest_count);

    auto first_quest_offset = quest_count_offset + sizeof quest_count;
    binss.seekg(first_quest_offset, binss.beg); // No really necessary!

    static_assert(
        std::is_pod<Quest>::value,
        "Bad Assumption! Quest must be a POD!"
    );
    assert( sizeof(Quest) == 48u );

    std::vector<Quest> quests;

    for ( auto i = 0u; i != quest_count; ++i ) {
        Quest q;
        binss.read(reinterpret_cast<char*>(&q), sizeof q);
        quests.push_back(q); // blegh, kind of inefficient - can construct in place?
    }

    auto location_data_offset = first_quest_offset + (quest_count * 48);
    auto locations = load_locations(binss, location_data_offset);

    // TODO: convert ASCII to show umlauts and such...
    char current_location[80];
    char save_name[80];

    binss.seekg(0u, binss.beg);
    // NOTE: These strings are null-terminated in the save files...
    binss.read(current_location, 21u);
    binss.read(save_name, 80u);

    std::cout << filename << ": " << save_name << "\n"
              << "  Date:" << day << "/" << month << "/" << year << " hour " << hour << "\n"
              << "  Current Location: " << current_location << "\n"
              << "  Number of Characters: " << character_count << "\n"
              << "  Number of Locations: " << locations.size() << "\n"
              << "Quests (" << quest_count << "):\n";
    for ( const auto & quest : quests ) {
        if ( quest.valid() )
            std::cout << " * " << display_quest(quest, items, locations) << "\n";
    }
    std::cout << std::endl;

    return 0;
}
