#include "binary.h"

void TBinary::GenerateBinary(std::vector<int> mylist, int pos){

    if(pos == _length){_finalVector.push_back(mylist);}
    else{
        auto mylist0 = mylist; auto mylist1 = mylist;
        mylist0.push_back(0); mylist1.push_back(1);

        TBinary::GenerateBinary(mylist0, pos + 1);
        TBinary::GenerateBinary(mylist1, pos + 1);
    }
}
