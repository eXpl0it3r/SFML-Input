#include "Application.hpp"
#include "ranges.hpp"
#include "strings.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>

namespace
{
constexpr auto help =
    "  -v, --verbose   Show more information\n"
    "  -d, --dot       Generate dot diagram about localize and delocalize functions\n"
    "  -u, --utf8      Encode console output as UTF-8 instead of ANSI\n"
    "  -h, --help      Show help and exit";

struct Arguments
{
    Arguments(int argc, char* argv[]);

    bool verbose         = false;
    bool generateDiagram = false;
    bool utf8            = false;
    bool help            = false;
};

void printScancodeDescriptions(std::ostream& os, Encoder encode);
void printLocalizeAndDelocalizeOddities(std::ostream& os);
void printLocalizeAndDelocalizeDiagram(std::ostream& os);

} // namespace

int main(int argc, char* argv[])
{
    const auto args = Arguments{argc, argv};
    if (args.help)
    {
        std::cout << "Usage: " << argv[0] << " [OPTION]...\n\n" << help << '\n';

        return 0;
    }

    const auto encode = args.utf8 ? encodeStringToUtf8 : encodeStringToAnsi;

    // Show sf::Keyboard::getDescription output
    printScancodeDescriptions(std::cout, encode);

    // Show sf::Keyboard::localize and sf::Keyboard::delocalize behavior
    if (args.verbose)
        printLocalizeAndDelocalizeOddities(std::cout);
    if (args.generateDiagram)
    {
        auto ofs = std::ofstream{"diagram.dot"};
        printLocalizeAndDelocalizeDiagram(ofs);
    }

    // Check events and sf::Keyboard::isPressed behavior interactively
    if (const auto resources = Resources::load("resources"))
        return Application{*resources, encode}.run();
    else
        return 1;
}

namespace
{
Arguments::Arguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        const auto arg = std::string{argv[i]};

        if (arg == "-h" || arg == "--help")
            help = true;
        else if (arg == "-v" || arg == "--verbose")
            verbose = true;
        else if (arg == "-d" || arg == "--dot")
            generateDiagram = true;
        else if (arg == "-u" || arg == "--utf8")
            utf8 = true;
        else
        {
            help = true;
            std::cout << "Error: unknown argument " << arg << '\n';
        }
    }
}

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Key code)
{
    return os << keyIdentifier(code);
}

std::ostream& operator<<(std::ostream& os, sf::Keyboard::Scancode scancode)
{
    return os << scancodeIdentifier(scancode);
}

void printScancodeDescriptions(std::ostream& os, Encoder encode)
{
    os << "\tScancode descriptions\n\n";
    for (auto scancode : scancodes)
        os << std::right << std::setw(3) << static_cast<int>(scancode) << ' ' << std::left << std::setw(24) << scancode
           << ' ' << encode(sf::Keyboard::getDescription(scancode)) << '\n';
    os << '\n';
}

void printLocalizeAndDelocalizeOddities(std::ostream& os)
{
    os << "\tKeys for which delocalize(key) == Scan::Unknown\n\n";
    for (auto key : keys)
        if (auto scancode = sf::Keyboard::delocalize(key); scancode == sf::Keyboard::Scan::Unknown)
            os << std::setw(10) << key << " -> " << scancode << '\n';
    os << '\n';

    os << "\tOther keys for which localize(delocalize(key)) == Unknown\n\n";
    for (auto key : keys)
        if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
            if (auto key2 = sf::Keyboard::localize(scancode); key2 == sf::Keyboard::Key::Unknown)
                os << std::setw(10) << key << " -> " << std::setw(24) << scancode << " -> " << key2 << '\n';
    os << '\n';

    os << "\tOther keys for which localize(delocalize(key)) != key\n\n";
    for (auto key : keys)
        if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
            if (auto key2 = sf::Keyboard::localize(scancode); key2 != sf::Keyboard::Key::Unknown && key2 != key)
                os << std::setw(10) << key << " -> " << std::setw(24) << scancode << " -> " << key2 << '\n';
    os << '\n';

    os << "\tScancodes for which localize(scancode) == Unknown\n\n";
    for (auto scancode : scancodes)
        if (auto key = sf::Keyboard::localize(scancode); key == sf::Keyboard::Key::Unknown)
            os << std::setw(24) << scancode << " -> " << key << '\n';
    os << '\n';

    os << "\tOther scancodes for which delocalize(localize(scancode)) == Scan::Unknown\n\n";
    for (auto scancode : scancodes)
        if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Key::Unknown)
            if (auto scancode2 = sf::Keyboard::delocalize(key); scancode2 == sf::Keyboard::Scan::Unknown)
                os << std::setw(24) << scancode << " -> " << std::setw(10) << key << " -> " << scancode2 << '\n';
    os << '\n';

    os << "\tOther scancodes for which delocalize(localize(scancode)) != scancode\n\n";
    for (auto scancode : scancodes)
        if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Key::Unknown)
            if (auto scancode2 = sf::Keyboard::delocalize(key);
                scancode2 != sf::Keyboard::Scan::Unknown && scancode2 != scancode)
                os << std::setw(24) << scancode << " -> " << std::setw(10) << key << " -> " << scancode2 << '\n';
    os << '\n';
}

void printLocalizeAndDelocalizeDiagram(std::ostream& os)
{
    auto inDegreeScancode = std::array<int, static_cast<std::size_t>(sf::Keyboard::Scan::ScancodeCount)>{};
    for (auto key : keys)
        if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
            inDegreeScancode[static_cast<std::size_t>(scancode)]++;

    auto inDegreeKey = std::array<int, sf::Keyboard::KeyCount>{};
    for (auto scancode : scancodes)
        if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Key::Unknown)
            inDegreeKey[static_cast<std::size_t>(key)]++;

    os << "digraph {\n"
       << "rankdir=LR\n"
       << "node [shape=box fontname=monospace]\n";

    for (auto key : keys)
        if (auto scancode = sf::Keyboard::delocalize(key); scancode != sf::Keyboard::Scan::Unknown)
        {
            auto key2 = sf::Keyboard::localize(scancode);
            if (inDegreeScancode[static_cast<int>(scancode)] != 1 || inDegreeKey[static_cast<std::size_t>(key)] != 1 ||
                key != key2)
                os << key << " -> \"" << scancode << "\"\n";
        }
        else
        {
            os << key << " {rank=min " << key << "}\n";
        }

    for (auto scancode : scancodes)
        if (auto key = sf::Keyboard::localize(scancode); key != sf::Keyboard::Key::Unknown)
        {
            auto scancode2 = sf::Keyboard::delocalize(key);
            if (inDegreeKey[static_cast<std::size_t>(key)] != 1 || inDegreeScancode[static_cast<int>(scancode)] != 1 ||
                scancode != scancode2)
                os << key << " -> \"" << scancode << "\" [dir=back]\n";
        }
        else
        {
            // Commented to get a small graph because a lot of scancodes are mapped to Unknown.
            //os << '"' << scancode << "\" {rank=max \"" << scancode << "\"}\n";
        }

    os << "}\n";
}

} // namespace
