#include <iostream>
#include <string>


#include "md5.hpp"
#include "sha256.hpp"


 
using std;
 
int main(int argc, char *argv[])
{
    string md5_value = md5("grape");
    string md5_value2 = md5(md5_value);
    
    string sha256_value = sha256("grape");
    string sha256_value2 = sha256(sha256_value);
    
    
    cout << "md5 (1) of 'grape': " << md5_value << endl;
    cout << "md5 (2) of 'grape': " << md5_value2 << endl;
    
    cout << "sha256 (1) of 'grape': " << sha256_value << endl;
    cout << "sha256 (2) of 'grape': " << sha256_value2 << endl;

    return 0;
}