
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>

#include <iostream> // TODO: remove!

const std::string& get_quest_giver_name(std::vector<std::string>::size_type i) {
    const std::vector<std::string> quest_givers{
        "Merchant",
        "#1",
        "#2",
        "#3",
        "Foreign Trader",
        "Pharmacist",
        "Medici",
        "Hanseatic League",
        "Fugger",
        "Schulz",
        "Mayor",
        "#11"
    };

    assert(i < quest_givers.size());

    return quest_givers[i];
}

// This data comes from a separate data file!
const std::vector<std::string> load_items(std::ifstream & is) {
    assert( is.is_open() );

    is.seekg(0, is.beg);
    std::uint8_t item_count_in;
    is.read(reinterpret_cast<char*>(&item_count_in), sizeof item_count_in);
    std::uint16_t item_count = item_count_in;

    is.seekg(3, is.beg);

    std::vector<std::string> item_descriptions;
    for ( auto i = 0u; i != item_count; ++i ) {
        char item_description[46];
        is.read(item_description, sizeof item_description); // this sizeof is okay!
        if ( item_description[0] != '\0' )
            item_descriptions.push_back(item_description);
    }

    return item_descriptions;
}

struct Location
{
    char unknown1[2];   // 2
    std::int8_t pos_x;  // 3
    std::int8_t pos_y;  // 4
    char unknown2[34];  // 48
    char name[20];      // 50
};

// This is from the save file however!
const std::vector<Location> load_locations(
        std::istringstream & is,
        unsigned int end_quest_offset
) {
    //assert( is.is_open() ); // implicit of istringstream?

    is.seekg(end_quest_offset, is.beg);
    std::uint16_t location_count;
    is.read(reinterpret_cast<char*>(&location_count), sizeof location_count);

    static_assert(
        std::is_pod<Location>::value,
        "Bad Assumption! Quest must be a POD!"
    );
    assert( sizeof(Location) == 58u );

    std::vector<Location> locations;

    // Vector memory is contiguous and struct Location is a POD,
    // so we can potentially read everything in one go...
    for ( auto i = 0u; i != location_count; ++i ) {
        Location l;
        is.read(reinterpret_cast<char*>(&l), sizeof l);
        locations.push_back(l);
    }

    return locations;
}
