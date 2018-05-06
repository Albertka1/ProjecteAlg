#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h>


#include "md5.hpp"
#include "sha256.hpp"


 
using std::cout; using std::endl;
 
int main(int argc, char *argv[])
{
    std::string md5_value = md5("grape");
    std::string md5_value2 = md5(md5_value);
    
    std::string sha256_value = sha256("grape");
    std::string sha256_value2 = sha256(sha256_value);
    
    
    cout << "md5 (1) of 'grape': " << md5_value << endl;
    cout << "md5 (2) of 'grape': " << md5_value2 << endl;
    
    cout << "sha256 (1) of 'grape': " << sha256_value << endl;
    cout << "sha256 (2) of 'grape': " << sha256_value2 << endl;
    return 0;
}