
#include <DataFunctions.hpp>

#include <cstdint>
#include <sstream>

struct Quest {
    int16_t field1;
    int16_t field2;
    int16_t field3;
    int16_t field4;
    int16_t field5;
    int16_t field6;
    int16_t field7;
    int16_t field8;
    int16_t field9;
    int16_t field10;
    int16_t field11;
    int16_t field12;
    int16_t field13;
    int16_t field14;
    int16_t field15;
    int16_t field16;
    int16_t field17;
    int16_t field18;
    int16_t field19;
    int16_t field20;
    int16_t field21;
    int16_t field22;
    int16_t field23;
    int16_t field24;

    bool invalid1() const {
        return field14 < 0 or field13 == field9;
    }

    bool invalid2() const {
        return (field18 != 8 or field22 == 0 or field23 != 2)
            and field24 == 0;
    }

    bool valid() const {
        return not invalid1() and not invalid2();
    }
};

std::string display_quest(
        const Quest & q,
        const std::vector<std::string> & items,
        const std::vector<Location> & locations
) {
    std::ostringstream os;

    if ( q.field24 != 0 ) {
        if ( q.field13 == 1499 )
            os << "Get " << items[q.field24] << " from ";
        else
            os << "Return (item " << q.field24 << ") to "
               << get_quest_giver_name(q.field14) << " at ";

        os << locations[q.field15].name << ".";
    }
    else {
        os << "Raubitter Quest - got to - " << locations[q.field15].name << ".";
    }

    return os.str();
}
