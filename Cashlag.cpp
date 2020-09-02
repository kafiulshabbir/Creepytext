#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;


// List of Names
const string TITLE_SOFTWARE = "Cashlag.exe"; // changeable
const string FILE_INPUT = "CashlagInput.txt"; // changeable
const string FILE_OUTPUT = "CashlagOutput.txt"; // changeable
const string FILE_INSTRUCTIONS = "CashlagInstructions.txt"; // changeable
const string HEX = "0123456789ABCDEF"; // changeable onlyif both parties do so


// List of Cryptographic Constants
const int LENGTH_KEY = 16; // changeable onlyif both parties do so
const int LENGTH_STR_SPECIAL = 8; // changeable onlyif both parties do so
const int SIZE_BLOCK = 256; // changeable onlyif both parties do so
const int SIZE_HALF_BLOCK = 16; // changeable onlyif both parties do so


// List of Formatting options
const int SPACE_AFTER = 8; // changeable onlyif both parties do so
const int LINE_AFTER = 4 * SPACE_AFTER; // changeable onlyif both parties do so


// List of special identifiers
// If the first 8 characters match, then this is the mode of decryption
const vector<string> STR_SPECIAL // changeable onlyif both parties do so
{
	"02DACBD8", "035EAF15", "04BFD256", "05B60366", "0626E884",
	"06F22A4F", "092D7875", "096AC8BF", "10FBD820", "14B26F48",
	"1933B809", "1B2A8E19", "20428456", "21B6FBE0", "23F227DE",
	"2841CBCD", "30D7AC38", "324EDE14", "339AF50D", "3AE94D34",
	"3C4195C7", "3C46D1D1", "3C69CCFB", "40964B1C", "427E4558",
	"4475707C", "44EE692F", "483D2846", "485FC99F", "486BB02A",
	"4BF8F762", "4CC6A43C", "4F05D1DE", "50782F71", "55C9AEC1",
	"588EAF2B", "58BB4C88", "5CFF3CFC", "5E94D4D9", "6E940411",
	"74BE4883", "75D1FA8E", "78CBD9C0", "7C15D991", "7C2FC5F2",
	"7C7707C3", "7F020571", "800B4976", "82EF286A", "83227C54",
	"86EFFA99", "87BDB321", "8954261E", "8C7B0FCD", "8EFF49E7",
	"8F9F316C", "912F8259", "9CE7C147", "9DFC709E", "9FED1DA2",
	"A0CD1F4A", "A207B7CD", "A449CC21", "A4E910A7", "AE7FBD8F",
	"B07C180F", "B2C3B9BF", "B4B7D8F7", "B9DA6030", "BA8BC63D",
	"BE2CD034", "BF37B010", "C2CC8101", "C3EB78BB", "C59E0AAE",
	"C88A15DC", "CA2122A4", "CAAB779F", "CC0CE741", "CC67636F",
	"D197B194", "D3C3993A", "D8D05F4A", "DBEEFBB0", "DD3DFBE2",
	"E213E66B", "E2D61A6D", "E4267A94", "E68F509D", "EAFF23A7",
	"EB6EB26E", "ED1E85F0", "ED697A73", "EF5674D4", "F0DA6CE8",
	"F2436A5D", "F3C07F25", "F6040631", "F8316395", "F99ED77F"
};

// List of Variables
/*
block - 256bit int, char, xored char, represented as hexadecimal
BlockFromStr - Function, calculate Block from a string of size 2
char_temp - temporary variable, buffer, input all of the file
Decrypt - Function, to convert Hexes to ACSII string
Encrypt - Change message to Hexes
file_input - file type, for input
FILE_INPUT - constant string name for the input file
FILE_INSTRUCTIONS - constant string name for the instruction file, it is generated during the first run
file_instructions - stream type file
FILE_OUTPUT - constant string name for the OutPut file
FirstTimeOpened - what all installations are necessary if the program is opened for the first time
FormatMakeSpace - a formattinng assister function, it adds space and new lines
FormatRemoveSpace - It purifies the input file from dirt
GenerateKey - Function to employ randomness and generate the key
HEX - A list of SIZE_HALF_BLOCK, it can be changed to anyother permuataion of char
key - key is a random generted vector of size LENGTH_KEY with which XOR is performed
LENGTH_KEY - length of the key on which XOR is performed
LENGTH_STR_SPECIAL - length of special strings
LINE_AFTER - Add a line but not a space after this many chars
out_file - stream type out file
s - small name for string that requires a lot of modifications
SIZE_BLOCK - how many bits in a block, used in random generation
SIZE_HALF_BLOCK - how many bits in half a block
SPACE_AFTER - Add space after this many chars
SpecialStrMatch - Function that checks of the input file begins with a special string
str_block - a string of size two, it is a representation of a block
str_input - input string
str_maybe_special - This may be a special string, in those case move to decryption mode
str_output - variable output after Encrypt() or Decrypt() is applied
str_processed - after formatting oprions are applied to string
str_raw - unprocessed string
STR_SPECIAL - vector of special identifiers
StrFirstTime generate string of message for first time opened program
StrFromBlock - Convert a 256int to a string of size 2
TITLE_SOFTWARE - the name of the software
vec_input - input file is converted to a vec to ints for processing
x - temporary variable name used in range base for loops
*/
// List of Functions Used
void FirstTimeOpened( );
string StrFirstTime( );


bool SpecialStrMatch( const string& str_input );
vector<int> GenerateKey( );
string Encrypt( const string& str_input );
string Decrypt( const string& str_input );


// Conversion between int and string
string StrFromBlock( int block );
int BlockFromStr( string str_block );


// General Formating
string FormatMakeSpace( const string& str_input );
string FormatRemoveSpace( const string& str_input );

int main( )
{
	srand(time( nullptr ));
	ifstream file_input( FILE_INPUT );
	
	if( !file_input.is_open( ) )
	{
		FirstTimeOpened( );
		return 0;
	}
	
	string str_input;
	char char_temp;
	while( file_input.get( char_temp ) )
	{
		str_input += char_temp;
	}
	
	string str_output;
	if( SpecialStrMatch( str_input ) )
	{
		str_input = FormatRemoveSpace( str_input );
		str_output = Decrypt( str_input );
	}
	else
	{
		str_output = Encrypt( str_input );
		str_output = FormatMakeSpace( str_output );
	}
	
	ofstream out_file( FILE_OUTPUT );
	out_file << str_output;
	return 0;
}


void FirstTimeOpened( )
{
	ofstream file_input( FILE_INPUT );
	ofstream file_instructions( FILE_INSTRUCTIONS );
	
	string s = StrFirstTime( );
	cout << s;
	file_instructions << s;
	cin.get();
}

string StrFirstTime( )
{
	string s;
	s += "Cashlag Encryption Software\n";
	s += "Programmed by Kafi on 2020-07-06\n";
	s += "Converts your messgae to encrypted code based on random key.\n\n";
	
	s += "Instructions:\n";
	
	s += "1-Input file " + FILE_INPUT  + " now created in this folder" + ".\n";
	s += "2-Close " + TITLE_SOFTWARE  + " by [ENTER]" + ".\n";
	s += "3-Open " + FILE_INPUT  + " using notepad" + ".\n";
	s += "4-Type your message, or paste the encrypted code in " + FILE_INPUT + ".\n";;
	s += "5-Save "	+ FILE_INPUT + " by [CTRL + S]" + ".\n";
	s += "6-DoubleClick "	+ TITLE_SOFTWARE + ".\n";
	s += "7-Open "	+ FILE_OUTPUT + " to see the encrypted message" + ".\n";
	s += "8-If the file was already open, close and open again.\n";
	
	s += "\nSuccessful Installation Complete! Wish You Secret Messaging!\n";
	s += "\nPress [ENTER] to close now!\n";
	return s;
}


bool SpecialStrMatch( const string& str_input )
{
	if( str_input.size( ) < LENGTH_STR_SPECIAL )
	{
		return false;
	}
	
	string str_maybe_special = str_input.substr( 0, LENGTH_STR_SPECIAL );
	return find
	(
		STR_SPECIAL.begin(),
		STR_SPECIAL.end(),
		str_maybe_special
	)
	!= STR_SPECIAL.end();
}

vector<int> GenerateKey( )
{
	vector<int> key( LENGTH_KEY );
	for( int& x: key )
	{
		x = rand() % SIZE_BLOCK;
	}
	return key;
}

string Encrypt( const string& str_input )
{
	string str_output = STR_SPECIAL[rand() % STR_SPECIAL.size()];
	
	vector<int> key = GenerateKey( );
	for( int x: key )
	{
		str_output += StrFromBlock( x );
	}
	
	for( int i = 0; i < str_input.size(); i++ )
	{
		int block = str_input[i];
		block ^= key[i % LENGTH_KEY];
		str_output += StrFromBlock( block );
	}
	
	return str_output;
}

string Decrypt( const string& str_input )
{
	vector<int> vec_input;
	for( int i = 0; i < str_input.size() - 1; i += 2 )
	{	
		vec_input.push_back( BlockFromStr(str_input.substr(i, 2)) );
	}

	string str_output;
	for( int i = 0; i < vec_input.size() - LENGTH_KEY; i++ )
	{
		int block = vec_input[i % LENGTH_KEY] ^ vec_input[i + LENGTH_KEY];
		str_output += static_cast<char>(block);
	}
	return str_output;
}

string StrFromBlock( int block )
{
	string s;
	for( int i = 0; i < 2; i++ )
	{
		s += HEX[block & (SIZE_HALF_BLOCK - 1)];
		block /= SIZE_HALF_BLOCK;
	}
	reverse( s.begin(), s.end() );
	return s;
}

int BlockFromStr( string str_block ){
	int block = 0;
	for( int x: str_block ){
		block *= SIZE_HALF_BLOCK;
		block += HEX.find( x );
	}
	return block;
}

string FormatMakeSpace( const string& str_raw )
{
	string str_processed;
	for( int i = 0; i < str_raw.size(); i++ )
	{
		str_processed += str_raw[i];
		if( (i + 1) % LINE_AFTER == 0 )
		{
			str_processed += '\n';
		}
		else if( (i + 1) % SPACE_AFTER == 0 )
		{
			str_processed += ' ';
		}
		
	}
	return str_processed;
}

string FormatRemoveSpace( const string& str_raw )
{
	string str_processed;
	for( int i = LENGTH_STR_SPECIAL; i < str_raw.size(); i++ )
	{
		if( HEX.find(str_raw[i]) != string::npos )
		{
			str_processed += str_raw[i];
		}
	}
	return str_processed;
}