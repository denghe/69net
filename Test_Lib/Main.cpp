#include "Lib/All.h"
using namespace std;
int main()
{
    FlatBuffer fb;
    fb.writes( 123, 1.23f, 1.23, "asdf", (byte)123 );
    cout << fb.dump().c_str() << endl;

    return 0;
}
