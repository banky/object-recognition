#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const;
        std::size_t size() const;
        void readNextRow(std::istream& str);
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data);