#include "exerciser.h"

void exercise(connection *C)
{
     query1(C, 1, 35, 40, 0, 0, 20, 0, 0, 6, 0, 0, 5, 0, 0.0, 10.0, 0, 0.0, 10.0);
     query1(C, 0, 10, 15, 1, 0, 0, 0, 0, 6, 0, 0, 5, 0, 0.0, 10.0, 0, 0.0, 10.0);
     
     query2(C, "Red");
     query2(C, "bla");

     query3(C, "USA");
     query3(C, "NCSU");

     query4(C, "NC", "bla");
     query4(C, "NC", "DarkBlue");

     query5(C, -1);
     query5(C, 13);
}
