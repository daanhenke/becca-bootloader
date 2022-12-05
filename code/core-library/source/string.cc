#include <becca/string.hh>

namespace becca::string
{
    char* Append(usize value, char* string, usize radix)
    {
        static const char* table = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        auto current = string;

        do
        {
            *current++ = table[value % radix];
            value /= radix;
        }
        while (value);
        *current = '\0';

        auto reverseIndex = string;
        current--;

        while (reverseIndex < current)
        {
            auto temp = *reverseIndex;
            *reverseIndex++ = *current;
            *current-- = temp;
        }

        return string;
    }
}
