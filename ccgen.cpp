
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <iostream>
#include <array>

std::random_device rnd;
std::default_random_engine e1(rnd());
std::uniform_int_distribution<> dist09(0, 9);

std::vector<std::string> visaPrefixList {"4539", "4556", "4916", "4532", "4929", "40240071", "4485", "4716", "4"};
std::vector<std::string> mastercardPrefixList {"51", "52", "53", "54", "55"};
std::vector<std::string> amexPrefixList {"34", "37"};
std::vector<std::string> discoverPrefixList {"6011"};
std::vector<std::string> dinersPrefixList {"300", "301", "302", "303", "36", "38"};
std::vector<std::string> enRoutePrefixList {"2014", "2149"};
std::vector<std::string> jcbPrefixList {"35"};
std::vector<std::string> voyagerPrefixList {"8699"};

template <typename T>
std::string vector_join(const std::vector<T>& v, const std::string& token)
{
	std::ostringstream result;
	for (typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); i++)
	{
		if (i != v.begin()) { result << token; };
		result << *i;
	}
	return result.str();
}

/*
'prefix' is the start of the CC number as a string, any number of digits.
'length' is the length of the CC number to generate. Typically 13 or 16
*/
std::string completed_number(const std::string& prefix, const unsigned int length)
{
	std::array<char, 16> ccnumber;	// cc number max length is 16
	for (int i=0; i < prefix.size(); ++i) { ccnumber[i] = prefix[i]; }

    // generate digits
	for (int i=prefix.size(); i<(length-1); ++i) {
		ccnumber[i] = dist09(e1) + '0';
	}

    // Calculate sum

    unsigned int sum = 0;
	unsigned int odd = 0;

	auto next = ccnumber.begin() + (length-1);
	while (next-- != ccnumber.begin()) {
		odd = (*next - '0') * 2;	// subtract '0' does ascii code -> int
        if ( odd > 9 ) {
            odd -= 9;
        }
        sum += odd;
		if (next == ccnumber.begin()) { break; }

		--next;
		sum += *next - '0';
	}

    // Calculate check digit

    auto checkdigit = int(( floor(sum/10) + 1) * 10 - sum) % 10;
	ccnumber[length-1] = checkdigit + '0';
    return std::string{ccnumber.begin(), ccnumber.begin()+length};
}

void credit_card_number(const std::vector<std::string>& prefixList, const unsigned int length, const unsigned int howMany, std::vector<std::string>& out)
{
	std::uniform_int_distribution<> dis(0, prefixList.size()-1);
    for (unsigned int i = 0; i < howMany; ++i) {
        out.emplace_back(completed_number(prefixList[ dis(rnd) ], length));
    }
}

void output(const std::string& title, const std::vector<std::string>& numbers, std::string& out)
{
	out.append("<div class='creditCardNumbers'>");
    out.append("<h3>").append(title).append("</h3>");
    out.append(vector_join<std::string>(numbers, "<br />"));
    out.append("</div>");
}

//
// Main
//
std::string ccgen()
{
	std::vector<std::string> numbers;
	std::string out;
	out.reserve(1572);

	out.append("<div class='creditCardSet'>");
	credit_card_number(mastercardPrefixList, 16, 10, numbers);
	output("Mastercard", numbers, out);
	numbers.clear();

	credit_card_number(visaPrefixList, 16, 10, numbers);
	output("VISA 16 digit", numbers, out);
	numbers.clear();
	out.append("</div>");

	out.append("<div class='creditCardSet'>");
	credit_card_number(visaPrefixList, 13, 5, numbers);
	output("VISA 13 digit", numbers, out);
	numbers.clear();

	credit_card_number(amexPrefixList, 15, 5, numbers);
	output("American Express", numbers, out);
	numbers.clear();
	out.append("</div>");

	// Minor cards

	out.append("<div class='creditCardSet'>");
	credit_card_number(discoverPrefixList, 16, 3, numbers);
	output("Discover", numbers, out);
	numbers.clear();

	credit_card_number(dinersPrefixList, 14, 3, numbers);
	output("Diners Club", numbers, out);
	numbers.clear();
	out.append("</div>");

	out.append("<div class='creditCardSet'>");
	credit_card_number(enRoutePrefixList, 15, 3, numbers);
	output("enRoute", numbers, out);
	numbers.clear();

	credit_card_number(jcbPrefixList, 16, 3, numbers);
	output("JCB", numbers, out);
	numbers.clear();
	out.append("</div>");

	out.append("<div class='creditCardSet'>");
	credit_card_number(voyagerPrefixList, 15, 3, numbers);
	output("Voyager", numbers, out);
	out.append("</div>");
	return out;
}

//int main() {
	//for (unsigned int i=0; i<10000; ++i) ccgen();
	//std::cout << ccgen() << std::endl;
//}
