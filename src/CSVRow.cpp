#include "CSVRow.h"
#include <iostream>

std::string const& CSVRow::operator[](std::size_t index) const
{
    return m_data[index];
}

std::size_t CSVRow::size() const
{
    return m_data.size();
}

void CSVRow::readNextRow(std::istream& str)
{
    std::string         line;
    std::getline(str, line);

    std::stringstream   lineStream(line);
    std::string         cell;

    m_data.clear();
    while(std::getline(lineStream, cell, ','))
    {
        // Account for comma in quotes
        if (std::count(cell.begin(), cell.end(), '\"') == 1) {
            std::string tmp;
            std::getline(lineStream, tmp, ',');
            cell += tmp;
        }

        m_data.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        m_data.push_back("");
    }
}

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}
